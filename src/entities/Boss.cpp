#include "Boss.h"

Boss::Boss()
{
    // 보스를 임시로 거대한 빨간색 사각형으로 표현합니다.
    m_shape.setSize({200.f, 250.f});
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition({800.f, 350.f});
}

void Boss::update(sf::Time deltaTime)
{
    // TODO: 여기에 보스의 AI 로직 (패턴, 이동 등)을 구현합니다.
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}
