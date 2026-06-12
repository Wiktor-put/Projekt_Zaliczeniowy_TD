#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <list>

class AudioManager {
private:
    static std::map<std::string, sf::SoundBuffer> buffers;
    static std::list<sf::Sound> sounds; // Używamy listy, by łatwo usuwać skończone dźwięki
    static sf::Music bgMusic;           // Muzyka w tle jest "streamowana" (nie obciąża RAMu)

public:
    static void playMusic(const std::string& path, float volume = 30.f);
    static void playSound(const std::string& path, float volume = 100.f, float pitch = 1.0f);
    static void update(); // Usuwa z pamięci dźwięki, które skończyły grać
};

#endif // AUDIOMANAGER_H