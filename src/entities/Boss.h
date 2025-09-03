#pragma once
#include <SFML/Graphics.hpp>

class Boss
{
public:
    Boss();
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape; // 임시로 사용할 사각형 모양
};
