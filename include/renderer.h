#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../include/cpu.h"

class Renderer
{
    public:
        Renderer(sf::RenderWindow &in_window, CPU &in_cpu);
        sf::RenderWindow &window;
        CPU &cpu;
        sf::Image image;
        int windowSizeX;
        int windowSizeY;
        const int dispX = 64;
        const int dispY = 32;
        void drawBuffer();
        void checkKeyPress();
        static constexpr sf::Keyboard::Key hexkeys[16] = 
        {
            sf::Keyboard::Key::Numpad0,
            sf::Keyboard::Key::Numpad1,
            sf::Keyboard::Key::Numpad2,
            sf::Keyboard::Key::Numpad3,
            sf::Keyboard::Key::Numpad4,
            sf::Keyboard::Key::Numpad5,
            sf::Keyboard::Key::Numpad6,
            sf::Keyboard::Key::Numpad7,
            sf::Keyboard::Key::Numpad8,
            sf::Keyboard::Key::Numpad9,
            sf::Keyboard::Key::A,
            sf::Keyboard::Key::B,
            sf::Keyboard::Key::C,
            sf::Keyboard::Key::D,
            sf::Keyboard::Key::E,
            sf::Keyboard::Key::F
        };
};