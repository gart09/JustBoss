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

    // 플레이어가 충돌을 확인할 수 있도록 자신의 히트박스를 반환하는 함수
    sf::FloatRect getHitbox() const;

    // 플레이어로부터 데미지를 받는 함수
    void takeDamage(int damage);

private:
    sf::RectangleShape m_shape; // 임시로 사용할 사각형 모양
    int m_hp = 500;
    int m_maxHp = 500;

    sf::Color m_originalColor;     // 원래 색상
    float m_hitEffectTimer;        // 하얗게 점멸하는 효과가 지속되는 시간
};
