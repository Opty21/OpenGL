# Adam Borawski

## Sokoban 3D
Sokoban (倉庫番 Sōkoban) – komputerowa gra logiczna, w której celem jest odpowiednie ustawienie skrzyń.
Poziomy wczytywane są z plików ".sok" stworzonych przez społeczność.
### Sterowanie

#### W S - ruch przód/tył
#### Mysz - sterowanie kamerą
#### E - Pchanie skrzyń
#### R - Restart poziomu
#### N - !!!CHEAT CODE!!! przejście na kolejny poziom (po wciśnięciu przycisku trzeba przesunąć jakąkolwiek skrzynię)

#### ESC - wyjście z gry

#### Menu pod prawym przyskiem myszy. Trzeba kliknąć klawisz 'P' aby odblokować kursor.
### Prezentacja gry
![](video.mp4)

### Główne założenia:
1. postać - obiekt, którym możemy wykonywać jakieś czynności
2. świat - scena złożona m.in. z obiektów, które oddziałują z postacią
3. problem/funkcjonalność/cel 

### Scena:
- [x] siatka powierzchni (podłoże) z implementacją prostej fizyki poruszania się po podłożu
- [x] obiekty statyczne i ruchome (np. obracające się)
- [x] wiele obiektów wygenerowanych metodą instanced rendering (np. trawa)
- [x] obiekty kolizyjne z naszą postacią (kamerą) i obiekty niekolizyjne
- [x] obiekty dobre i złe (zwiększające i zmniejszające wartość funkcji celu gry)
- [x] nasza postać, która porusza się po scenie, widok pierwszo-osobowy/trzecio-osobowy

### Rendering:
- [x] obsługa plików .OBJ
- [x] tekstury i tekstury przezroczyste (np. kwiaty)
- [x] implementacja materiałów (błyszczące, matowe itp.)
- [ ] implementacja oświetlenia kierunkowego (np. słońce) i punktowego (np. latarnia)
- [ ] implementacja cieni w oświetleniu kierunkowym/punktowym
- [x] tekst na ekranie (informacje o punktach, czasie trwania gry itp.)
- [x] rendering pozaekranowy (np. minimapa, lustro)
- [x] implementacja skyboxa
- [x] implementacja environmental mappingu (np. odbicie, refrakcja)
- [ ] wykorzystanie shadera geometrii

### Inne:
- [x] menu kontekstowe lub inna forma zmiany opcji aplikacji
- [x] zmiana trybu rozdzielczości (np. fullscreen)
- [ ] zmiana ilości obiektów generowanych losowo
- [ ] zmiana wielkości mapy cieni i innych parametrów aplikacji
