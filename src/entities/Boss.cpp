#pragma once
#include "Boss.h"
#include "../state/BossPhase1State.h"
#include <iostream>
#include <random>

constexpr float BOSS_WIDTH = 200.f;
constexpr float BOSS_HEIGHT = 200.f;
constexpr float WINDOW_WIDTH = 1280.f;
constexpr float WINDOW_HEIGHT = 720.f;
constexpr float GROUND_SIZE = 100.f;

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(gen);
}

Boss::Boss()
{
    m_shape.setSize({BOSS_WIDTH, BOSS_HEIGHT});
    m_shape.setFillColor(sf::Color::Red);
    m_originalColor = m_shape.getFillColor();
    m_shape.setPosition({800.f, WINDOW_HEIGHT - BOSS_HEIGHT - GROUND_SIZE});

    m_hp = m_maxHp;
    m_hitEffectTimer = 0.f;
    changeState(std::make_unique<BossPhase1State>());
}
Boss::~Boss() = default;

void Boss::update(sf::Time deltaTime, Player& player)
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
    if(m_currentState)
        m_currentState->update(*this, dt, player);
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    if(m_currentState) {
        m_currentState->draw(window);
    }
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

    /*if( this->m_hp < 60 && dynamic_cast<BossPhase1State*>(currentState)) {
        changeState(new BossPhase2State());
    }*/
    if (m_hp <= 0) {
        std::cout << "Boss has been defeated!" << std::endl;
        // TODO: 보스 사망 로직 (예: 상태 변경, 소멸 애니메이션)
    }
}

void Boss::changeState(std::unique_ptr<IBossPhaseState> newState) {
    m_currentState = std::move(newState);
    if(m_currentState) {
        m_currentState->enter(*this);
    }
}

void Boss::wander(float dt, float moveSpeed) {
    // 1. 타이머를 매 프레임 감소시킵니다.
    wanderTimer_ -= dt;

    // 2. 타이머가 0 이하로 떨어지면 방향을 바꾸고 타이머를 재설정합니다.
    if (wanderTimer_ <= 0.0f) {
        // 방향을 반대로 바꿉니다. (1 -> -1, -1 -> 1)
        wanderDirection_ *= -1.0f; 
        
        // 다음 방향 전환까지의 시간을 1초에서 3초 사이의 랜덤한 값으로 설정합니다.
        wanderTimer_ = randomFloat(1.0f, 3.0f); 
    }

    // 3. 현재 설정된 방향으로 보스를 이동시킵니다.
    move(wanderDirection_ * moveSpeed * dt, 0);
}

// Boss의 위치를 실제로 바꾸는 함수 (예시)
void Boss::move(float vx, float vy) {
    m_shape.setPosition({m_shape.getPosition().x + vx, m_shape.getPosition().y + vy});
}