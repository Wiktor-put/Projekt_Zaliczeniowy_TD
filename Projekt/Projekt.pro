TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/bonus.cpp \
        src/bullet.cpp \
        src/flame.cpp \
        src/flamethrowertower.cpp \
        src/machineguntower.cpp \
        src/main.cpp \
        src/Game.cpp \
        src/Gameobject.cpp \
        src/map.cpp \
        src/player.cpp \
        src/projectile.cpp \
        src/rocket.cpp \
        src/rockettower.cpp \
        src/runner.cpp \
        src/slowertower.cpp \
        src/snipertower.cpp \
        src/snowball.cpp \
        src/tower.cpp \
        src/wavemanager.cpp \
        src/zombie.cpp \
        src/walker.cpp

HEADERS += \
    src/Config.h \
    src/Game.h \
    src/Gameobject.h \
    src/Random.h \
    src/bonus.h \
    src/bullet.h \
    src/flame.h \
    src/flamethrowertower.h \
    src/machineguntower.h \
    src/map.h \
    src/player.h \
    src/projectile.h \
    src/rocket.h \
    src/rockettower.h \
    src/runner.h \
    src/slowertower.h \
    src/snipertower.h \
    src/snowball.h \
    src/tower.h \
    src/wavemanager.h \
    src/zombie.h \
    src/walker.h

# Ścieżka do folderu 'include' pobranego SFML
INCLUDEPATH += "C:/SFML/SFML-2.6.1/include"

# Ścieżka do folderu 'lib' oraz dołączenie modułów SFML
LIBS += -L"C:/SFML/SFML-2.6.1/lib" \
        -lsfml-graphics \
        -lsfml-window \
        -lsfml-system \
        -lsfml-audio \
        -lsfml-network

# Ścieżka do katalogu projektu przekazywana jako makro do kodu (używana przez map.loadFromFile)
DEFINES += ASSETS_DIR=\\\"$$PWD\\\"
