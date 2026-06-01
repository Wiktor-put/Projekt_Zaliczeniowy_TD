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
    float range     = 0.f;         // zasięg wykrywania zombie (px)
    float fireRate  = 1.f;         // liczba strzałów na sekundę
    float cooldown  = 0.f;         // pozostały czas do następnego strzału (s)
    int damage      = 0;           // obrażenia na strzał
    int cost        = 0;           // koszt zakupu w walucie gry
    int level       = 1;           // poziom ulepszenia (1 = bazowy)
    Zombie* currentTarget = nullptr; // aktualnie śledzony cel (nullptr = brak)

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

    // Zeruje currentTarget jeśli cel już nie żyje — wywołać przed usunięciem martwych obiektów.
    // Zapobiega dangling pointer gdy unique_ptr zombi zostaje zwolniony.
    void clearDeadTarget();
};

#endif // TOWER_H
