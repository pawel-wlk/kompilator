# Kompilator

by Paweł Wilkosz

# Zależności
- bison 3.5
- flex 2.6.4
- gcc 9.2.0

# Instalacja
```
make
```

# Sposób użycia
```
./kompilator <plik wejsciowy> <plik wyjsciowy>
```
W przypadku gdy nie podano pliku wyjściowego, wynik kompilacji zapisywany jest w pliku `out.mr`.

# Pliki
1. `parser.ypp` - główny plik programu, zawiera gramatykę oraz obsługę wejścia/wyjścia kompilatora.
1. `lekser.l` - kod leksera.
1. `Memory.hpp` i `Memory.cpp`- klasa reprezentująca tablicę symboli.
1. `Value.hpp` - klasa abstrakcyjna obejmująca zmienne i literale liczb.
1. `Variable.hpp` i `Variable.cpp` - klasa reprezentująca zmienne.
1. `Constant.hpp` i `Constant.cpp` - klasa reprezentująca literale liczb.
1. `Operation.hpp` i `Operation.cpp` - struktura reprezentująca operację w kodzie wynikowym.
1. `labels.hpp` i `labels.cpp` - struktury reprezentujące instrukcje warunkowe i pętle.
1. `Code.hpp` i `Code.cpp` - główna klasa zajmująca się generowaniem kodu wynikowego.
