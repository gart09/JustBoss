#pragma once
#include <SFML/Graphics.hpp>

class Boss
{
public:
    Boss();
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    int getHP() { return m_hp; }
    int getMaxHP() { return m_hp; }

private:
    sf::RectangleShape m_shape; // 임시로 사용할 사각형 모양
    int m_hp = 500;
    int m_maxHp = 500;
};
