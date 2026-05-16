#include "map.h"

void Map::loadFromFile(const std::string& path){
    std::string pathWayPoint = path + "/Waypoint.txt";
    std::ifstream odczyt(pathWayPoint, std::ios::in);
    if(odczyt.is_open())
    {

    }
    else
        std::cout<<"Nie udało się otworzyć pliku";

}

Map::Map() {}
