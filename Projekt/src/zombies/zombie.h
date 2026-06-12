#ifndef ZOMBIE_H
#define ZOMBIE_H

// zombie.h — abstrakcyjna klasa bazowa dla wszystkich typów zombie

#include "Gameobject.h"
#include "Config.h"
#include <vector>

enum class DamageType {
    BULLET,
    EXPLOSION,
    FIRE,
    SNOWBALL
};

// Klasa bazowa zombie: porusza się po wyznaczonej ścieżce (waypoints), przyjmuje
// obrażenia różnych typów i może być spowolniona lub podpalona.
class Zombie : public GameObject
{
protected:
    int hp;                                   // bieżące punkty życia
    int maxHp;                                // maksymalne HP (do paska życia)
    int reward;                               // waluta przyznawana graczowi za zabicie
    float baseSpeed;                          // bazowa prędkość ruchu (px/s)
    float currentSpeed;                       // aktualna prędkość (może być zmniejszona przez slow)
    const std::vector<sf::Vector2f>& path;    // trasa zombie (waypointy mapy)
    int currentWaypointIndex = 1;             // indeks waypointa, do którego zombie zmierza
    int lifeCost;                             // ile żyć traci gracz, gdy zombie dojdzie do bazy

    // Efekty czasowe nakładane przez pociski (spowolnienie, podpalenie, ogłuszenie)
    float slowTimer = 0.f;        // pozostały czas spowolnienia (s)
    int snowDamage = 0;           // obrażenia kuli śnieżnej (niewykorzystywane do DOT, pomocnicze)
    float burnTimer = 0.f;        // pozostały czas podpalenia (s)
    int burnDamage = 0;           // obrażenia podpalenia na sekundę
    float burnAccumulator = 0.f;  // akumulator czasu, by zadawać obrażenia ognia co 1 s
    float stunTimer = 0.f;        // pozostały czas ogłuszenia (EMP) — zombie stoi

    // --- WSPÓLNY SYSTEM SPRITE'ÓW I ANIMACJI (dla wszystkich typów zombie) ---
    // Cztery kierunkowe paski klatek. To obserwatorzy — właścicielem tekstur
    // jest ResourceManager, więc trzymamy surowe wskaźniki, nie kopie.
    sf::Texture* texDown  = nullptr;
    sf::Texture* texUp    = nullptr;
    sf::Texture* texRight = nullptr;
    sf::Texture* texLeft  = nullptr;
    sf::Sprite sprite;                    // rysowany model zombie
    int currentFrame = 0;                 // bieżąca klatka w pasku
    int totalFrames = 6;                  // liczba klatek w pasku (zależna od grafiki)
    float animationTimer = 0.f;           // odmierza czas do zmiany klatki
    float frameDuration = 0.1f;           // czas wyświetlania jednej klatki (s)
    sf::Vector2f spriteOffset{0.f, 0.f};  // korekta wizualna pozycji sprite'a (np. Tank)

    // Wczytuje 4 paski animacji przez ResourceManager i konfiguruje sprite'a.
    // Wołane z konstruktorów klas pochodnych — eliminuje duplikację kodu tekstur.
    void initSprite(const char* down, const char* up, const char* right, const char* left,
                    int frames, float frameDur, float scale);

    // Wybiera pasek wg kierunku ruchu i przesuwa animację o dt. Wołane z update().
    void updateAnimation(float dt);

public:
    Zombie(const std::vector<sf::Vector2f>& path);

    // Wspólna logika ruchu + animacji. Klasy pochodne zwykle NIE muszą nadpisywać.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    // Rysuje sprite (jeśli ustawiony) oraz pasek HP. Wspólne dla wszystkich zombie.
    void render(sf::RenderWindow& window) override;

    // klasy pochodne (np. ArmoredZombie) mogą nadpisać.
    virtual void takeDamage(int dmg, DamageType type);

    // Spowalnia zombie na podany czas; nie stackuje się — resetuje timer.
    void applySlow(float duration);

    // Podpala zombie: zadaje dps obrażeń na sekundę przez duration sekund.
    void applyBurn(int dps, float duration);

    // Ogłusza zombie na podany czas (bonus EMP) — przez ten czas stoi w miejscu.
    void applyStun(float duration) { stunTimer = duration; }

    int getReward() const { return reward; }
    int getLifeCost() const {return lifeCost;}
    // Zwraca true, gdy zombie dotarło do ostatniego waypointa (bazy gracza).
    bool reachedEnd() const;
};

#endif // ZOMBIE_H
