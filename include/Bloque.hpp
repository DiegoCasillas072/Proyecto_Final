#pragma once
#include <SFML/Graphics.hpp>

class Bloque {
public:
    sf::RectangleShape shape;
    int golpesRestantes;

    Bloque(float x, float y, int golpes) : golpesRestantes(golpes) {
        shape.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
        shape.setOrigin(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
        shape.setPosition(x, y);
        actualizarColor();
    }

    void recibirGolpe() {
        if (golpesRestantes > 0) {
            golpesRestantes--;
            actualizarColor();
        }
    }

    bool destruido() const {
        return golpesRestantes <= 0;
    }

    void actualizarColor() {
        if (golpesRestantes == 3)
            shape.setFillColor(sf::Color::Red);
        else if (golpesRestantes == 2)
            shape.setFillColor(sf::Color::Yellow);
        else
            shape.setFillColor(sf::Color::Green);
    }
};