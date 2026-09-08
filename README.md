# Plants vs Zombies - Progetto di Computer Grafica

Alessandro Bozzini - 6400176

---

## Requisiti

*   **Compilatore:** C++17 (g++ su Linux/Mac, MinGW o MSVC su Windows)
*   **Libreria:** SFML 3.0 (Moduli: Graphics, Window, System)
*   **Build System:** CMake (>= 3.16)

---

## Come compilare

Dalla cartella principale del progetto (quella contenente il file `CMakeLists.txt`), eseguire i seguenti comandi:

```bash
cmake -B build
cmake --build build
```

Gli eseguibili si troveranno nella cartella build, e si chiameranno Tappa01, Tappa02, ..., Tappa10
```
cd build
./Tappa##
