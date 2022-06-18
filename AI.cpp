#include "AI.hpp"
#include "Character.hpp"
#include "CharacterAI.hpp"
#include "Game.hpp"
#include "Room.hpp"
#include "BasicTile.hpp"
#include "MagicTile.hpp"
#include <queue>
#include <vector>
#include <memory>
#include "Tile.hpp"
#include <exception>
#include <algorithm>

AI& AI::getInstance()
{
  static AI instance{};
  return instance;
}

void AI::copyGameboard()
{
  auto rooms = MagicMaze::Game::getInstance().getRooms();

  int board_row{0};
  for (std::vector<std::shared_ptr<Room>>& row_of_rooms : rooms)
  {
    for (int i{0}; i < 5; i++)
    {
      std::vector<std::shared_ptr<Tile>> gameboard_row;
      int board_col{0};

      for (std::shared_ptr<Room>& room : row_of_rooms)
      {
        for (int room_row_iterations{0}; room_row_iterations < 5; room_row_iterations++)
        {
          std::shared_ptr<Tile> current_tile = room->getRoomMap().at(board_row % 5).at(board_col % 5);

          if (std::dynamic_pointer_cast<BasicTile>(current_tile))
            gameboard_row.push_back(std::make_shared<BasicTile>(current_tile->getTileType(),
                                      current_tile->getInsideRoomId(), board_row, board_col));
          else
            gameboard_row.push_back(std::make_shared<MagicTile>(current_tile->getTileType(),
                                      current_tile->getInsideRoomId(), board_row, board_col));
          board_col++;
        }
      }
      gameboard_.push_back(gameboard_row);
      board_row++;
    }
  }
  auto characters = MagicMaze::Game::getInstance().getAllCharacters();

  for (int counter{0}; counter < MagicMaze::CHARACTERS_NUMBER; counter++)
  {
    characters_.at(counter) = std::make_shared<CharacterAI>(characters.at(counter));
    characters_.at(counter)->updateCurrentTile(characters.at(counter));
  }
}

void AI::copySpecificTile(std::shared_ptr<Tile>& tile)
{
  const std::shared_ptr<Room> room = MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId());

  const int row{ tile->getRow() + room->getRow() * 5 };
  const int col{ tile->getColumn() + room->getColumn() * 5 };

  if (std::dynamic_pointer_cast<BasicTile>(tile))
    gameboard_.at(row).at(col) = std::make_shared<BasicTile>(tile->getTileType(), tile->getInsideRoomId(), row, col);
  else
    gameboard_.at(row).at(col) = std::make_shared<MagicTile>(tile->getTileType(), tile->getInsideRoomId(), row, col);
}

std::shared_ptr<CharacterAI> AI::getCharacterAIById(const char& id)
{
  switch(id)
  {
    case 'F':
      return characters_.at(0);
    case 'T':
      return characters_.at(1);
    case 'S':
      return characters_.at(2);
  }
  return nullptr;
}

void AI::copySpecificTile(std::shared_ptr<Tile>& tile, const std::shared_ptr<Character>& character)
{
  copySpecificTile(tile);
  std::shared_ptr<CharacterAI> current_ai_character{ getCharacterAIById(character->getCharacterTypeAsChar()) };

  current_ai_character->updateCurrentTile(character);
}


void AI::determineHighPriorities()
{
  const bool buttons_visible{MagicMaze::Game::getInstance().ifAllButtonsVisible()};
  std::vector<std::shared_ptr<CharacterAI>> characters_without_specific_goal;
  for (auto& character : characters_)
  {
    if (character->getPriority() == PRIORITY::LOOT || character->getPriority() == PRIORITY::BUTTON ||
        character->getPriority() == PRIORITY::REVEAL || character->getPriority() == PRIORITY::NONE)
      characters_without_specific_goal.push_back(character);
  }

  if (!MagicMaze::Game::getInstance().getRoomById('L')->isRevealed() || !buttons_visible)
  {
    for (auto& character : characters_without_specific_goal)
      character->setPriority(PRIORITY::REVEAL);
  }
  else if (buttons_visible && !MagicMaze::Game::getInstance().checkIfAllCharactersOnButton())
  {
    for (auto& character : characters_without_specific_goal)
      character->setPriority(PRIORITY::BUTTON);
  }
  else
    for (auto& character : characters_without_specific_goal)
      character->setPriority(PRIORITY::LOOT);
}


void AI::play()
{
  determineHighPriorities();
  giveGoalsToCharacters();
}

void AI::giveGoalsToCharacters()
{
  for (auto& character : characters_)
  {
    if (!ifGoalCorrespondsToPriority(character))
      findGoalTile(character);
    else if (character->getPriority() == PRIORITY::LEAVE_TILE)
      leaveTileIfPossible(character);
    std::cout << character->getFullName() << "| Goal row:" << character->getGoalTile()->getRow() << " col:" << character->getGoalTile()->getColumn() << std::endl;
  }
}

bool AI::ifGoalCorrespondsToPriority(const std::shared_ptr<CharacterAI>& character)
{
  if (!character->hasGoal())
    return false;
  TileType goal_tile_type{ character->getGoalTile()->getTileType() };
  PRIORITY priority{ character->getPriority() };

  if (priority == PRIORITY::LOOT)
  {
    if (goal_tile_type == TileType::LOOT)
      return true;
  }
  else if (priority == PRIORITY::REVEAL)
  {
    if (ifTileReveals(character->getGoalTile()) || (goal_tile_type == TileType::CRYSTAL_BALL && 
                                                    character->getCharacterType() == CharacterType::SEER))
      return true;
  }
  else if (character->getCharacterType() == CharacterType::FIGHTER && priority == PRIORITY::FIGHT)
  {
    if (goal_tile_type == TileType::MONSTER)
      return true;
  }
  else if (character->getCharacterType() == CharacterType::THIEF && priority == PRIORITY::UNLOCK)
  {
    if (goal_tile_type == TileType::HORIZONTAL_DOOR || goal_tile_type == TileType::VERTICAL_DOOR)
      return true;
  }
  else if (priority == PRIORITY::BUTTON)
  {
    if ((character->getCharacterType() == CharacterType::FIGHTER && goal_tile_type == TileType::FIGHTER_BUTTON) ||
        (character->getCharacterType() == CharacterType::SEER && goal_tile_type == TileType::SEER_BUTTON) ||
        (character->getCharacterType() == CharacterType::THIEF && goal_tile_type == TileType::THIEF_BUTTON))
      return true;
  }
  else if (priority == PRIORITY::LEAVE_TILE)
    return true;

  return false;
}

bool AI::ifTileReveals(const std::shared_ptr<Tile>& tile)
{
  static const std::array<int, 4> ROW_VALUES { -1, 0, 1, 0};
  static const std::array<int, 4> COL_VALUES { 0, 1, 0, -1};
  const int row{tile->getRow()};
  const int col{tile->getColumn()};

  for (int i{0}; i < 4; i++)
  {
    try
    {
      std::shared_ptr<Tile> neighor{ gameboard_.at(row + ROW_VALUES.at(i)).at(col + COL_VALUES.at(i)) };
      if (!MagicMaze::Game::getInstance().getRoomById(neighor->getInsideRoomId())->isRevealed())
        return true;
    }
    catch(std::out_of_range& e)
    {
      continue;
    }
  }
  return false;
}


void AI::collectNeighborTiles(std::queue<std::shared_ptr<Tile>>& tiles, std::vector<std::vector<bool>>& visited)
{
  const int row = tiles.front()->getRow();
  const int column = tiles.front()->getColumn();

  static const std::array<int, 4> ROW_VALUES { -1, 0, 1, 0}; // UP  RIGHT  DOWN  LEFT
  static const std::array<int, 4> COL_VALUES { 0, 1, 0, -1};

  for(int n{0}; n < 4; n++)
  {
    try
    {
      std::shared_ptr<Tile> tile = gameboard_.at(row + ROW_VALUES.at(n)).at(column + COL_VALUES.at(n));
      if(!visited.at(tile->getRow()).at(tile->getColumn()) && tile->getTileType() != TileType::WALL && 
        MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId())->isRevealed())
      {
        visited.at(tile->getRow()).at(tile->getColumn()) = true;
        tiles.push(tile);
      }
    }
    catch(std::out_of_range& e)
    {
      continue;
    }
  }
}

void AI::checkPriority(const std::shared_ptr<CharacterAI> character, bool& goal_found,
                       const std::shared_ptr<Tile>& current_tile)
{
  const PRIORITY priority = character->getPriority();
  const TileType type = current_tile->getTileType();

  if(priority == PRIORITY::LOOT && type == TileType::LOOT)
  {
    character->setGoalTile(current_tile);
    goal_found = true;
  }
  else if (priority == PRIORITY::REVEAL && type != TileType::CRYSTAL_BALL && ifTileReveals(current_tile))
  { // check if this character is the best for this reveal tile
    character->setGoalTile(current_tile);
    goal_found = true;
  }
  switch(character->getCharacterType())
  {
    case CharacterType::FIGHTER:
      if((type == TileType::FIGHTER_BUTTON && priority == PRIORITY::BUTTON) ||
          (type == TileType::MONSTER && priority == PRIORITY::FIGHT))
        {
          character->setGoalTile(current_tile);
          goal_found = true;
        }
        break;
    case CharacterType::SEER:
      if((type == TileType::SEER_BUTTON && priority == PRIORITY::BUTTON) ||
          (type == TileType::CRYSTAL_BALL && priority == PRIORITY::REVEAL))
      {
        character->setGoalTile(current_tile);
        goal_found = true;
      }
      break;
    case CharacterType::THIEF:
      if((type == TileType::THIEF_BUTTON && priority == PRIORITY::BUTTON) ||
          ((TileType::HORIZONTAL_DOOR == type || TileType::VERTICAL_DOOR == type) && priority == PRIORITY::UNLOCK))
      {
        character->setGoalTile(current_tile);
        goal_found = true;
      }
      break;
    case CharacterType::NONE:
      break;
  }
}

void AI::findGoalTile(const std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  const int row = tile->getRow();
  const int column = tile->getColumn();
  bool goal_found = false;

  std::vector<std::vector<bool>> visited(gameboard_.size(), std::vector<bool>(gameboard_.at(0).size(), false));
  std::queue<std::shared_ptr<Tile>> current_tiles;

  current_tiles.push(gameboard_.at(row).at(column));
  visited.at(row).at(column) = true;

  while(!goal_found && !current_tiles.empty())
  {
    checkPriority(character, goal_found, current_tiles.front());
    collectNeighborTiles(current_tiles, visited);
    current_tiles.pop();
  }
}

void AI::getMaxPoint(int& max_cut_points, const std::shared_ptr<Tile>& goal_tile, const MagicMaze::DIRECTIONS& dir)
{
    int next_row{goal_tile->getRow()};
    int next_col{goal_tile->getColumn()};

    try
    {
      while(true)
      {
        MagicMaze::Game::changeNextRowCol(next_row, next_col, dir);
        if (!gameboard_.at(next_row).at(next_col)->ifAvailable())
          return;
        else
          max_cut_points++;
      }
    }
    catch(std::exception& e)
    {
      return;
    }
}

void AI::getMaxCutPoints(CutPoints& max_cut_points, const std::shared_ptr<Tile>& goal_tile)
{
    std::array<MagicMaze::DIRECTIONS, 4> directions{ MagicMaze::DIRECTIONS::DOWN, MagicMaze::DIRECTIONS::RIGHT,
                                                     MagicMaze::DIRECTIONS::LEFT, MagicMaze::DIRECTIONS::UP };

    for (int i{0}; i < 4; i++)
    {
        switch(directions.at(i))
        {
            case MagicMaze::DIRECTIONS::DOWN:
                getMaxPoint(max_cut_points.y_bottom_, goal_tile, directions.at(i));
                break;
            case MagicMaze::DIRECTIONS::RIGHT:
                getMaxPoint(max_cut_points.x_right_, goal_tile, directions.at(i));
                break;
            case MagicMaze::DIRECTIONS::LEFT:
                getMaxPoint(max_cut_points.x_left_, goal_tile, directions.at(i));
                break;
            case MagicMaze::DIRECTIONS::UP:
                getMaxPoint(max_cut_points.y_top_, goal_tile, directions.at(i));
                break;
        }
    }
}

bool AI::checkIfCuts(CUT_TYPE& cut, const std::shared_ptr<CharacterAI>& character)
{
    const std::shared_ptr<Tile> character_tile{ character->getCurrentile().lock() };
    const std::shared_ptr<Tile> goal_tile{ character->getGoalTile() };
    CutPoints max_cut_points{};

    getMaxCutPoints(max_cut_points, goal_tile);


    bool horizontal_cut = false;
    bool vertical_cut = false;

    if (character_tile->getRow() <= goal_tile->getRow() + max_cut_points.y_bottom_ &&
        character_tile->getRow() >= goal_tile->getRow() - max_cut_points.y_top_)
    {
      horizontal_cut = true;
    }
    if (character_tile->getColumn() >= goal_tile->getColumn() - max_cut_points.x_left_ &&
        character_tile->getColumn() <= goal_tile->getColumn() + max_cut_points.x_right_)
    {
      vertical_cut = true;
    }

    if (horizontal_cut && vertical_cut)
        cut = CUT_TYPE::BOTH;
    else if (horizontal_cut)
        cut = CUT_TYPE::HORIZONTAL;
    else if (vertical_cut)
        cut = CUT_TYPE::VERTICAL;
    else
        return false;

    return true;
}

bool AI::checkIfCutsInPossibleDirection(const CUT_TYPE& cut, const int& total_iterations)
{
  MagicMaze::DIRECTIONS current_direction{ MagicMaze::Game::getInstance().getCurrentDirection() };

  if (cut == CUT_TYPE::HORIZONTAL && total_iterations < 0 && current_direction == MagicMaze::DIRECTIONS::LEFT)
    return true;
  else if (cut == CUT_TYPE::HORIZONTAL && total_iterations > 0 && current_direction == MagicMaze::DIRECTIONS::RIGHT)
    return true;
  else if (cut == CUT_TYPE::VERTICAL && total_iterations < 0 && current_direction == MagicMaze::DIRECTIONS::UP)
    return true;
  else if (cut == CUT_TYPE::VERTICAL && total_iterations > 0 && current_direction == MagicMaze::DIRECTIONS::DOWN)
    return true;

  return false;
}

bool AI::checkTilesWayForAvailability(const std::shared_ptr<CharacterAI>& character, CUT_TYPE cut,
                                      bool& currently_best_way)
{
  std::shared_ptr<Tile> goal_tile{ character->getGoalTile() };
  std::shared_ptr<Tile> character_tile{ character->getCurrentile().lock() };
  const int total_iterations = (cut == CUT_TYPE::HORIZONTAL ? goal_tile->getColumn() - character_tile->getColumn() :
                                                              goal_tile->getRow() - character_tile->getRow());

  int next_row{character_tile->getRow()};
  int next_col{character_tile->getColumn()};

  const int iterations = (total_iterations > 0) ? total_iterations : -total_iterations;

  for (int i{0}; i < iterations; i++)
  {
    if (cut == CUT_TYPE::HORIZONTAL && total_iterations < 0)
      next_col--;
    else if (cut == CUT_TYPE::HORIZONTAL && total_iterations > 0)
      next_col++;
    else if (cut == CUT_TYPE::VERTICAL && total_iterations < 0)
      next_row--;
    else if (cut == CUT_TYPE::VERTICAL && total_iterations > 0)
      next_row++;

    std::shared_ptr<Tile> tile{ gameboard_.at(next_row).at(next_col) };

    if (!tile->ifAvailable() && !tile->ifContainsCharacter())
      return false;

    else if (tile->ifContainsCharacter() && i == total_iterations - 1)
    {
      std::shared_ptr<CharacterAI> ai_character = getCharacterAIById(tile->getCharacter()->getCharacterTypeAsChar());
      ai_character->setPriority(PRIORITY::LEAVE_TILE);
      leaveTileIfPossible(ai_character);
      return false;
    }
  }
  if (!checkIfCutsInPossibleDirection(cut, total_iterations))
    currently_best_way = false;

  return true;
}

bool AI::checkIfPossibleCut(const std::shared_ptr<CharacterAI>& character, CUT_TYPE& cut, bool& currently_best_way)
{
  if (checkIfCuts(cut, character))
  {
    if (cut == CUT_TYPE::BOTH)
    {
      bool best_horizontal{true};
      bool best_vertical{true};
      if (checkTilesWayForAvailability(character, CUT_TYPE::HORIZONTAL, best_horizontal) && best_horizontal)
      {
        cut = CUT_TYPE::HORIZONTAL;
        return true;
      }
      else if (checkTilesWayForAvailability(character, CUT_TYPE::VERTICAL, best_vertical) && best_vertical)
      {
        cut = CUT_TYPE::VERTICAL;
        return true;
      }
      else
      {
        currently_best_way = false;
        return true;
      }
    }
    else if (checkTilesWayForAvailability(character, cut, currently_best_way))
      return true;
  }
  return false;
}

void AI::leaveTileIfPossible(std::shared_ptr<CharacterAI>& character)
{
  int free_space{0};
  countFreeSpaceInCertainDirection(character, MagicMaze::Game::getInstance().getCurrentDirection(), free_space);
  if (free_space == 2)
  {
    // call move
    character->setPriority(PRIORITY::NONE);
  }
  else
  {
    int free_space_other_directions{0};
    checkForBestLeave(character, free_space_other_directions, free_space);
    if (free_space <= free_space_other_directions)
    {
      // call move
      character->setPriority(PRIORITY::NONE);
    }
  }
}

void AI::countFreeSpaceInCertainDirection(const std::shared_ptr<CharacterAI>& character,
                                          MagicMaze::DIRECTIONS direction, int& free_space)
{
  std::shared_ptr<Tile> character_tile{ character->getCurrentile().lock() };
  const int t_row{ character_tile->getRow() };
  const int t_column{ character_tile->getColumn() };

  free_space = 0;

  for (int i{0}; i < 2; i++)
  {
    int next_row{t_row};
    int next_col{t_column};
    try
    {
      MagicMaze::Game::getInstance().changeNextRowCol(next_row, next_col, direction);
      std::shared_ptr<Tile> next_tile{ gameboard_.at(next_row).at(next_col) };

      // todo: check for monsters
      if (MagicMaze::Game::getInstance().getRoomById(next_tile->getInsideRoomId())->isRevealed())
        break;
      else if (next_tile->ifAvailable())
        free_space++;
      else if (!next_tile->ifAvailable() && next_tile->ifContainsCharacter() && i == 0)
        continue;
      else
        break;
    }
    catch(std::out_of_range& e)
    {
      if (!free_space)
        return;
    }
  }
}

void AI::checkForBestLeave(std::shared_ptr<CharacterAI>& character, int& other_free_spaces, const int& free_space)
{
  std::shared_ptr<Tile> character_tile{ character->getCurrentile().lock() };
  static std::vector<MagicMaze::DIRECTIONS> directions { MagicMaze::DIRECTIONS::UP, MagicMaze::DIRECTIONS::DOWN,
                                                         MagicMaze::DIRECTIONS::LEFT, MagicMaze::DIRECTIONS::RIGHT };

  for (int i{0}; i < 4; i++)
  {
    if (directions.at(i) == MagicMaze::Game::getInstance().getCurrentDirection())
      continue;

    countFreeSpaceInCertainDirection(character, directions.at(i), other_free_spaces);

    if (other_free_spaces > free_space)
      return;
  }
}

void AI::printCommand(const std::vector<std::string>& command_input) const noexcept
{
  std::cout << "AI:";
  for (auto& str : command_input)
    std::cout << " " + str;
  std::cout << std::endl;
}
