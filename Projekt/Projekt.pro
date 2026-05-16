TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        scr/Game.cpp \
        scr/Gameobject.cpp \
        scr/map.cpp \
        scr/player.cpp \
        scr/projectile.cpp \
        scr/tower.cpp \
        scr/zombie.cpp

HEADERS += \
    scr/Game.h \
    scr/Gameobject.h \
    scr/map.h \
    scr/player.h \
    scr/projectile.h \
    scr/tower.h \
    scr/zombie.h
    # Ścieżka do folderu 'include' pobranego SFML
    INCLUDEPATH += "C:/SFML/SFML-2.6.1/include"

    # Ścieżka do folderu 'lib' oraz dołączenie modułów SFML
    LIBS += -L"C:/SFML/SFML-2.6.1/lib" \
            -lsfml-graphics \
            -lsfml-window \
            -lsfml-system \
            -lsfml-audio \
            -lsfml-network
