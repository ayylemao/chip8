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
};