#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Texto {
public:
    sf::Text presentacion;
    sf::Text pausa;
    sf::Text gameOver;
    sf::Text nivelGanado;
    sf::Text juegoGanado;
    sf::Text intentos;
    sf::Text nivel;

    Texto(const sf::Font& font) {
        presentacion = sf::Text(
            "Juego Alleyway\n"
            "Toca ENTER para iniciar\n\n"
            "CONTROLES\n"
            "Flecha Izquierda Derecha Mover paleta\n"
            "ESPACIO Pausa\n"
            "ESC Salir\n", font, 36);
        presentacion.setFillColor(sf::Color::White);
        presentacion.setStyle(sf::Text::Bold);
        presentacion.setPosition(150, 120);

        pausa = sf::Text("PAUSA\nPresiona ESPACIO para continuar", font, 36);
        pausa.setFillColor(sf::Color::Cyan);
        pausa.setStyle(sf::Text::Bold);
        pausa.setPosition(250, 300);

        gameOver = sf::Text("Haz perdido intentalo mas tarde\nPresiona ESC para salir", font, 36);
        gameOver.setFillColor(sf::Color::Red);
        gameOver.setStyle(sf::Text::Bold);
        gameOver.setPosition(200, 250);

        nivelGanado = sf::Text("Haz ganado\nPresiona ENTER para el siguiente nivel", font, 32);
        nivelGanado.setFillColor(sf::Color::Green);
        nivelGanado.setStyle(sf::Text::Bold);
        nivelGanado.setPosition(120, 220);

        juegoGanado = sf::Text("Felicidades Has ganado ahora fuera de aqui", font, 36);
        juegoGanado.setFillColor(sf::Color::Yellow);
        juegoGanado.setStyle(sf::Text::Bold);
        juegoGanado.setPosition(100, 250);

        intentos = sf::Text("", font, 24);
        intentos.setFillColor(sf::Color::Yellow);
        intentos.setPosition(10, 10);

        nivel = sf::Text("", font, 24);
        nivel.setFillColor(sf::Color::White);
        nivel.setPosition(650, 10);
    }

    void setIntentos(int valor) {
        intentos.setString("Intentos: " + std::to_string(valor));
    }
    void setNivel(int valor) {
        nivel.setString("Nivel: " + std::to_string(valor));
    }
};