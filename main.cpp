#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <time.h>
#include <sstream>
#include "include/Grafo.h"

int main() {
    srand(time(0));
    int n = 10; //filas
    int m = 10; //columnas
    int cantObj = 1;
    int cellSize = 25; //tamano de celda
    int ayuda = 0;
    Grafo* g = new Grafo(n, m); //genera grafo con un algoritmo aleatorio
    g->generarLaberinto(rand()%3,cantObj);
    LinkedList<int>* ruta = g->dijkstra(g->getActual()); //guarda el dijkstra

    //presionar teclas
    bool upKeyPressed = false;
    bool downKeyPressed = false;
    bool leftKeyPressed = false;
    bool rightKeyPressed = false;
    bool hKeyPressed = false;
    bool showRoute = false;
    bool gameOver = false;

    //fuente
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Clock clock;
    int timer = 3 * 60;

    sf::RenderWindow window(sf::VideoMode(m * cellSize, n * cellSize), "Laberintos");

    //texto de reloj y game over
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    sf::Color textColor = sf::Color::White;
    textColor.a = 128;
    text.setFillColor(textColor);
    float xCoordinate = (m-2)*cellSize;
    text.setPosition(xCoordinate, 10.0f);

    //jugador
    const int pRadius = 40 * cellSize / 100;
    sf::CircleShape player(pRadius);
    player.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        int cols = g->getCols();

        //jugador llega al final
        if(g->getActual() == g->getFin() && g->getObjetivos()->getSize() == 0) {
            if(!(n >= 40)) n += 5;
            m += 5;
            cantObj += 2;
            ayuda = 0;
            delete g;
            g = new Grafo(n, m);
            rand();
            g->generarLaberinto(rand()%3,cantObj);
            window.setSize(sf::Vector2u(m * cellSize, n * cellSize));
            sf::FloatRect visibleArea(0,0,m * cellSize,n * cellSize);
            window.setView(sf::View(visibleArea));
            float xCoordinate = (m-2)*cellSize;
            text.setPosition(xCoordinate, 10.0f);
            clock.restart();
        }

        //si el jugador toca un objetivo
        for(g->getObjetivos()->goToStart();!g->getObjetivos()->atEnd();g->getObjetivos()->next()) {
            if(g->getActual() == g->getObjetivos()->getElement()) g->getObjetivos()->remove();
        }

        //presionar teclas
        if (event.type == sf::Event::KeyPressed && !gameOver) {
            if (event.key.code == sf::Keyboard::Up && !upKeyPressed) {
                upKeyPressed = true;
                g->moverArriba();
                showRoute = false;
            } else if (event.key.code == sf::Keyboard::Down && !downKeyPressed) {
                downKeyPressed = true;
                g->moverAbajo();
                showRoute = false;
            } else if (event.key.code == sf::Keyboard::Left && !leftKeyPressed) {
                leftKeyPressed = true;
                g->moverIzquierda();
                showRoute = false;
            } else if (event.key.code == sf::Keyboard::Right && !rightKeyPressed) {
                rightKeyPressed = true;
                g->moverDerecha();
                showRoute = false;
            }
        }


        //soltar teclas
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Up) {
                upKeyPressed = false;
            } else if (event.key.code == sf::Keyboard::Down) {
                downKeyPressed = false;
            } else if (event.key.code == sf::Keyboard::Left) {
                leftKeyPressed = false;
            } else if (event.key.code == sf::Keyboard::Right) {
                rightKeyPressed = false;
            } else if (event.key.code == sf::Keyboard::Key::H) {
                hKeyPressed = false;
            }
        }


        //tecla de ayuda (h)
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) && !hKeyPressed && !showRoute && !gameOver) {
            hKeyPressed = true;
            ayuda += 20;
            showRoute = true;
            ruta = g->dijkstra(g->getActual());
        }

        //posicion del jugador cada loop
        int playerPos = g->getActual();
        int playerRow = playerPos / m;
        int playerCol = playerPos % m;
        player.setPosition((playerCol * cellSize) + (cellSize - 2 * pRadius) / 2, (playerRow * cellSize) + (cellSize - 2 * pRadius) / 2);


        //manejo de tiempo
        sf::Time elapsed = clock.getElapsedTime();
        int seconds = timer - static_cast<int>(elapsed.asSeconds()) - ayuda;
        seconds = max(0,seconds);
        int minutes = seconds/60;
        seconds %= 60;
        std::stringstream ss;
        ss << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
        text.setString(ss.str());

        //si es game over
        if(seconds <= 0 && minutes <= 0) {
            gameOver = true;
        }

        //termina el juego y no deja al jugador moverse
        if(gameOver) {
            sf::Color textColor = sf::Color::Magenta;
            textColor.a = 255;
            text.setFillColor(textColor);
            text.setCharacterSize(30);
            text.setStyle(sf::Text::Bold);
            text.setPosition(((cellSize*(m-2)) - (cellSize*3)) /2, ((cellSize*n) - (cellSize*2)) /2);
            text.setString("Game over!");
        }

        window.clear();

        //dibuja la cuadricula
        for (int i = 0; i < g->getnumNodos(); ++i) {
            int row = i/cols;
            int col = i%cols;
            LinkedList<int>* lista = g->getListaAdy();
            LinkedList<int>* objetivos = g->getObjetivos();
            //dibuja una linea a la derecha de un nodo si tiene una pared
            if(!lista[i].contains(i+1)) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f((col+1) * cellSize, row * cellSize), sf::Color::White),
                    sf::Vertex(sf::Vector2f((col+1) * cellSize, cellSize * (row + 1)), sf::Color::White)
                };
                window.draw(line,2,sf::Lines);
            }
            //dibuja una linea abajo de un nodo si tiene una pared
            if (!lista[i].contains(i + cols)) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(col * cellSize, (row + 1) * cellSize), sf::Color::White),
                    sf::Vertex(sf::Vector2f((col + 1) * cellSize, (row + 1) * cellSize), sf::Color::White)
                };
                window.draw(line, 2, sf::Lines);
            }

            //muestra la ruta de dijkstra
            if(showRoute) {
                if(ruta->contains(i)) {
                    int objSize = 90*cellSize/100;
                    sf::RectangleShape cell(sf::Vector2f(objSize, objSize));
                    cell.setPosition((col * cellSize) + (cellSize-objSize)/2, (row * cellSize) + (cellSize-objSize)/2);
                    cell.setFillColor(sf::Color::Blue); // Set fill color to transparent
                    window.draw(cell);
                }
            }

            //dibuja la meta
            if(i == g->getFin()) {
                int endSize = 70*cellSize/100;
                sf::RectangleShape cell(sf::Vector2f(endSize, endSize));
                cell.setPosition((col * cellSize) + (cellSize-endSize)/2, (row * cellSize) + (cellSize-endSize)/2);
                cell.setFillColor(sf::Color::Red); // Set fill color to transparent
                window.draw(cell);
            }

            //dibuja objetivos
            if(objetivos->contains(i)) {
               int objSize = 40*cellSize/100;
                sf::RectangleShape cell(sf::Vector2f(objSize, objSize));
                cell.setPosition((col * cellSize) + (cellSize-objSize)/2, (row * cellSize) + (cellSize-objSize)/2);
                cell.setFillColor(sf::Color::Yellow); // Set fill color to transparent
                window.draw(cell);
            }

        }

        window.draw(player);
        window.draw(text);

        //Display
        window.display();
    }

    return EXIT_SUCCESS;
}
