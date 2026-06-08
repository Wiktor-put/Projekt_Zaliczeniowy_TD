# Projekt_Zaliczeniowy_TD

# DEAD ZONE

Gra tower defense w klimacie post-apokaliptycznym, w której gracz broni bazy
przed nadciągającymi falami zombie. Projekt zaliczeniowy z C++ / SFML.

## Status projektu

W trakcie realizacji — obecnie zrealizowane:
-**Milestone 1** (fundament)
-**Milestone 2** (grywalny prototyp)
-**Milestone 4** (game feel)

## Autorzy

- Natalia Ćwiek (Natalia2006420)
- Wiktor Czapliński (Wiktor-put)

## Technologia

- **Język:** C++17
- **Biblioteka graficzna:** SFML 2.6.1 (niekompatybilne z 3.x)
- **Build system:** qmake (.pro)
- **IDE referencyjne:** Qt Creator

## Wymagania

- Kompilator z obsługą C++17 (GCC, Clang lub MSVC)
- Qt Creator z zainstalowanym MinGW lub MSVC
- SFML 2.5+ (NIE 3.x)

## Budowanie

### Qt Creator

1. Otwórz plik `Projekt.pro` w Qt Creator
2. Skonfiguruj kit z C++17
3. W zakładce *Projects → Run → Working directory* ustaw `$$PWD`
4. Zbuduj (`Ctrl+R`)
