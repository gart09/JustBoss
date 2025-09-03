#include "Boss.h"

Boss::Boss()
{
    m_shape.setSize({250.f, 250.f});
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition({800.f, 720.f-250.f});
}

void Boss::update(sf::Time deltaTime)
{
    // TODO: 여기에 보스의 AI 로직 (패턴, 이동 등)을 구현합니다.
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}
