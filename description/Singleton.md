# Singleton

## Einleitung

In diesem Abschnitt erfährst du,

- welches Ziel mit einer Singleton-Klasse erreicht werden kann,
- wie eine Singleton-Klasse definiert werden kann und
- wie die (einzige) Instanz ener Singleton-Klasse genutzt werden kann.

Solltest du weitere Informationen über Singletons in C++ benötigen, kannst du gerne in Literatur und Internet recherchieren und natürlich sind wir bei Fragen zu diesem Thema auch im TeachCenter-Forum für dich da.

## Was ist ein Singleton?

Eine Singleton-Klasse stellt sicher, dass es nur *genau ein* Objekt dieser Klasse gibt. Wird eine Klasse also als Singleton implementiert, so kann es nicht mehr als eine Instanz der Klasse geben.

## Beispiel ein Singleton-Klasse

```C++
#ifndef TEST_SINGLETONCLASS_H
#define TEST_SINGLETONCLASS_H

class SingletonClass
{
  public:
    static SingletonClass& instance()
    {
       static SingletonClass instance_;
       return instance_;
    }
    ~SingletonClass() {}
    void function();

  private:
    SingletonClass() : variable{20} {}
    SingletonClass(const SingletonClass&); 
    SingletonClass& operator=(const SingletonClass&); 

    int variable;
};
#endif
```

Dadurch, dass die Konstruktoren `private` sind, kann eine Instanz von `SingletonClass` nur innerhalb dieser Klasse erstellt werden. Von außen kann ein `SingletonClass`-Objekt über die `static`-Methode `instance` angefordert werden. Auch bei wiederholten Aufrufen von `instance` wird immer dieselbe Instanz zurückgegeben. (Eine `static`-Variable in einer Funktion oder Methode bedeutet, dass die Variable nur ein Mal angelegt wird. Wir greifen daher bei wiederholten Aufrufen der Funktion bzw. Methode immer auf das *gleiche* Objekt zu und es wird *nicht* jedes Mal neu erzeugt.)

## Verwendung der Klasse

Um die (einzige) Instanz der Singleton-Klasse zu verwenden, kann entweder eine Referenz verwendet werden, oder direkt auf die Instanz zugegriffen werden:

```C++
// using a reference to call a member function:
SingletonClass &singleton = SingletonClass::instance();
singleton.function();
// using the instance directly to call a member function:
SingletonClass::instance().function();
```
