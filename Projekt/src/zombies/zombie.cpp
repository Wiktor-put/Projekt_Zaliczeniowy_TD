#include "zombie.h"
#include "resourcemanager.h"
#include "audiomanager.h"
#include "Config.h"
#include <cmath>

Zombie::Zombie(const std::vector<sf::Vector2f>& waypoints)
    : path(waypoints), currentWaypointIndex(1)
{
}

void Zombie::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;  // Zombie nie potrzebuje kontenera w update

    // Obsługa spowolnienia
    if (slowTimer > 0.f) {
        slowTimer -= dt;
        if (slowTimer <= 0.f) {
            currentSpeed = baseSpeed; // Koniec spowolnienia, powrót do normy
        }
    }

    // Obsługa podpalenia (otrzymywanie obrażeń w czasie)
    if (burnTimer > 0.f) {
        burnTimer -= dt;
        burnAccumulator += burnDamage * dt; // Zbieramy ułamkowe obrażenia (np. 15 HP na sekundę)

        if (burnAccumulator >= 1.f) {
            int dmgToTake = static_cast<int>(burnAccumulator);
            takeDamage(dmgToTake, DamageType::FIRE); // Odbieramy punkty życia
            burnAccumulator -= dmgToTake;
        }
    }

    if (stunTimer > 0.f) {
        stunTimer -= dt;
        return; // Zombiak jest rażony prądem EMP - NIE ROBI NIC, nie porusza się
    }

    if (!reachedEnd()) {
        sf::Vector2f target = path[currentWaypointIndex];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 2.f) {
            sf::Vector2f normalizedDir = direction / distance;
            velocity = normalizedDir * currentSpeed;
            position += velocity * dt;
        } else {
            currentWaypointIndex++;
        }
    }
    else destroy();

    // Wspólna animacja kierunkowa + podpięcie sprite'a pod pozycję obiektu.
    updateAnimation(dt);
    sprite.setPosition(position + spriteOffset);
}

void Zombie::render(sf::RenderWindow& window) {
    // Rysujemy model tylko gdy klasa pochodna ustawiła tekstury przez initSprite().
    if (texDown) window.draw(sprite);

    // Pasek HP
    float ratio = (maxHp > 0) ? static_cast<float>(hp) / static_cast<float>(maxHp) : 0.f;
    sf::RectangleShape bgBar(sf::Vector2f(30.f, 4.f));
    bgBar.setFillColor(sf::Color::Red);
    bgBar.setPosition(position - sf::Vector2f(15.f, 20.f));
    window.draw(bgBar);

    sf::RectangleShape hpBar(sf::Vector2f(30.f * ratio, 4.f));
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setPosition(position - sf::Vector2f(15.f, 20.f));
    window.draw(hpBar);
}

void Zombie::takeDamage(int dmg, DamageType type) {
    if (!alive) return;
    (void)type;
    hp -= dmg;
    if (hp <= 0){
        destroy();
        AudioManager::playSound(Config::Assets::SOUND_ZOMBIE_DEATH, 15.f);
    }
}

void Zombie::applySlow(float duration) {
    slowTimer = duration;
    currentSpeed = baseSpeed * 0.5f;
}

void Zombie::applyBurn(int dps, float duration) {
    burnTimer = duration;
    burnDamage = dps;
}

bool Zombie::reachedEnd() const {
    return currentWaypointIndex >= static_cast<int>(path.size());
}

void Zombie::initSprite(const char* down, const char* up, const char* right, const char* left,
                        int frames, float frameDur, float scale) {
    // Wszystkie tekstury przechodzą przez ResourceManager (jedna kopia w pamięci).
    texDown  = &ResourceManager::getTexture(down);
    texUp    = &ResourceManager::getTexture(up);
    texRight = &ResourceManager::getTexture(right);
    texLeft  = &ResourceManager::getTexture(left);

    totalFrames   = frames;
    frameDuration = frameDur;

    // Startujemy od pierwszej klatki paska "w prawo".
    sprite.setTexture(*texRight);
    int fW = static_cast<int>(texRight->getSize().x) / totalFrames;
    int fH = static_cast<int>(texRight->getSize().y);
    sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
    sprite.setOrigin(fW / 2.f, fH / 2.f);
    sprite.setScale(scale, scale);
    sprite.setPosition(position);
}

void Zombie::updateAnimation(float dt) {
    if (!texDown) return;        // klasa pochodna nie ustawiła tekstur
    if (reachedEnd()) return;    // zombie dotarło do bazy — nie animujemy

    // Wybór paska wg dominującej składowej prędkości.
    sf::Texture* dir;
    if (std::abs(velocity.x) > std::abs(velocity.y))
        dir = (velocity.x > 0.f) ? texRight : texLeft;
    else
        dir = (velocity.y > 0.f) ? texDown : texUp;

    sprite.setTexture(*dir);

    animationTimer += dt;
    if (animationTimer >= frameDuration) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % totalFrames;
    }

    // Rect liczymy z AKTUALNEJ tekstury — paski "bok" i "przód" mogą mieć różną
    // szerokość klatki, więc nie wolno tu cache'ować rozmiaru z innego kierunku.
    int fW = static_cast<int>(dir->getSize().x) / totalFrames;
    int fH = static_cast<int>(dir->getSize().y);
    sprite.setTextureRect(sf::IntRect(currentFrame * fW, 0, fW, fH));
}
