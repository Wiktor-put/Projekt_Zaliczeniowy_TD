#include "audiomanager.h"

#ifndef ASSETS_DIR
#define ASSETS_DIR "."
#endif

std::map<std::string, sf::SoundBuffer> AudioManager::buffers;
std::list<sf::Sound> AudioManager::sounds;
sf::Music AudioManager::bgMusic;

void AudioManager::playMusic(const std::string& path, float volume) {
    if (bgMusic.openFromFile(std::string(ASSETS_DIR) + path)) {
        bgMusic.setLoop(true);
        bgMusic.setVolume(volume);
        bgMusic.play();
    }
}

void AudioManager::playSound(const std::string& path, float volume) {
    // 1. Sprawdzamy czy ten dźwięk był już kiedyś wczytany
    if (buffers.find(path) == buffers.end()) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(std::string(ASSETS_DIR) + path)) {
            buffers[path] = buffer;
        } else {
            return;  // nie udało się wczytać dźwięku — pomijamy odtwarzanie
        }
    }

    // 2. Dodajemy "Odtwarzacz" na koniec listy i puszczamy dźwięk
    sounds.emplace_back(buffers[path]);
    sounds.back().setVolume(volume);
    sounds.back().play();
}

void AudioManager::update() {
    // 3. Usuwamy z listy te dźwięki, które całkowicie zamilkły
    sounds.remove_if([](const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}