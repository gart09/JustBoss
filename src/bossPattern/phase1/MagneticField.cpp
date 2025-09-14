#include "MagneticField.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>

constexpr float HITBOX_WIDTH = 600.f;
constexpr float HITBOX_HEIGHT = 600.f;
constexpr int DAMAGE = 25;

const AttackTimings MF_TIMINGS = {1.5f, 1.0f, 0.5f}; // 선딜레이, 공격 활성, 후딜레이
const AttackColors MF_COLORS = {
    sf::Color(255, 165, 0, 100), // 경고 색상 (주황)
    sf::Color(0, 0, 139, 200)    // 공격 색상 (검푸른색)
};


MagneticField::MagneticField() {
    cooldown = 7.0f;
    currentCooldown = 3.f;
}

bool MagneticField::canExecute(const Boss& boss, const Player& player) const {
    // 플레이어가 보스 주변 일정 거리 안에 있을 때만 발동 (광역기의 일반적인 조건)
    float playerCenterX = player.getPosition().x + player.getSize() / 2.f;
    float bossCenterX = boss.getPosition().x + boss.getSize() / 2.f;
    float distance = std::abs(bossCenterX - playerCenterX);
    
    float optimalDistance = 300.0f; // 300px 이내로 들어오면 사용

    return currentCooldown <= 0 && distance < optimalDistance;
}

void MagneticField::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});

    std::cout << "Executing MagneticField pattern!" << std::endl;
    
    m_effectTimer = 0.f;

    float bossCenterX = boss.getPosition().x + boss.getSize() / 2.f;
    float bossCenterY = boss.getPosition().y + boss.getSize() / 2.f;
    float hitboxX = bossCenterX - (HITBOX_WIDTH / 2.f);
    float hitboxY = bossCenterY - (HITBOX_HEIGHT / 2.f);
    sf::FloatRect finalHitbox({hitboxX, hitboxY}, {HITBOX_WIDTH, HITBOX_HEIGHT});

    m_visualEffectRect.setPosition(finalHitbox.position);
    m_visualEffectRect.setSize({finalHitbox.size.x, finalHitbox.size.y});

    m_helper.start({finalHitbox}, DAMAGE, MF_TIMINGS, MF_COLORS);
}

void MagneticField::update(float dt, Boss& boss, Player& player) {
    // 모든 업데이트 로직을 Helper에게 위임
    m_helper.update(dt, player, boss);

    if (!m_helper.isFinished()) {
        m_effectTimer += dt;
    }
}

void MagneticField::draw(sf::RenderTarget& target) {
    if (m_helper.isAttack()) {
        const sf::Color color1 = sf::Color(0, 0, 139, 200); // 기본 검푸른색
        const sf::Color color2 = sf::Color(0, 255, 255, 200); // 밝은 청록색
        const float blinkInterval = 0.08f; // 깜빡이는 속도 (작을수록 빠름)

        // effectTimer를 이용해 두 색상을 번갈아 선택
        bool showColor1 = std::fmod(m_effectTimer, blinkInterval * 2) < blinkInterval;

        if (showColor1) {
            m_visualEffectRect.setFillColor(color1);
        } else {
            m_visualEffectRect.setFillColor(color2);
        }
        
        target.draw(m_visualEffectRect);
    } 
    else {
        m_helper.draw(target);
    }
}

bool MagneticField::isFinished() const {
    // 패턴의 종료 여부도 Helper에게 물어봄
    return m_helper.isFinished();
}