#ifndef ZOMBIE_H
#define ZOMBIE_H

// zombie.h — deklaracja abstrakcyjnej klasy bazowej dla wszystkich typów zombie

#include "Gameobject.h"
#include "Config.h"
#include <vector>

enum class DamageType {
    BULLET,
    EXPLOSION,
    FIRE
};

// Klasa bazowa zombie: porusza się po wyznaczonej ścieżce (waypoints), przyjmuje
// obrażenia różnych typów i może być spowolniona lub podpalona.
// Konkretne warianty (np. FastZombie) dziedziczą po tej klasie.
class Zombie: public GameObject
{
protected:
    int hp;                                  // aktualne punkty życia
    int maxHp;                               // maksymalna pula HP (do paska zdrowia)
    int reward;                              // ile waluty daje przy śmierci
    float baseSpeed;                         // bazowa prędkość (px/s)
    float currentSpeed;                      // aktualna (modyfikowana przez Slower)
    const std::vector<sf::Vector2f>& waypoints; // ścieżka przez całą mapę
    int currentWaypointIndex;                // do którego waypointa zmierza

    // Efekty - przydadzą się w MS3
    float slowTimer;
    float burnTimer;
    int burnDamage;

    // Reprezentacja graficzna
    sf::CircleShape shape;
public:
    // Inicjalizuje zombie przypisując mu ścieżkę; wartości HP, speed itp.
    // ustawiane są przez konstruktory klas pochodnych.
    Zombie(const std::vector<sf::Vector2f>& path);

    // Przesuwa zombie wzdłuż ścieżki i aplikuje aktywne efekty (slow, burn).
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    // Rysuje kształt zombie oraz pasek HP na podanym oknie.
    void render(sf::RenderWindow& window) override;

    // Zadaje obrażenia z uwzględnieniem typu — klasy pochodne mogą nadpisać
    // by dodać odporności (np. ArmoredZombie ignoruje BULLET).
    virtual void takeDamage(int dmg, DamageType type);

    // Spowalnia zombie na podany czas; nie stackuje się — resetuje timer.
    void applySlow(float duration);

    // Podpala zombie: zadaje dps obrażeń na sekundę przez duration sekund.
    void applyBurn(int dps, float duration);

    int getReward() const { return reward; }
    bool reachedEnd() const;   // czy zombie dotarł do bazy

};

//=== WALKER ===
class Walker : public Zombie
{
public:
    // Konstruktor przyjmujący pozycję startową
    //Walker(sf::Vector2f startPosition);
    // droga
    Walker(const std::vector<sf::Vector2f>& waypoints);
};


#endif // ZOMBIE_H
