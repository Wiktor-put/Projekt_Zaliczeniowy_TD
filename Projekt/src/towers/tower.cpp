#include "tower.h"

Tower::Tower(sf::Vector2f pos) {
    position = pos;
    // === KORPUS WIEŻY (kwadrat 40x40) ===
    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setOrigin(20.f, 20.f);           // środek w pozycji wieży (nie róg)
    shape.setFillColor(sf::Color(80, 80, 100));  // szaroniebieski
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setPosition(position);

}


void Tower::initSprite(const sf::Texture* lvl1, const sf::Texture* lvl2, const sf::Texture* lvl3,
                       float scale, float forwardOffsetDeg) {
    levelTextures[0] = lvl1;
    levelTextures[1] = lvl2;
    levelTextures[2] = lvl3;
    textureForwardOffset = forwardOffsetDeg;
    useSprite = true;

    sprite.setScale(scale, scale);
    sprite.setPosition(position);
    updateSpriteForLevel();
}

void Tower::updateSpriteForLevel() {
    int idx = level - 1;            // poziom 1 -> indeks 0
    if (idx < 0) idx = 0;
    if (idx > 2) idx = 2;

    const sf::Texture* tex = levelTextures[idx];
    if (!tex) return;

    sprite.setTexture(*tex, true);  // true = zresetuj prostokąt tekstury do pełnej grafiki
    // Origin na środku grafiki — żeby obrót następował wokół środka wieży.
    sprite.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
}

void Tower::render(sf::RenderWindow& window){
    if (useSprite) {
        sprite.setPosition(position);
        // Obracamy sprite w stronę celu; offset koryguje domyślną orientację tekstury.
        sprite.setRotation(rotation + textureForwardOffset);
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
}

void Tower::rotateToward(sf::Vector2f targetPos, float dt){
    sf::Vector2f diff = targetPos - position;
    float targetAngle = std::atan2(diff.y, diff.x) * 180.f / 3.14159f;

    float diff_angle = targetAngle - rotation;
    while (diff_angle > 180.f) diff_angle -= 360.f;
    while (diff_angle < -180.f) diff_angle += 360.f;

    // obrót z ograniczoną prędkością
    float maxRotation = rotationSpeed * dt;
    if (std::abs(diff_angle) <= maxRotation) {
        rotation = targetAngle;
    } else {
        rotation += (diff_angle > 0.f ? 1.f : -1.f) * maxRotation;
    }

}

void Tower::findTarget(const std::vector<std::unique_ptr<GameObject>>& objects){
    currentTarget = nullptr;
    float closestDistSq = range * range;
    for(const auto& obj: objects){
        Zombie* zombie = dynamic_cast<Zombie*>(obj.get());
        if (!zombie || !zombie->isAlive()) continue;
        sf::Vector2f diff = zombie->getPosition() - position;
        float distSq = diff.x * diff.x + diff.y * diff.y;
        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            currentTarget = zombie;
        }
    }
}

bool Tower::isTargetValid() const{
    if(!currentTarget) return false;
    if(!currentTarget->isAlive()) return false;

    sf::Vector2f diff = currentTarget->getPosition() - position;
    float distSq = diff.x * diff.x + diff.y * diff.y;

    return distSq <= range * range;
}




void Tower::clearDeadTarget() {
    if (currentTarget && !currentTarget->isAlive())
        currentTarget = nullptr;
}

void Tower::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects){
    if(!isTargetValid()){
        findTarget(objects);
    }
    if (currentTarget) {
        rotateToward(currentTarget->getPosition(), dt);

        cooldown -= dt;
        if (cooldown <= 0.f) {
            shoot(objects);
            cooldown = 1.f / fireRate;
        }
    }

}

void Tower::upgrade() {
    if (level >= 3) return; // Maksymalnie 3 poziom

    level++;

    // Mnożymy statystyki przez wartości z Configu
    damage = static_cast<int>(damage * Config::UPGRADE_DAMAGE_MULTIPLIER);
    range *= Config::UPGRADE_RANGE_MULTIPLIER;

    // Aktualizujemy całkowitą wartość wieży (przyda się do zwrotu kasy przy sprzedaży)
    cost = static_cast<int>(cost * Config::UPGRADE_COST_MULTIPLIER);

    // Wieże teksturowe zmieniają grafikę na odpowiadającą nowemu poziomowi.
    if (useSprite) {
        updateSpriteForLevel();
        return;
    }

    // Placeholder: wieża robi się odrobinę ciemniejsza po ulepszeniu
    sf::Color currentColor = shape.getFillColor();
    shape.setFillColor(sf::Color(
        std::max(0, currentColor.r - 20),
        std::max(0, currentColor.g - 20),
        std::max(0, currentColor.b - 20)
        ));
}
