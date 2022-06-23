#include "AI.hpp"
#include "Character.hpp"
#include "CharacterAI.hpp"
#include "Game.hpp"
#include "Room.hpp"
#include "BasicTile.hpp"
#include "MagicTile.hpp"
#include <queue>
#include <string>
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
    if (character->getPriority() == PRIORITY::NONE || character->getPriority() == PRIORITY::REVEAL ||
          character->getPriority() == PRIORITY::BUTTON)
    {
      characters_without_specific_goal.push_back(character);
    }
  }

  if(!MagicMaze::Game::getInstance().getRoomById('L')->isRevealed() || !buttons_visible)
  {
    for (auto& character : characters_without_specific_goal)
      character->setPriority(PRIORITY::REVEAL);
  }
  else if (buttons_visible && !buttons_used_)
  {
    for (auto& character : characters_without_specific_goal)
      character->setPriority(PRIORITY::BUTTON);
  }
  else
    for (auto& character : characters_without_specific_goal)
        character->setPriority(PRIORITY::LOOT);
}

void AI::callMove(std::shared_ptr<CharacterAI>& character, const int& distance)
{
  using namespace MagicMaze;
  if (character->hasGoal() && character->getGoalTile() == character->getCurrentile().lock())
    character->setGoalTile(nullptr);

  std::vector<std::string> move_input{std::string{"move"}, std::string{character->getCharacterTypeAsChar()}};

  move_input.push_back(Game::getInstance().getDirectionAsString(Game::getInstance().getCurrentDirection()));
  move_input.push_back(std::to_string(distance));

  MagicMaze::Game::getInstance().move(move_input);
  printCommand(move_input);
}

void AI::checkifMonsterBlocksGoalRoom(std::shared_ptr<CharacterAI>& character)
{
  if (MagicMaze::Game::getInstance().getRoomById(character->getGoalTile()->getInsideRoomId())->getNumOfMonsters() &&
      character->getCharacterType() != CharacterType::FIGHTER)
  {
    callFigherToFight();
  }
}

void AI::callFigherToFight()
{
  std::shared_ptr<CharacterAI> fighter{ getCharacterAIById('F') };
  if (fighter->getPriority() != PRIORITY::FIGHT)
  {
    fighter->setPriority(PRIORITY::FIGHT);
    findGoalTile(fighter);
    runCharacter(fighter);
  }
}

bool AI::playNextMove(std::shared_ptr<CharacterAI>& character)
{
  checkifMonsterBlocksGoalRoom(character);

  std::queue<std::shared_ptr<Tile>> q_finder;
  q_finder.push(character->getGoalTile());

  bool best_way {false};
  int distance{0};

  std::shared_ptr<CharacterAI> copied_character{ std::make_shared<CharacterAI>(*character) };
  std::vector<std::vector<bool>> finder_visited(gameboard_.size(),std::vector<bool>(gameboard_[0].size() ,false));
  CUT_TYPE cut{CUT_TYPE::NONE};

  while(!q_finder.empty())
  {
    std::shared_ptr<Tile> next_goal_tile = q_finder.front();
    finder_visited.at(next_goal_tile->getRow()).at(next_goal_tile->getColumn()) = true;
    copied_character->setGoalTile(next_goal_tile);

    if(checkIfPossibleCut(copied_character, cut, best_way, distance))
    {
      if(!best_way)
        return false;

      if(cut == CUT_TYPE::HORIZONTAL || cut == CUT_TYPE::VERTICAL)
      {
        character->setGoalTile( copied_character->getGoalTile() );
        callMove(character, distance);
        return true;
      }
    }
    if (copied_character->ifBlockedWay())
    {
      character->setBlockingCharacter(copied_character->getBlockingCharacter());
      character->setBlockedWay(true);
      return false;
    }
    collectNeighborTiles(q_finder, finder_visited, character->getCharacterType());
    q_finder.pop();
  }
  return false;
}

void AI::runCharacter(std::shared_ptr<CharacterAI>& character)
{
  MagicMaze::DIRECTIONS current_dir{ MagicMaze::Game::getInstance().getCurrentDirection() };

  while (character->hasGoal() && character->getBlockedDirection() != current_dir && playNextMove(character))
  {
    checkIfPowerCouldBeUsed(character);
    determineHighPriorities();
    giveGoalToCharacter(character);
  }
  checkIfPowerCouldBeUsed(character);
}

bool AI::checkIfCharactersBlockingWays(std::shared_ptr<CharacterAI>& character)
{
  CharacterType blocking_character{ character->getBlockingCharacter() };
  bool decision_made{false};

  for (auto& single_character : characters_)
  {
     std::pair<std::shared_ptr<CharacterAI>, std::shared_ptr<CharacterAI>> characters{character, single_character};

    if (single_character == character)
      continue;
    else if (single_character->ifBlockedWay() && single_character->getBlockingCharacter() != 
            character->getCharacterType() && !getCharacterAIById(MagicMaze::Game::getInstance().getCharacter(
            single_character->getBlockingCharacter())->getCharacterTypeAsChar())->ifBlockedWay())
    {
      break;
    }
    else if (blocking_character == single_character->getCharacterType() && single_character->getGoalTile() &&
      !single_character->ifBlockedWay() && blocking_character != CharacterType::FIGHTER &&
      MagicMaze::Game::getInstance().getRoomById(single_character->getGoalTile()->getInsideRoomId())->getNumOfMonsters())
    {
      decideWhoLeavesTile(characters, decision_made, characters.second);
    }
    else if (blocking_character == single_character->getCharacterType() && single_character->getGoalTile() &&
      !single_character->ifBlockedWay())
    {
      character->setBlockedWay(false);
      character->setBlockingCharacter(CharacterType::NONE);
    }
    else if (blocking_character == single_character->getCharacterType())
    {
      decideWhoLeavesTile(characters, decision_made, nullptr);
    }
    if (decision_made)
    {
      runCharacter(character);
      runCharacter(single_character);
      return true;
    }
  }
  return false;
}

bool AI::ifBlockingSomeoneOther(CharacterType first_type, CharacterType second_type) const
{
  for (auto& character : characters_)
  {
    if (character->getCharacterType() == first_type || character->getCharacterType() == second_type)
      continue;
    if (character->getBlockingCharacter() == first_type)
      return true;
  }

  return false;
}

void AI::decideWhoLeavesTile(std::pair<std::shared_ptr<CharacterAI>, std::shared_ptr<CharacterAI>>& characters,
    bool& decision_made, std::shared_ptr<CharacterAI> priority_character)
{
  MagicMaze::DIRECTIONS direction{ MagicMaze::Game::getInstance().getCurrentDirection() };
  bool first_blocking_other{ ifBlockingSomeoneOther(characters.first->getCharacterType(), 
                                                    characters.second->getCharacterType()) };
  bool second_blocking_other{ ifBlockingSomeoneOther(characters.second->getCharacterType(), 
                                                     characters.first->getCharacterType()) };

  int first_free_space{0};
  countFreeSpaceInCertainDirection(characters.first, direction, first_free_space);
  int second_free_space{0};
  countFreeSpaceInCertainDirection(characters.second, direction, second_free_space);

  invertDirection(direction);

  if (!characters.second->hasGoal() || priority_character == characters.second)
  {
    if (second_free_space && !second_blocking_other)
    {
      second_free_space == 1 ? callMove(characters.second, 1) : callMove(characters.second, 2);
      decision_made = true;
      characters.second->setBlockedDirection(direction);
    }
  }
  else if ((first_free_space || second_free_space) && characters.second->hasGoal() && priority_character == nullptr)
  {
    if (second_free_space && !second_blocking_other)
    {
      second_free_space == 1 ? callMove(characters.second, 1) : callMove(characters.second, 2);
      characters.second->setBlockedDirection(direction);
    }
    else if (first_free_space && !first_blocking_other)
    {
      first_free_space == 1 ? callMove(characters.first, 1) : callMove(characters.first, 2);
      characters.first->setBlockedDirection(direction);
    }
    decision_made = true;
  }

  if (decision_made)
  {
    characters.first->setBlockingCharacter(CharacterType::NONE);
    characters.second->setBlockingCharacter(CharacterType::NONE);
    characters.first->setBlockedWay(false);
    characters.second->setBlockedWay(false);
  }
}

void AI::play()
{
  if (!buttons_used_)
    buttons_used_ = MagicMaze::Game::getInstance().checkIfAllCharactersOnButton();

  determineHighPriorities();

  for(auto& character : characters_)
  {
    giveGoalToCharacter(character);
    runCharacter(character);
  }

  for (auto& character : characters_)
    if (character->ifBlockedWay())
      checkIfCharactersBlockingWays(character);
  checkIfCharacterPaused();
}

void AI::checkIfCharacterPaused()
{
  MagicMaze::DIRECTIONS current_dir{ MagicMaze::Game::getInstance().getCurrentDirection() };

  for (auto& character : characters_)
    if (character->getBlockedDirection() == current_dir)
      character->setBlockedDirection(MagicMaze::DIRECTIONS::NONE);
}

void AI::giveGoalToCharacter(std::shared_ptr<CharacterAI>& character)
{
  if (character->hasGoal() && character->getGoalTile() == character->getCurrentile().lock())
    character->setGoalTile(nullptr);

  else if (!ifGoalCorrespondsToPriority(character))
  {
    character->setGoalTile(nullptr);
    findGoalTile(character);
  }
}

bool AI::ifGoalCorrespondsToPriority(std::shared_ptr<CharacterAI>& character)
{
  if (!character->hasGoal())
    return false;

  TileType goal_tile_type{ character->getGoalTile()->getTileType() };
  PRIORITY priority{ character->getPriority() };

  if (goal_tile_type == TileType::HOURGLASS)
    return true;
  else if (priority == PRIORITY::NONE)
    return false;
  else if (priority == PRIORITY::LOOT)
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
    {
      optimizePower(character);
      return true;
    }
  }
  else if (character->getCharacterType() == CharacterType::THIEF && priority == PRIORITY::UNLOCK)
  {
    if (goal_tile_type == TileType::HORIZONTAL_DOOR || goal_tile_type == TileType::VERTICAL_DOOR)
    {
      optimizePower(character);
      return true;
    }
    else if (ifTileReveals(character->getGoalTile()))
      return true;
  }
  else if (priority == PRIORITY::BUTTON)
  {
    if ((character->getCharacterType() == CharacterType::FIGHTER && goal_tile_type == TileType::FIGHTER_BUTTON) ||
        (character->getCharacterType() == CharacterType::SEER && goal_tile_type == TileType::SEER_BUTTON) ||
        (character->getCharacterType() == CharacterType::THIEF && goal_tile_type == TileType::THIEF_BUTTON))
      return true;
  }

  return false;
}

bool AI::ifTileReveals(const std::shared_ptr<Tile>& tile)
{
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

bool AI::ifDoorBlocksWay(const std::shared_ptr<Tile>& tile, const std::shared_ptr<Room>& room)
{
  if (tile->getTileType() == TileType::HORIZONTAL_DOOR || tile->getTileType() == TileType::VERTICAL_DOOR)
  {
    if (ifDoorAtRoomEdge(room, tile))
    {
      bool if_room_accessable{ ifDoorAtRoomsEdgeAccessable(tile, room) };

      if (room->getNumOfMonsters() && !if_room_accessable)
        return false;
      else if (tile->getRow() == 0 || tile->getColumn() == 0 || tile->getRow() == static_cast<int>
               (gameboard_.size() - 1) || tile->getColumn() == static_cast<int>(gameboard_.at(0).size() - 1))
        return false;
    }
    return true;
  }
  return false;
}

void AI::callThief()
{
  std::shared_ptr<CharacterAI> thief{ getCharacterAIById('T') };
  if (thief->getPriority() != PRIORITY::UNLOCK)
  {
    thief->setPriority(PRIORITY::UNLOCK);
    findGoalTile(thief);
    runCharacter(thief);
  }
}

bool AI::ifDoorAtRoomsEdgeAccessable(const std::shared_ptr<Tile>& tile, const std::shared_ptr<Room>& room)
{
  using namespace MagicMaze;

  auto rooms{ Game::getInstance().getRooms() };
  const int room_row{room->getRow()};
  const int room_col{room->getColumn()};

  const int row{ tile->getRow() - room_row * 5};
  const int col{ tile->getColumn() - room_col * 5};

  std::shared_ptr<Room> other_room{nullptr};

  if (row == 0 || row == 4 || col == 0 || col == 4)
  {
    if (row == 0 && col != 0 && col != 4)
      other_room = Game::getInstance().getNeighborsRoom(room_row - 1, room_col);
    else if (row == 4 && col != 0 && col != 4)
      other_room = Game::getInstance().getNeighborsRoom(room_row + 1, room_col);
    else if (col == 0 && row != 0 && row != 4)
      other_room = Game::getInstance().getNeighborsRoom(room_row, room_col - 1);
    else if (col == 4 && row != 0 && row != 4)
      other_room = Game::getInstance().getNeighborsRoom(room_row, room_col + 1);
    else
      return true;
  }
  if (other_room && other_room->isRevealed())
    return true;

  return false;
}

void AI::collectNeighborTiles(std::queue<std::shared_ptr<Tile>>& tiles, std::vector<std::vector<bool>>& visited,
   CharacterType character_type)
{
  const int row = tiles.front()->getRow();
  const int column = tiles.front()->getColumn();

  for(int n{0}; n < 4; n++)
  {
    try
    {
      std::shared_ptr<Tile> tile = gameboard_.at(row + ROW_VALUES.at(n)).at(column + COL_VALUES.at(n));
      std::shared_ptr<Room> room = MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId());

      if (!room->isRevealed() || visited.at(tile->getRow()).at(tile->getColumn()))
        continue;

      if (tile->ifAvailable() || tile->ifContainsCharacter() || ifCharacterAllowsToCollectTile(character_type, tile))
      {
        if (room->getNumOfMonsters() && character_type != CharacterType::NONE && character_type != CharacterType::FIGHTER)
        {
          if (ifDoorAtRoomEdge(room, tile) && ifCharacterAllowsToCollectTile(character_type, tile))
          {
            visited.at(tile->getRow()).at(tile->getColumn()) = true;
            tiles.push(tile);
          }
          callFigherToFight();
        }
        visited.at(tile->getRow()).at(tile->getColumn()) = true;
        tiles.push(tile);
      }
      else if (ifDoorBlocksWay(tile, room))
        callThief();

    }
    catch(std::out_of_range& e)
    {
      continue;
    }
  }
}

bool AI::ifDoorAtRoomEdge(const std::shared_ptr<Room>& room, const std::shared_ptr<Tile>& tile)
{
  if (tile->getTileType() == TileType::HORIZONTAL_DOOR || tile->getTileType() == TileType::VERTICAL_DOOR)
  {
    const int row{ tile->getRow() - room->getRow() * 5};
    const int col{ tile->getColumn() - room->getColumn() * 5};
    if ((row == 0 || row == 4) || (col == 0 || col == 4))
      return true;
  }
  return false;
}

bool AI::ifCharacterAllowsToCollectTile(CharacterType character_type, const std::shared_ptr<Tile>& tile)
{
  TileType tile_type{ tile->getTileType() };
  switch(character_type)
  {
    case CharacterType::NONE:
      return false;
    case CharacterType::FIGHTER:
      if (tile_type == TileType::MONSTER)
        return true;
      break;
    case CharacterType::THIEF:
      if (tile_type == TileType::HORIZONTAL_DOOR || tile_type == TileType::VERTICAL_DOOR)
        return true;
      break;
    case CharacterType::SEER:
      break;
  }
  return false;
}

void AI::checkPriority(std::shared_ptr<CharacterAI> character, bool& goal_found,
                       const std::shared_ptr<Tile>& current_tile)
{
  const PRIORITY priority = character->getPriority();
  const TileType type = current_tile->getTileType();
  const std::shared_ptr<Room> room = MagicMaze::Game::getInstance().getRoomById(current_tile->getInsideRoomId());

  if (type == TileType::HOURGLASS)
  {
    optimizePriority(character, current_tile);
    if (character->hasGoal())
      goal_found = true;
  }
  if((priority == PRIORITY::LOOT && type == TileType::LOOT) || ((priority == PRIORITY::REVEAL ||
     priority == PRIORITY::UNLOCK) && (type != TileType::CRYSTAL_BALL && type != TileType::MONSTER && 
     type != TileType::HORIZONTAL_DOOR && type != TileType::VERTICAL_DOOR && ifTileReveals(current_tile))))
  {
    optimizePriority(character, current_tile);
    if (character->hasGoal())
      goal_found = true;
  }
  if (goal_found)
    return;

  switch(character->getCharacterType())
  {
    case CharacterType::FIGHTER:
      if((type == TileType::FIGHTER_BUTTON && priority == PRIORITY::BUTTON) || type == TileType::MONSTER)
      {
        character->setGoalTile(current_tile);
        if (type == TileType::MONSTER)
        {
          character->setPriority(PRIORITY::FIGHT);
          optimizePower(character);
        }
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
      if((type == TileType::THIEF_BUTTON && priority == PRIORITY::BUTTON) || ((TileType::HORIZONTAL_DOOR == type ||
         TileType::VERTICAL_DOOR == type) && ifDoorBlocksWay(current_tile, room)))
      {
        character->setGoalTile(current_tile);
        if (TileType::HORIZONTAL_DOOR == type || TileType::VERTICAL_DOOR == type)
        {
          character->setPriority(PRIORITY::UNLOCK);
          optimizePower(character);
        }
        goal_found = true;
      }
      break;
    case CharacterType::NONE:
      break;
  }
}

bool AI::checkIfAlreadyOnGoalTile(const std::shared_ptr<CharacterAI>& character)
{
  if (character->getPriority() == PRIORITY::BUTTON)
  {
    if (character->ifOnButton())
      return true;
  }
  return false;
}

void AI::findGoalTile(const std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<Tile> tile = character->getCurrentile().lock();
  if (checkIfAlreadyOnGoalTile(character))
    return;

  const int row = tile->getRow();
  const int column = tile->getColumn();
  bool goal_found = false;

  std::vector<std::vector<bool>> visited(gameboard_.size(), std::vector<bool>(gameboard_.at(0).size(), false));
  std::queue<std::shared_ptr<Tile>> current_tiles;

  current_tiles.push(gameboard_.at(row).at(column));
  visited.at(row).at(column) = true;

  while(!goal_found && !current_tiles.empty())
  {
    checkifTileSameAsGameboardTile(current_tiles.front());
    checkPriority(character, goal_found, current_tiles.front());
    collectNeighborTiles(current_tiles, visited, character->getCharacterType());
    current_tiles.pop();
  }
}

void AI::checkifTileSameAsGameboardTile(std::shared_ptr<Tile>& tile)
{
  if (gameboard_.at(tile->getRow()).at(tile->getColumn()) != tile)
    tile = gameboard_.at(tile->getRow()).at(tile->getColumn());
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
            case MagicMaze::DIRECTIONS::NONE:
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
                                       bool& currently_best_way, int& distance)
{
  std::shared_ptr<Tile> goal_tile{ character->getGoalTile() };
  std::shared_ptr<Tile> character_tile{ character->getCurrentile().lock() };
  const int total_iterations = (cut == CUT_TYPE::HORIZONTAL ? goal_tile->getColumn() - character_tile->getColumn() :
                                                              goal_tile->getRow() - character_tile->getRow());

  int next_row{character_tile->getRow()};
  int next_col{character_tile->getColumn()};

  distance = (total_iterations > 0) ? total_iterations : -total_iterations;

  bool character_blocking{false};
  std::shared_ptr<Tile> tile{ nullptr };
  for (int i{0}; i < distance; i++)
  {
    if (cut == CUT_TYPE::HORIZONTAL && total_iterations < 0)
      next_col--;
    else if (cut == CUT_TYPE::HORIZONTAL && total_iterations > 0)
      next_col++;
    else if (cut == CUT_TYPE::VERTICAL && total_iterations < 0)
      next_row--;
    else if (cut == CUT_TYPE::VERTICAL && total_iterations > 0)
      next_row++;

    tile = gameboard_.at(next_row).at(next_col);
    std::shared_ptr<Room> room{ MagicMaze::Game::getInstance().getRoomById(tile->getInsideRoomId()) };

    if (!tile->ifAvailable() && !tile->ifContainsCharacter())
      return false;
    else if (room->getNumOfMonsters() && character->getCharacterType() != CharacterType::FIGHTER)
      return false;

    else if (tile->ifContainsCharacter() && i == distance - 1 && ifDirectHit(character))
      character_blocking = true;
  }
  if (checkIfCutsInPossibleDirection(cut, total_iterations))
  {
    currently_best_way = true;
  }
  if (character_blocking)
  {
      character->setBlockingCharacter(tile->getCharacter()->getCharacterType());
      character->setBlockedWay(true);
      return false;
  }
  return true;
}

bool AI::checkIfPossibleCut(const std::shared_ptr<CharacterAI>& character, CUT_TYPE& cut, bool& currently_best_way,
                            int& distance)
{
  if (checkIfCuts(cut, character))
  {
    if (cut == CUT_TYPE::BOTH)
    {
      if (checkTilesWayForAvailability(character, CUT_TYPE::HORIZONTAL, currently_best_way, distance))
      {
        cut = CUT_TYPE::HORIZONTAL;
        return true;
      }
      else if (checkTilesWayForAvailability(character, CUT_TYPE::VERTICAL, currently_best_way, distance))
      {
        cut = CUT_TYPE::VERTICAL;
        return true;
      }
    }
    else
      return checkTilesWayForAvailability(character, cut, currently_best_way, distance);
  }
  return false;
}


void AI::countFreeSpaceInCertainDirection(const std::shared_ptr<CharacterAI>& character,
                                          MagicMaze::DIRECTIONS direction, int& free_space)
{
  std::shared_ptr<Tile> character_tile{ character->getCurrentile().lock() };
  int t_row{ character_tile->getRow() };
  int t_column{ character_tile->getColumn() };
  bool character_on_the_way{false};

  for (int i{0}; i < 2; i++)
  {
    try
    {
      MagicMaze::Game::getInstance().changeNextRowCol(t_row, t_column, direction);
      std::shared_ptr<Tile> next_tile{ gameboard_.at(t_row).at(t_column) };
      std::shared_ptr<Room> room{ MagicMaze::Game::getInstance().getRoomById(next_tile->getInsideRoomId()) };

      if (!room->isRevealed() || (room->getNumOfMonsters() && character->getCharacterType() != CharacterType::FIGHTER))
        break;
      else if (next_tile->ifAvailable() && i == 0)
        free_space++;
      else if (next_tile->ifAvailable() && i == 1)
      {
        if (character_on_the_way)
          free_space++;
        else
          break;
      }
      else if (!next_tile->ifAvailable() && next_tile->ifContainsCharacter() && i == 0)
      {
        free_space++;
        character_on_the_way = true;
      }
      else if(!next_tile->ifAvailable())
        break;
    }
    catch(std::out_of_range& e)
    {
      break;
    }
  }
  if (free_space == 1 && character_on_the_way)
    free_space = 0;
}

void AI::printCommand(const std::vector<std::string>& command_input) const noexcept
{
  std::cout << "AI:";
  for (auto& str : command_input)
    std::cout << " " + str;
  std::cout << std::endl;
}

void AI::optimizePower(std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<CharacterAI> copied_character = std::make_shared<CharacterAI>(*character);
  std::vector<std::vector<bool>> visited(gameboard_.size(), std::vector<bool>(gameboard_.at(0).size(), false));

  CUT_TYPE cut{ CUT_TYPE::NONE };
  int distance{0};
  bool best_way{false};

  std::queue<std::shared_ptr<Tile>> neighbors;
  neighbors.push(character->getGoalTile());
  collectNeighborTiles(neighbors, visited, character->getCharacterType());

  std::shared_ptr<Tile> current_best_tile{nullptr};
  neighbors.pop();
  while(!neighbors.empty())
  {
    copied_character->setGoalTile(neighbors.front());
    if(checkIfPossibleCut(copied_character, cut, best_way, distance))
    {
      current_best_tile = neighbors.front();
      if (ifDirectHit(copied_character))
        break;
    }
    neighbors.pop();
  }
  if (current_best_tile)
    character->setGoalTile(current_best_tile);
}


void AI::optimizePriority(std::shared_ptr<CharacterAI>& character, const std::shared_ptr<Tile>& test_tile)
{
  PRIORITY priority{ character->getPriority() };

  if (test_tile->getTileType() == TileType::HOURGLASS || priority == PRIORITY::REVEAL || priority == PRIORITY::UNLOCK)
    optimizeGoals(character, test_tile);

  else if (priority == PRIORITY::LOOT)
  {
    for (auto& single_character : characters_)
    {
      if (single_character == character)
        continue;
      else if (single_character->getGoalTile() == test_tile)
        return;
    }
    character->setGoalTile(test_tile);
  }
}

void AI::optimizeGoals(std::shared_ptr<CharacterAI>& character, const std::shared_ptr<Tile>& test_tile)
{
  if (test_tile->getTileType() == TileType::HOURGLASS && MagicMaze::Game::getInstance().getRoomById(
      test_tile->getInsideRoomId())->getNumOfMonsters() && character->getCharacterType() != CharacterType::FIGHTER)
    return;

  int counter{0};
  for (auto& single_character : characters_)
  {
    if (single_character == character)
      continue;
    if (checkIfInBetterPosition(character, single_character, test_tile))
      counter++;
  }
  if (counter == 2)
  {
    character->setGoalTile(test_tile);
    for (auto& single_character : characters_)
    {
      if (single_character == character)
        continue;
      if (single_character->getGoalTile() == test_tile)
        single_character->setGoalTile(nullptr);
    }
  }
}

bool AI::checkIfInBetterPosition(std::shared_ptr<CharacterAI>& original_current_character,
      std::shared_ptr<CharacterAI>& original_other_character, const std::shared_ptr<Tile>& test_tile)
{
  std::vector<std::vector<bool>> visited(gameboard_.size(), std::vector<bool>(gameboard_.at(0).size(), false));

  std::queue<std::shared_ptr<Tile>> q_tracker;
  q_tracker.push(test_tile);

  const bool if_others_goal{ test_tile == original_other_character->getGoalTile() };

  int distance{0};

  while(!q_tracker.empty())
  {
    std::shared_ptr<CharacterAI> current = std::make_shared<CharacterAI>(*original_current_character);
    std::shared_ptr<CharacterAI> other = std::make_shared<CharacterAI>(*original_other_character);
    std::shared_ptr<Tile> front = q_tracker.front();
    current->setGoalTile(front);
    other->setGoalTile(front);

    bool current_best_way{false};
    bool other_best_way{false};
    CUT_TYPE cut_current{CUT_TYPE::NONE};
    bool current_cuts = checkIfCuts(cut_current, current) && checkTilesWayForAvailability(current, cut_current,
                                                                                          current_best_way, distance);
    CUT_TYPE cut_other = CUT_TYPE::NONE;
    bool other_cuts = checkIfCuts(cut_other, other) && checkTilesWayForAvailability(other, cut_other, other_best_way,
                                                                                    distance);

    if (current_cuts && other_cuts)
    {
      if (MagicMaze::Game::getInstance().getCurrentRound() == 0)
      {
        if (!current_best_way)
          return false;
        if (!other_best_way)
          return true;
        if (other_best_way && if_others_goal)
          return false;
        else if (other_best_way && !if_others_goal)
        {
          original_other_character->setGoalTile(front);
          return false;
        }
      }
      if (!if_others_goal)
        return true;
      else if (if_others_goal && other_best_way)
        return false;
      else if (if_others_goal && current_best_way)
        return true;
    }
    else if (!current_cuts && other_cuts)
      return false;
    else if (current_cuts && !other_cuts)
      return true;

    collectNeighborTiles(q_tracker, visited, CharacterType::NONE);
    q_tracker.pop();
  }
  return false; 
}


bool AI::ifDirectHit(const std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<Tile> goal = character->getGoalTile();
  std::shared_ptr<Tile> current = character->getCurrentile().lock();

  if (goal->getRow() == current->getRow())
    return true;
  else if (goal->getColumn() == current->getColumn())
    return true;
  return false;
}

bool AI::checkIfPowerCouldBeUsed(std::shared_ptr<CharacterAI>& character)
{
  std::shared_ptr<Tile> current{ character->getCurrentile().lock() };
  if (character->getCharacterType() == CharacterType::SEER && current->getTileType() == TileType::CRYSTAL_BALL)
  {
    return useCertainPower(character, MagicMaze::COMMANDS::SCRY);
  }
  else if (character->getCharacterType() == CharacterType::FIGHTER && character->getPriority() == PRIORITY::FIGHT)
  {
    return useCertainPower(character, MagicMaze::COMMANDS::FIGHT);
  }
  else if (character->getCharacterType() == CharacterType::THIEF && character->getPriority() == PRIORITY::UNLOCK)
  {
    return useCertainPower(character, MagicMaze::COMMANDS::UNLOCK);
  }

  return false;
}

bool AI::useCertainPower(std::shared_ptr<CharacterAI>& character, MagicMaze::COMMANDS command)
{
  if (command == MagicMaze::COMMANDS::SCRY)
    return callScry();

  std::vector<std::vector<bool>> visited(gameboard_.size(), std::vector<bool>(gameboard_.at(0).size(), false));

  std::queue<std::shared_ptr<Tile> > neighbor_tiles;
  neighbor_tiles.push(character->getCurrentile().lock());

  collectNeighborTiles(neighbor_tiles, visited, character->getCharacterType());
  neighbor_tiles.pop();

  while(!neighbor_tiles.empty())
  {
    TileType front_type{neighbor_tiles.front()->getTileType()};

    if (command == MagicMaze::COMMANDS::FIGHT && front_type == TileType::MONSTER)
    {
      callCommand(MagicMaze::COMMANDS::FIGHT);
      character->setPriority(PRIORITY::NONE);
      character->setGoalTile(nullptr);
      return true;
    }
    else if (command == MagicMaze::COMMANDS::UNLOCK && (front_type == TileType::HORIZONTAL_DOOR ||
                                                        front_type == TileType::VERTICAL_DOOR))
    {
      callCommand(MagicMaze::COMMANDS::UNLOCK);
      character->setPriority(PRIORITY::NONE);
      character->setGoalTile(nullptr);
      return true;
    }
    neighbor_tiles.pop();
  }
  return false;
}

void AI::callCommand(MagicMaze::COMMANDS command)
{
  using namespace MagicMaze;
  std::vector<std::string> input;
  if (command == COMMANDS::FLIP)
  {
    Game::getInstance().flip();
    input.push_back(std::string{"flip"});
    printCommand(input);
  }
  else if (command == COMMANDS::FIGHT)
  {
    Game::getInstance().fightMonster();
    input.push_back(std::string{"fight"});
    printCommand(input);
  }
  else if (command == COMMANDS::UNLOCK)
  {
    Game::getInstance().unlock();
    input.push_back(std::string{"unlock"});
    printCommand(input);
  }
}

void AI::findRoomToScry(std::shared_ptr<Room>& goal_room, std::vector<std::string>& scry_input)
{
  auto rooms{ MagicMaze::Game::getInstance().getRooms() };

  for (auto& row_of_rooms : rooms)
    for (auto& single_room : row_of_rooms)
    {
      if (!single_room->isRevealed() && findScryFromRoom(single_room->getRow(), single_room->getColumn(), scry_input))
      {
        goal_room = single_room;
        return;
      }
    }
}

bool AI::callScry()
{
  std::shared_ptr<CharacterAI> seer{ getCharacterAIById('S') };
  seer->setGoalTile(nullptr);

  std::vector<std::string> scry_input{ std::string{"scry"}};
  std::shared_ptr<Room> goal_room{nullptr};
  findRoomToScry(goal_room, scry_input);

  if (!goal_room)
  {
    std::shared_ptr<Tile> tile = seer->getCurrentile().lock();
    gameboard_.at(tile->getRow()).at(tile->getColumn()) = std::make_shared<BasicTile>(tile->getTileType(),
                                                          tile->getInsideRoomId(), tile->getRow(), tile->getColumn());

    seer->updateCurrentTile(MagicMaze::Game::getInstance().getCharacter(CharacterType::SEER));
    return false;
  }

  MagicMaze::Game::getInstance().scry(scry_input);
  printCommand(scry_input);

  return true;
}

bool AI::findScryFromRoom(const int& goal_row, const int& goal_col, std::vector<std::string>& scry_input)
{
  using namespace MagicMaze;

  std::shared_ptr<Room> scry_from_room{nullptr};
  auto rooms{ Game::getInstance().getRooms() };
  std::vector<DIRECTIONS> directions{ DIRECTIONS::UP, DIRECTIONS::DOWN, DIRECTIONS::RIGHT, DIRECTIONS::LEFT };

  for (int i{0}; i < 4; i++)
  {
    int next_row{goal_row};
    int next_col{goal_col};
    try
    {
      DIRECTIONS direction{ directions.at(i) };
      Game::changeNextRowCol(next_row, next_col, direction);

      scry_from_room = rooms.at(next_row).at(next_col);
      if (scry_from_room->isRevealed())
      {
        scry_input.push_back(std::string{scry_from_room->getRoomId()});
        invertDirection(direction);
        scry_input.push_back(MagicMaze::Game::getDirectionAsString(direction));
        return true;
      }
    }
    catch(std::out_of_range& e)
    {
      continue;
    }
  }
  return false;
}

void AI::invertDirection(MagicMaze::DIRECTIONS& direction)
{
  switch (direction)
  {
    case MagicMaze::DIRECTIONS::LEFT:
      direction = MagicMaze::DIRECTIONS::RIGHT;
      break;
    case MagicMaze::DIRECTIONS::RIGHT:
      direction = MagicMaze::DIRECTIONS::LEFT;
      break;
    case MagicMaze::DIRECTIONS::UP:
      direction = MagicMaze::DIRECTIONS::DOWN;
      break;
    case MagicMaze::DIRECTIONS::DOWN:
      direction = MagicMaze::DIRECTIONS::UP;
      break;
    case MagicMaze::DIRECTIONS::NONE:
      break;
  }
}
