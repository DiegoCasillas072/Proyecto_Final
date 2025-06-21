#pragma once
#include <SFML/Graphics.hpp>

class Nivel {
private:
    int nivel;
    int maxNivel;
public:
    Nivel(int maxNivel) : nivel(1), maxNivel(maxNivel) {}

    void siguiente() {
        if (nivel < maxNivel) nivel++;
    }

    void reiniciar() {
        nivel = 1;
    }

    int get() const {
        return nivel;
    }

    bool esUltimo() const {
        return nivel == maxNivel;
    }

    int getMax() const {
        return maxNivel;
    }
};