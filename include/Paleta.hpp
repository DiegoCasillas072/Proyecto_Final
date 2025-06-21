#pragma once
#include <SFML/Graphics.hpp>

class Paleta {
public:
    sf::Sprite sprite;
    sf::RectangleShape shape; // Para colisiones

    Paleta(const sf::Texture& textura) {
        sprite.setTexture(textura);
        sprite.setOrigin(textura.getSize().x / 2, textura.getSize().y / 2);
        sprite.setPosition(WIDTH / 2, HEIGHT - 40);
        //posision de sprite en el centro de PADDLE
        
        
        

        // Crear la forma de colisión
        shape.setSize(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
        shape.setOrigin(PADDLE_WIDTH / 2, PADDLE_HEIGHT / 2);
        shape.setPosition(WIDTH / 2, HEIGHT - 40);
        shape.setFillColor(sf::Color::Red);
    }

    void mover(float dx) {
        float nuevaX = sprite.getPosition().x + dx;
        if (nuevaX - PADDLE_WIDTH / 2 < 0)
            nuevaX = PADDLE_WIDTH / 2;
        if (nuevaX + PADDLE_WIDTH / 2 > WIDTH)
            nuevaX = WIDTH - PADDLE_WIDTH / 2;
        sprite.setPosition(nuevaX, sprite.getPosition().y);
        shape.setPosition(nuevaX, shape.getPosition().y);
    }
};