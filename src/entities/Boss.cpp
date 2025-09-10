#include "Boss.h"
#include <iostream>

constexpr float BOSS_WIDTH = 250.f;
constexpr float BOSS_HEIGHT = 250.f;
constexpr float BOSS_MAX_HP = 500.f;
constexpr float WINDOW_WIDTH = 1280.f;
constexpr float WINDOW_HEIGHT = 720.f;
constexpr float GROUND_SIZE = 100.f;

Boss::Boss()
{
    m_shape.setSize({BOSS_WIDTH, BOSS_HEIGHT});
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition({800.f, WINDOW_HEIGHT - BOSS_HEIGHT - GROUND_SIZE});

    m_hp = BOSS_MAX_HP;
    m_hitEffectTimer = 0.f;
}

void Boss::update(sf::Time deltaTime)
{
    float dt = deltaTime.asSeconds();
    // 히트 점멸 효과 처리
    if (m_hitEffectTimer > 0.f) {
        m_hitEffectTimer -= dt;
        // 점멸 효과 시간이 끝나면 원래 색으로 복원
        if (m_hitEffectTimer <= 0.f) {
            m_shape.setFillColor(m_originalColor);
        }
    }
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

// Player 클래스에서 충돌 검사에 사용
sf::FloatRect Boss::getHitbox() const
{
    return m_shape.getGlobalBounds();
}

// 공격받는 로직
void Boss::takeDamage(int damage)
{
    if (m_hp <= 0) {
        return;
    }

    m_hp -= damage;
    std::cout << "Boss hit! Remaining HP: " << m_hp << std::endl;

    m_hitEffectTimer = 0.15f;    // 0.15초간 하얗게 점멸
    m_shape.setFillColor(sf::Color::White);

    if (m_hp <= 0) {
        std::cout << "Boss has been defeated!" << std::endl;
        // TODO: 보스 사망 로직 (예: 상태 변경, 소멸 애니메이션)
    }
}
