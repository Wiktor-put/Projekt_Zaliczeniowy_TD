TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
            src/core/main.cpp \
            src/core/Game.cpp \
            src/core/map.cpp \
            src/core/resourcemanager.cpp \
            src/core/wavemanager.cpp \
            src/core/player.cpp \
            src/entities/Gameobject.cpp \
            src/entities/bonus.cpp \
            src/zombies/zombie.cpp \
            src/zombies/walker.cpp \
            src/zombies/runner.cpp \
            src/zombies/armored.cpp \
            src/zombies/tank.cpp \
            src/towers/tower.cpp \
            src/towers/machineguntower.cpp \
            src/towers/snipertower.cpp \
            src/towers/rockettower.cpp \
            src/towers/flamethrowertower.cpp \
            src/towers/slowertower.cpp \
            src/projectiles/projectile.cpp \
            src/projectiles/bullet.cpp \
            src/projectiles/rocket.cpp \
            src/projectiles/flame.cpp \
            src/projectiles/snowball.cpp

HEADERS += \
            src/core/Game.h \
            src/core/map.h \
            src/core/resourcemanager.h \
            src/core/wavemanager.h \
            src/core/Config.h \
            src/core/Random.h \
            src/core/player.h \
            src/entities/Gameobject.h \
            src/entities/bonus.h \
            src/zombies/zombie.h \
            src/zombies/walker.h \
            src/zombies/runner.h \
            src/zombies/armored.h \
            src/zombies/tank.h \
            src/towers/tower.h \
            src/towers/machineguntower.h \
            src/towers/snipertower.h \
            src/towers/rockettower.h \
            src/towers/flamethrowertower.h \
            src/towers/slowertower.h \
            src/projectiles/projectile.h \
            src/projectiles/bullet.h \
            src/projectiles/rocket.h \
            src/projectiles/flame.h \
            src/projectiles/snowball.h


# Ścieżka do folderu 'include' pobranego SFML
INCLUDEPATH += "C:/SFML/SFML-2.6.1/include"
INCLUDEPATH += src/core src/entities src/zombies src/towers src/projectiles

# Ścieżka do folderu 'lib' oraz dołączenie modułów SFML
LIBS += -L"C:/SFML/SFML-2.6.1/lib" \
        -lsfml-graphics \
        -lsfml-window \
        -lsfml-system \
        -lsfml-audio \
        -lsfml-network

# Ścieżka do katalogu projektu przekazywana jako makro do kodu (używana przez map.loadFromFile)
DEFINES += ASSETS_DIR=\\\"$$PWD\\\"
