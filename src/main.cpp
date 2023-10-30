#include <cstdio>
#include "../include/cpu.h"
#include "../include/renderer.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace utils;

int main()
{
    const int windowX = 640;
    const int windowY = 320;
    CPU cpu;
    Memory memory;
    cpu.reset(memory);
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "SFML works!");
    Renderer renderer(window, cpu);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Texture texture;
        renderer.drawBuffer();
        texture.loadFromImage(renderer.image);
        sf::Sprite sprite(texture);
        sprite.setScale(10, 10);
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0; 

}