#include "BossGroggyState.h"
#include "../entities/Boss.h"
#include "BossPhase1State.h"
#include <iostream>
#include <cmath>

BossGroggyState::BossGroggyState(float duration, PhaseID prevPhaseId) : m_duration(duration), m_timer(0.f), m_prevPhaseId(prevPhaseId) {}

void BossGroggyState::enter(Boss& boss) {
    std::cout << "Boss is now Groggy for " << m_duration << " seconds." << std::endl;
    m_timer = 0.f;
    
    // 그로기 상태에 진입하면 움직임을 멈춥니다.
    boss.setVelocity({0.f, 0.f});
}

void BossGroggyState::update(Boss& boss, float dt, Player& player) {
    m_timer += dt;
    if (m_timer >= m_duration) {
        switch(m_prevPhaseId){
            case PhaseID::Phase1:
                boss.changeState(std::make_unique<BossPhase1State>());
                break;
            default:
                break;
        }
    }
}

void BossGroggyState::draw(sf::RenderTarget& target, Boss& boss) {
    const sf::Color color1 = sf::Color::Red;
    const sf::Color color2 = sf::Color(255, 165, 0); // 주황색
    const float blinkInterval = 0.15f;

    bool showColor1 = std::fmod(m_timer, blinkInterval * 2) < blinkInterval;

    if (showColor1) {
        boss.setColor(color1);
    } else {
        boss.setColor(color2);
    }
}

void BossGroggyState::exit(Boss& boss) {
    // 그로기 상태가 끝나면 보스의 색상을 원래대로 되돌립니다.
    boss.resetColor();
    std::cout << "Boss recovered from groggy state." << std::endl;
}

PhaseID BossGroggyState::getPhaseID() const {
    return PhaseID::Groggy;
}
