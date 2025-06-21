#pragma once
#include <SFML/Graphics.hpp>

class Pelota {
public:
    sf::Sprite sprite;
    sf::CircleShape shape; // Para colisiones
    sf::Vector2f velocidadBase;
    sf::Vector2f velocidad;

    Pelota(const sf::Texture& textura) {
        sprite.setTexture(textura);

        // Ajustar escala para que se ajuste al tamaño de la pelota
        float diametroPelota = BALL_RADIUS * 2;
        float escalaPelota = diametroPelota / textura.getSize().x;
        sprite.setScale(escalaPelota, escalaPelota);
        sprite.setOrigin(textura.getSize().x / 2, textura.getSize().y / 2);

        sprite.setPosition(WIDTH / 2, HEIGHT / 2);

        shape.setRadius(BALL_RADIUS);
        shape.setOrigin(BALL_RADIUS, BALL_RADIUS);
        shape.setPosition(WIDTH / 2, HEIGHT / 2);
        shape.setFillColor(sf::Color::Transparent);

        // Aumentar velocidad base un 20% y luego un 30% extra (total 56% más rápido)
        velocidadBase = sf::Vector2f(4.f * 1.2f * 1.3f, -4.f * 1.2f * 1.3f);
        velocidad = velocidadBase;
    }

    void mover() {
        sprite.move(velocidad);
        shape.move(velocidad);
    }

    void reboteHorizontal() {
        velocidad.x = -velocidad.x;
    }

    void reboteVertical() {
        velocidad.y = -velocidad.y;
    }

    void reset(int nivel) {
        sprite.setPosition(WIDTH / 2, HEIGHT / 2);
        shape.setPosition(WIDTH / 2, HEIGHT / 2);
        float incremento = 1.5f * (nivel - 1);
        // Aumentar velocidad base un 20% y luego un 30% extra (total 56% más rápido)
        velocidadBase = sf::Vector2f((4.f + incremento) * 1.2f * 1.3f, (-4.f - incremento) * 1.2f * 1.3f);
        velocidad = velocidadBase;
    }
};