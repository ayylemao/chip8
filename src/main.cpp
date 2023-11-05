#include <cstdio>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../include/cpu.h"
#include "../include/renderer.h"
#include "../include/utils.h"

using namespace utils;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <ROM PATH>\n";
        return 1;
    }
    std::string filepath = argv[1];
    const int windowX = 640;
    const int windowY = 320;
    sf::Clock clock;  // Initialize a clock to measure elapsed time
    CPU cpu;
    Memory memory;
    cpu.reset(memory);
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "CHIP-8");
    Renderer renderer(window, cpu);
    static constexpr Word ROM_OFFSET = 0x200;
    cpu.reset(memory);
    cpu.loadSprites(memory);
    std::vector<Byte> rom = utils::loadROM(filepath);

    for (int addr = 0; addr < rom.size(); addr++)
    {
        memory[ROM_OFFSET + addr] = rom[addr];
    }
    cpu.PC = ROM_OFFSET;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        if (clock.getElapsedTime().asSeconds() >= 0.00185)
        {
            clock.restart();  // Reset the clock
            //utils::printOpcode(memory[cpu.PC], memory[cpu.PC+1]);
            //std::cout << '\n';
            cpu.execute(memory);
        }
        // KeyPresses
        renderer.checkKeyPress();

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