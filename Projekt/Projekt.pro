TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/bullet.cpp \
        src/machineguntower.cpp \
        src/main.cpp \
        src/Game.cpp \
        src/Gameobject.cpp \
        src/map.cpp \
        src/player.cpp \
        src/projectile.cpp \
        src/runner.cpp \
        src/tower.cpp \
        src/zombie.cpp \
        src/walker.cpp

HEADERS += \
    src/Config.h \
    src/Game.h \
    src/Gameobject.h \
    src/Random.h \
    src/bullet.h \
    src/machineguntower.h \
    src/map.h \
    src/player.h \
    src/projectile.h \
    src/runner.h \
    src/tower.h \
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
