# DEAD ZONE

Gra typu **tower defense** w klimacie post-apokaliptycznym. Gracz buduje wieżyczki
obronne wzdłuż ścieżki, którą nadciągają fale zombie, i stara się nie dopuścić ich
do bazy. Projekt zaliczeniowy z programowania obiektowego w C++ / SFML.

## Status projektu

Projekt **ukończony** — rozgrywka jest kompletna i w pełni grywalna.
Zaimplementowane:

- pełna mechanika (ruch zombie po waypointach, budowa/ulepszanie/sprzedaż wież, kolizje, fale),
- 4 typy zombie i 5 typów wież, każdy z własnym zachowaniem,
- 5 map do wyboru, system fal wczytywany z plików,
- bonusy (amunicja / apteczka / EMP), animacje zombie i wież teksturowych,
- menu, pauza, ekran końca gry z zapisem wyniku oraz tablica najlepszych wyników (top 10).
- pełne udźwiękowienie rozgrywki, w tym muzyka w tle oraz immersyjne efekty dźwiękowe dla walki i interfejsu.

## Autorzy

- Natalia Ćwiek (Natalia2006420)
- Wiktor Czapliński (Wiktor-put)

## Technologia

- **Język:** C++17
- **Biblioteka graficzna:** SFML 2.6.1 (niekompatybilne z 3.x)
- **Build system:** qmake (.pro)
- **IDE referencyjne:** Qt Creator z MinGW

## Rozgrywka

- Zombie ruszają z jednego końca mapy i podążają ścieżką do bazy gracza.
- Każdy zombie, który dojdzie do bazy, odbiera graczowi życia — po ich utracie następuje koniec gry.
- Za zabite zombie gracz dostaje walutę, którą wydaje na nowe wieże oraz ich ulepszenia.
- Falę uruchamia się ręcznie klawiszem **SPACJA**, co pozwala przygotować obronę.

### Typy zombie

| Typ | Charakterystyka |
|-----|-----------------|
| Walker  | podstawowy, niska prędkość i HP |
| Runner  | bardzo szybki, ale słabo opancerzony |
| Armored | powolny, pancerz redukuje obrażenia od kul o 50% |
| Tank    | ogromna ilość HP, bardzo wolny |

### Typy wież

| Wieża | Działanie |
|-------|-----------|
| Karabin maszynowy | szybki ostrzał, niskie obrażenia jednostkowe |
| Snajper           | ogromny zasięg i obrażenia, wolny ostrzał |
| Wyrzutnia rakiet  | obrażenia obszarowe (AoE) |
| Miotacz ognia     | podpala zombie (obrażenia w czasie) |
| Spowalniacz       | spowalnia trafione zombie |

### Bonusy

Co jakiś czas nad strefą zrzutu spada bonus, który gracz może kliknąć:
amunicja (dodatkowa waluta), apteczka (życie) oraz EMP (chwilowe ogłuszenie wszystkich zombie).

## Sterowanie

- **Lewy przycisk myszy** — wybór slotu / przycisku w menu i HUD
- **Klawisze 1–5** — budowa wieży w zaznaczonym slocie
- **U / S** — ulepszenie / sprzedaż wieży w zaznaczonym slocie
- **Prawy przycisk myszy / ESC** — odznaczenie slotu
- **ESC** (gdy slot nie jest zaznaczony) — pauza
- **SPACJA** — uruchomienie kolejnej fali
- **Strzałki + ENTER** — nawigacja po ekranach menu

## Wymagania

- Kompilator z obsługą C++17 (GCC / MinGW, Clang lub MSVC)
- Qt Creator z zainstalowanym MinGW lub MSVC
- SFML 2.5+ (NIE 3.x)

## Budowanie (Qt Creator)

1. Otwórz plik `Projekt/Projekt.pro` w Qt Creator.
2. Skonfiguruj kit z obsługą C++17 i biblioteką SFML 2.x.
3. W zakładce *Projects → Run → Working directory* ustaw katalog projektu (`$$PWD`),
   aby gra znalazła folder `assets/`.
4. Zbuduj i uruchom (`Ctrl+R`).

## Struktura projektu

```
Projekt/
├── Projekt.pro          # plik qmake (HEADERS / SOURCES / INCLUDEPATH)
├── src/
│   ├── core/            # pętla gry, mapa, gracz, fale, HUD, zasoby, konfiguracja
│   ├── entities/        # klasa bazowa GameObject oraz bonusy
│   ├── zombies/         # bazowa klasa Zombie i jej warianty
│   ├── towers/          # bazowa klasa Tower i 5 typów wież
│   └── projectiles/     # bazowa klasa Projectile i typy pocisków
└── assets/
    ├── maps/            # definicje map (waypointy, sloty, drop zone, tło)
    ├── waves/           # definicje fal
    ├── fonts/           # czcionka interfejsu
    └── textures/        # grafiki teł, ścieżki, zombie, wież i pocisków
```
