#ifndef PLAYER_H
#define PLAYER_H

// player.h — stan gracza: życia i waluta

// Przechowuje zasoby gracza potrzebne do kupowania wież i śledzenia porażki.
// Życia maleją gdy zombie dotrze do bazy; waluta rośnie po zabiciu zombie.
class Player
{
private:
    int lives; // pozostałe życia; 0 = koniec gry
    int money; // aktualna waluta do kupowania i ulepszania wież
public:
    Player();
};

#endif // PLAYER_H
