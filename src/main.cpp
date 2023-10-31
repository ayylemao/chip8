#include <cstdio>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../include/cpu.h"
#include "../include/renderer.h"
#include "../include/utils.h"

using namespace utils;

int main()
{
    const int windowX = 640;
    const int windowY = 320;
    sf::Clock clock;  // Initialize a clock to measure elapsed time
    CPU cpu;
    Memory memory;
    cpu.reset(memory);
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "SFML works!");
    Renderer renderer(window, cpu);

    cpu.reset(memory);
    cpu.loadSprites(memory);
    std::vector<Byte> rom = utils::loadROM("/home/matthias/projects/chip8/roms/2-ibm-logo.ch8");

    for (int addr = 0; addr < 260; addr++)
    {
        memory[0x200 + addr] = rom[addr];
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        if (clock.getElapsedTime().asSeconds() >= 0.5)
        {
            clock.restart();  // Reset the clock
            printByte(memory[cpu.PC]);
            printByte(memory[cpu.PC+1]);
            cpu.execute(memory);
        }
        sf::Texture texture;
        if (cpu.redraw == true)
        {
            renderer.drawBuffer();
            cpu.redraw = false;
        }
        texture.loadFromImage(renderer.image);
        sf::Sprite sprite(texture);
        sprite.setScale(10, 10);
        window.draw(sprite);
        window.display();
    }
    return 0; 
}