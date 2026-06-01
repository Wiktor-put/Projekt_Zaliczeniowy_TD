#ifndef PLAYER_H
#define PLAYER_H

// player.h — stan gracza: życia, waluta, wynik
// Przechowuje zasoby gracza potrzebne do kupowania wież i śledzenia porażki.
// Życia maleją gdy zombie dotrze do bazy; waluta rośnie po zabiciu zombie.

class Player
{
private:
    int lives;      // pozostałe życia; 0 = koniec gry
    int money;      // aktualna waluta do kupowania i ulepszania wież
    int score = 0;  // wynik (suma punktów za zabite zombi)

public:
    Player();

    // Zwraca true gdy gracz nadal ma życia. Bazuje wyłącznie na lives > 0,
    // dzięki czemu nie ma duplikatu stanu (jedno źródło prawdy).
    bool isAlive() const { return lives > 0; }

    // Sprawdza czy gracz może wydać daną kwotę. Nie modyfikuje stanu (const),
    // więc można używać do podglądu zanim faktycznie odejmiemy pieniądze.
    bool enoughMoney(int cost) const { return money >= cost; }

    // Próbuje wydać cost. Zwraca true jeśli się udało, false gdy zabrakło środków.
    // Wywołujący musi sprawdzić zwracaną wartość przed założeniem że zakup się powiódł.
    bool spendMoney(int cost);

    void addMoney(int amount);

    // Odejmuje życia gracza, klamruje wynik do zera (nigdy nie zejdzie poniżej).
    // Detekcja przegranej dzieje się w Game przez sprawdzenie isAlive().
    void lostLives(int amount);

    void addScore(int points) { score += points; }

    // Przywraca pola do wartości startowych z Config.
    // Wywoływane przez Game::startNewGame() przy nowej grze i po GAME_OVER -> R.
    void reset();

    int getLives() const { return lives; }
    int getMoney() const { return money; }
    int getScore() const { return score; }
    // za bonusy dostajemy zycie
    void addLives(int amount) { lives += amount; }
};

#endif // PLAYER_H
