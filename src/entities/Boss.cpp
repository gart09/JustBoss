#pragma once
#include "Boss.h"
#include "../state/BossPhase1State.h"
#include "../state/BossGroggyState.h"
#include "../state/IBossPhaseState.h"
#include <iostream>
#include <random>

constexpr float WINDOW_WIDTH = 1280.f;
constexpr float WINDOW_HEIGHT = 720.f;
constexpr float GROUND_SIZE = 100.f;

float Boss::randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(gen);
}

Boss::Boss()
{
    m_shape.setSize({m_size, m_size});
    m_shape.setFillColor(sf::Color::Red);
    m_originalColor = m_shape.getFillColor();
    m_shape.setPosition({800.f, WINDOW_HEIGHT - m_size - GROUND_SIZE});

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
    applyPhysics(dt);
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
    if(m_currentState) {
        m_currentState->draw(window, *this);
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
    if (m_currentState && m_currentState->isGroggyState()) {
        damage *= 2;
        std::cout << "Groggy! Damage doubled to " << damage << std::endl;
    }
    m_hp -= damage;
    std::cout << "Boss hit! Remaining HP: " << m_hp << std::endl;

    m_hitEffectTimer = 0.15f;    // 0.15초간 하얗게 점멸
    m_shape.setFillColor(sf::Color::White);

    if (m_hp <= 0) {
        std::cout << "Boss has been defeated!" << std::endl;
    }
}

void Boss::changeState(std::unique_ptr<IBossPhaseState> newState) {
    if(m_currentState)
        m_currentState->exit(*this);
    m_currentState = std::move(newState);
    if(m_currentState) {
        m_currentState->enter(*this);
    }
}

void Boss::wander(float dt) {
    wanderTimer_ -= dt;

    if (wanderTimer_ <= 0.0f) {
        int randomChoice = rand() % 3;
        switch (randomChoice) {
            case 0:
                wanderDirection_ = -1.0f;
                break;
            case 1:
                wanderDirection_ = 0.0f;
                break;
            case 2:
                wanderDirection_ = 1.0f;
                break;
        }
        wanderTimer_ = randomFloat(1.0f, 3.0f); 
    }

    // 3. 현재 설정된 방향으로 보스를 이동시킵니다.
    setVelocity({m_speed * wanderDirection_, 0.f});
}


void Boss::enterGroggyState (PhaseID phaseId) {
    std::cout << "Boss weak point hit! Entering Groggy State!" << std::endl;
    changeState(std::make_unique<BossGroggyState>(3.f, phaseId));
}

IPattern* Boss::getCurrentPattern() const {
    return m_currentState->getCurrentPattern();
}

sf::Vector2f Boss::getCenter() const {
    return {m_shape.getPosition().x + m_size / 2, m_shape.getPosition().y + m_size / 2};
}

void Boss::setVelocity(const sf::Vector2f& velocity) {
    m_velocity = velocity;
}

void Boss::applyPhysics(float dt) {
    m_shape.move(m_velocity * dt);
    
    const sf::Vector2f pos = m_shape.getPosition();
    const sf::Vector2f size = m_shape.getSize();

    if (pos.x < 0.f) {
        m_shape.setPosition({0.f, pos.y});
        m_velocity.x = 0;
    }
    else if (pos.x + size.x > WINDOW_WIDTH) {
        m_shape.setPosition({WINDOW_WIDTH - size.x, pos.y});
        m_velocity.x = 0;
    }
}

PhaseID Boss::getCurrentPhase() const {
    if (m_currentState) {
        return m_currentState->getPhaseID();
    }
    return PhaseID::Phase1;
}