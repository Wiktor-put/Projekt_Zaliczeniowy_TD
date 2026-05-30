#ifndef TOWER_H
#define TOWER_H

// tower.h — abstrakcyjna klasa bazowa dla wszystkich typów wież

#include <memory>
#include <cmath>
#include "Gameobject.h"
#include "zombie.h"

// Wspólna logika wieży: szukanie celu, obrót, cooldown strzału i ulepszenia.
// Konkretne typy (MachineGunTower, SniperTower itp.) nadpisują shoot().
class Tower: public GameObject
{
protected:
    float range;          // zasięg wykrywania zombie (px)
    float fireRate;       // liczba strzałów na sekundę
    float cooldown;       // pozostały czas do następnego strzału (s)
    int damage;           // obrażenia na strzał
    int cost;             // koszt zakupu w walucie gry
    int level;            // poziom ulepszenia (1 = bazowy)
    Zombie* currentTarget; // aktualnie śledzony cel (nullptr = brak)

    sf::RectangleShape shape;   // placeholder graficzny

public:
    // Ustawia pozycję wieży; wartości zasięgu/damage inicjalizuje klasa pochodna.
    Tower(sf::Vector2f pos);

    // Aktualizuje cooldown, obraca wieżę w stronę celu i strzela gdy gotowa.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    // Tworzy pocisk i dodaje go do listy obiektów; nadpisywany przez podklasy.
    virtual void shoot(std::vector<std::unique_ptr<GameObject>>& objects) = 0;

    // Rysuje kształt wieży i wskaźnik zasięgu.
    void render(sf::RenderWindow& window) override;

    // Przeszukuje listę obiektów i wybiera najbliższego żywego zombie w zasięgu.
    void findTarget(const std::vector<std::unique_ptr<GameObject>>& objects);

    // Zwraca true jeśli currentTarget nadal istnieje i jest w zasięgu.
    bool isTargetValid() const;

    // Zwiększa poziom wieży; skaluje damage i range wg Config.
    void upgrade();

    // Obraca wieżę płynnie w kierunku targetPos z ograniczoną prędkością kątową.
    void rotateToward(sf::Vector2f targetPos, float dt);
};

#endif // TOWER_H
