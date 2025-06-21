#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

const int WIDTH = 1200;   // Ventana más ancha
const int HEIGHT = 800;
const int BLOCK_COLS = 12; // Más columnas para aprovechar el ancho
const int BLOCK_ROWS = 5;
float BLOCK_WIDTH = 60.f;  // Se ajustará dinámicamente en crearNivel
float BLOCK_HEIGHT = 20.f;
const float PADDLE_WIDTH = 100.f * 1.3f; // Antes: 100.f
const float PADDLE_HEIGHT = 20.f;
const float BALL_RADIUS = 10.f;
const int INTENTOS_MAX = 3;
const int NIVELES_MAX = 5;


#include "Bloque.hpp"
#include "Paleta.hpp"
#include "Pelota.hpp"
#include "Intentos.hpp"
#include "Texto.hpp"

enum EstadoJuego { PRESENTACION, JUGANDO, NIVEL_GANADO, GAMEOVER, JUEGO_GANADO };

void crearNivel(std::vector<Bloque>& bloques, int nivel) {
    bloques.clear();

    // Ajustar tamaño de bloque según el nivel y el ancho de la ventana
    int cols = BLOCK_COLS;
    float blockWidth = BLOCK_WIDTH;
    if (nivel == 3 || nivel == 5) {
        cols = 16; // Más bloques en niveles 3 y 5
        blockWidth = (WIDTH - 2 * 40) / cols - 10; // 40 px de margen a cada lado, 10 px separación
    } else {
        cols = BLOCK_COLS;
        blockWidth = (WIDTH - 2 * 40) / cols - 10;
    }
    float blockHeight = BLOCK_HEIGHT;

    int rows = BLOCK_ROWS + nivel - 1;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = 40 + blockWidth / 2 + j * (blockWidth + 10);
            float y = 50 + i * (blockHeight + 10);

            int golpes = 1;
            // Ajustar golpes según el nivel
            if (nivel == 2 && (i + j) % 5 == 0)
                golpes = 2;
            if (nivel == 3) {
                if ((i + j) % 7 == 0)
                    golpes = 3;
                else if ((i + j) % 4 == 0)
                    golpes = 2;
            }
            if (nivel == 4) {
                if ((i + j) % 6 == 0)
                    golpes = 4; // Más resistentes en nivel 4
                else if ((i + j) % 3 == 0)
                    golpes = 3;
                else
                    golpes = 2;
            }
            if (nivel == 5) {
                if ((i + j) % 5 == 0)
                    golpes = 5; // Más resistentes en nivel 5
                else if ((i + j) % 3 == 0)
                    golpes = 4;
                else
                    golpes = 3;
            }

            Bloque bloque(x, y, golpes);
            bloque.shape.setSize(sf::Vector2f(blockWidth, blockHeight));
            bloque.shape.setOrigin(blockWidth / 2, blockHeight / 2);
            bloque.actualizarColor();
            bloques.push_back(bloque);
        }
    }
}

// Clase para manejar el nivel actual
#include "Nivel.hpp"


//int main(){
   // Juego j;
   // j.Iniciar();
//}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Alleyway con Sprites y Fondo");
    window.setFramerateLimit(60);

    // --- MÚSICA Y SONIDOS ---
    sf::Music musicaFondo;
    sf::Music musicaNivel[5];
    musicaNivel[0].openFromFile("./assets/music/nivel1.mp3");
    musicaNivel[1].openFromFile("./assets/music/nivel2.mp3");
    musicaNivel[2].openFromFile("./assets/music/nivel3.mp3");
    musicaNivel[3].openFromFile("./assets/music/nivel4.mp3");
    musicaNivel[4].openFromFile("./assets/music/nivel5.mp3");

    musicaFondo.openFromFile("./assets/music/Intro Theme.mp3");

    sf::SoundBuffer bufferBloque, bufferPaleta;
    bufferBloque.loadFromFile("./assets/music/jump_03.wav");
    bufferPaleta.loadFromFile("./assets/music/jump_08.wav");
    sf::Sound sonidoBloque(bufferBloque);
    sf::Sound sonidoPaleta(bufferPaleta);

    // Cargar texturas
    sf::Texture texturaFondo, texturaPaleta, texturaPelota;
    if (!texturaFondo.loadFromFile("./assets/images/fondo.jpg") ||
        !texturaPaleta.loadFromFile("./assets/images/paleta.png") ||
        !texturaPelota.loadFromFile("./assets/images/pelota.png")) {
        return 1;
    }

    // Crear sprites
    sf::Sprite fondo(texturaFondo);
    sf::Sprite spritePaleta(texturaPaleta);
    sf::Sprite spritePelota(texturaPelota);

    // Ajustar tamaño del fondo al tamaño de la ventana
    float escalaFondoX = static_cast<float>(WIDTH) / texturaFondo.getSize().x;
    float escalaFondoY = static_cast<float>(HEIGHT) / texturaFondo.getSize().y;
    fondo.setScale(escalaFondoX, escalaFondoY);

    // Ajustar tamaño de la paleta
    spritePaleta.setScale(0.05f, 0.2f);

    // Ajustar tamaño de la pelota (asumiendo textura cuadrada)
    float diametroPelota = BALL_RADIUS * 2;
    float escalaPelota = diametroPelota / texturaPelota.getSize().x;
    spritePelota.setScale(escalaPelota, escalaPelota);

    sf::Font font;
    if (!font.loadFromFile("./assets/fonts/Ring.ttf")) {
        return 1;
    }

    sf::Text textoPresentacion(
        "Juego Alleyway\n"
        "Toca ENTER para iniciar\n\n"
        "CONTROLES\n"
        "Flecha Izquierda Derecha Mover paleta\n"
        "ESPACIO Pausa\n"
        "ESC Salir\n",
        font, 36);
    textoPresentacion.setFillColor(sf::Color::White);
    textoPresentacion.setStyle(sf::Text::Bold);
    textoPresentacion.setPosition(150, 120);

    sf::Text textoPausa("PAUSA\nPresiona ESPACIO para continuar", font, 36);
    textoPausa.setFillColor(sf::Color::Cyan);
    textoPausa.setStyle(sf::Text::Bold);
    textoPausa.setPosition(250, 300);

    sf::Text textoGameOver("Haz perdido intentalo mas tarde\nPresiona ESC para salir", font, 36);
    textoGameOver.setFillColor(sf::Color::Red);
    textoGameOver.setStyle(sf::Text::Bold);
    textoGameOver.setPosition(200, 250);

    sf::Text textoIntentos("", font, 24);
    textoIntentos.setFillColor(sf::Color::Yellow);
    textoIntentos.setPosition(10, 10);

    sf::Text textoNivel("", font, 24);
    textoNivel.setFillColor(sf::Color::White);
    textoNivel.setPosition(650, 10);

    sf::Text textoNivelGanado("Haz ganado\nPresiona ENTER para el siguiente nivel", font, 32);
    textoNivelGanado.setFillColor(sf::Color::Green);
    textoNivelGanado.setStyle(sf::Text::Bold);
    textoNivelGanado.setPosition(120, 220);

    sf::Text textoJuegoGanado("Felicidades Has ganado ahora fuera de aqui", font, 36);
    textoJuegoGanado.setFillColor(sf::Color::Magenta);
    textoJuegoGanado.setStyle(sf::Text::Bold);
    textoJuegoGanado.setPosition(100, 250);

    EstadoJuego estado = PRESENTACION;
    Nivel nivel(NIVELES_MAX);

    Paleta paleta(texturaPaleta);
    Pelota pelota(texturaPelota);
    std::vector<Bloque> bloques;
    crearNivel(bloques, nivel.get());

    float velocidadPaleta = 8.f * 1.3f; // Ahora la paleta es 30% más rápida en todos los niveles
    float escalaPaleta = 0.05f; // Escala normal

    bool enPausa = false;

    // Iniciar música de fondo para la pantalla de inicio
    musicaFondo.setLoop(true);
    musicaFondo.play();

    int nivelActualMusica = 0; // Para saber si hay que cambiar la música de nivel

    Intentos intentos(INTENTOS_MAX);
    Texto textos(font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (estado == PRESENTACION && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                estado = JUGANDO;
                intentos.reiniciar();
                nivel.reiniciar();
                pelota.reset(nivel.get());
                paleta = Paleta(texturaPaleta);
                crearNivel(bloques, nivel.get());

                // Cambiar a música de nivel 1
                musicaFondo.stop();
                musicaNivel[0].setLoop(true);
                musicaNivel[0].play();
                nivelActualMusica = 1;
            }
            if (estado == NIVEL_GANADO && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                nivel.siguiente();
                if (nivel.get() > nivel.getMax()) {
                    estado = JUEGO_GANADO;
                    // Música de final
                    for (int i = 0; i < 5; ++i) musicaNivel[i].stop();
                    musicaFondo.setLoop(true);
                    musicaFondo.play();
                } else {
                    crearNivel(bloques, nivel.get());
                    pelota.reset(nivel.get());
                    paleta = Paleta(texturaPaleta);
                    intentos.reiniciar();
                    estado = JUGANDO;
                    // Cambiar música de nivel
                    for (int i = 0; i < 5; ++i) musicaNivel[i].stop();
                    musicaNivel[nivel.get() - 1].setLoop(true);
                    musicaNivel[nivel.get() - 1].play();
                    nivelActualMusica = nivel.get();
                }
            }
            if (estado == GAMEOVER && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (estado == JUEGO_GANADO && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            // Botón de pausa con ESPACIO
            if (estado == JUGANDO && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                enPausa = !enPausa;
            }
        }

        // Cambiar música si cambia el nivel manualmente (por seguridad)
        if (estado == JUGANDO && nivelActualMusica != nivel.get()) {
            for (int i = 0; i < 5; ++i) musicaNivel[i].stop();
            musicaNivel[nivel.get() - 1].setLoop(true);
            musicaNivel[nivel.get() - 1].play();
            nivelActualMusica = nivel.get();
        }

        // Ajustar la paleta según el nivel
        if (nivel.esUltimo()) {
            escalaPaleta = 0.05f / 1.33f;
            velocidadPaleta = 8.f * 3 * 1.3f; // 3 veces más rápida y 30% extra
        } else if (nivel.get() >= 3) {
            escalaPaleta = 0.05f;
            velocidadPaleta = 8.f * 1.5f * 1.3f; // 50% más rápida desde nivel 3 y 30% extra
        } else {
            escalaPaleta = 0.05f;
            velocidadPaleta = 8.f * 1.3f; // 30% más rápida en niveles bajos
        }
        paleta.sprite.setScale(escalaPaleta, 0.2f);

        // Calcular escala para el sprite de la paleta
        float anchoSprite = paleta.sprite.getTexture()->getSize().x;
        float altoSprite = paleta.sprite.getTexture()->getSize().y;
        // Aumenta un 18% el ancho visual respecto a la paleta lógica
        float escalaX = (PADDLE_WIDTH * 1.18f) / anchoSprite;
        float escalaY = escalaX; // Mantener proporción para no deformar el sprite

        paleta.sprite.setScale(escalaX, escalaY);

        // Si quieres que la altura lógica de la paleta también se ajuste visualmente, puedes hacer:
        paleta.shape.setSize(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
        paleta.shape.setOrigin(PADDLE_WIDTH / 2, PADDLE_HEIGHT / 2);

        // Fondo y color de texto según estado
        if (estado == JUGANDO) {
            window.clear();
            window.draw(fondo);
        } else if (estado == PRESENTACION) {
            window.clear(sf::Color::Black);
            textoPresentacion.setFillColor(sf::Color::White);
        } else if (estado == JUEGO_GANADO) {
            window.clear(sf::Color::Black);
            textoJuegoGanado.setFillColor(sf::Color::Yellow);
        } else if (estado == GAMEOVER) {
            window.clear(sf::Color::Black);
            textoGameOver.setFillColor(sf::Color::Red);
        } else if (estado == NIVEL_GANADO) {
            window.clear();
            window.draw(fondo);
        }

        if (estado == PRESENTACION) {
            window.draw(textoPresentacion);
        }
        else if (estado == JUGANDO) {
            if (enPausa) {
                window.draw(textoPausa);
                window.display();
                continue;
            }

            // Movimiento paleta
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                paleta.mover(-velocidadPaleta);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                paleta.mover(velocidadPaleta);

            // Mover pelota
            pelota.mover();

            // Rebote en paredes
            if (pelota.shape.getPosition().x - BALL_RADIUS < 0 || pelota.shape.getPosition().x + BALL_RADIUS > WIDTH)
                pelota.reboteHorizontal();
            if (pelota.shape.getPosition().y - BALL_RADIUS < 0)
                pelota.reboteVertical();

            // Rebote en paleta
            if (pelota.shape.getGlobalBounds().intersects(paleta.shape.getGlobalBounds())) {
                pelota.reboteVertical();
                sonidoPaleta.play();
            }

            // Rebote y daño en bloques
            for (auto& bloque : bloques) {
                if (!bloque.destruido() && pelota.shape.getGlobalBounds().intersects(bloque.shape.getGlobalBounds())) {
                    bloque.recibirGolpe();
                    pelota.reboteVertical();
                    sonidoBloque.play();
                    break;
                }
            }

            // Perder la bola
            if (pelota.shape.getPosition().y - BALL_RADIUS > HEIGHT) {
                intentos.perder();
                if (!intentos.agotado()) {
                    pelota.reset(nivel.get());
                    paleta = Paleta(texturaPaleta);
                } else {
                    estado = GAMEOVER;
                }
            }

            // Mostrar intentos y nivel
            textos.setIntentos(intentos.get());
            textos.setNivel(nivel.get());
            window.draw(textos.intentos);
            window.draw(textos.nivel);

            // Mostrar textos según estado
            if (estado == PRESENTACION) window.draw(textos.presentacion);
            else if (estado == JUGANDO && enPausa) window.draw(textos.pausa);
            else if (estado == NIVEL_GANADO) window.draw(textos.nivelGanado);
            else if (estado == GAMEOVER) window.draw(textos.gameOver);
            else if (estado == JUEGO_GANADO) window.draw(textos.juegoGanado);

            // Dibujar
            window.draw(paleta.sprite);
            window.draw(pelota.sprite);
            for (const auto& bloque : bloques)
                if (!bloque.destruido())
                    window.draw(bloque.shape);

            // Si todos los bloques están destruidos, pasar al siguiente nivel
            bool todosDestruidos = true;
            for (const auto& bloque : bloques)
                if (!bloque.destruido())
                    todosDestruidos = false;
            if (todosDestruidos) {
                estado = NIVEL_GANADO;
            }
        }
        else if (estado == NIVEL_GANADO) {
            window.draw(textoNivelGanado);
        }
        else if (estado == GAMEOVER) {
            window.draw(textoGameOver);
        }
        else if (estado == JUEGO_GANADO) {
            window.draw(textoJuegoGanado);
        }

        window.display();
    }

    return 0;
}