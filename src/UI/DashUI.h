#pragma once
#include <SFML/Graphics.hpp>
#include "../entities/Player.h"

class DashUI {
public:
    void draw(Player& player, sf::RenderWindow& window);
};