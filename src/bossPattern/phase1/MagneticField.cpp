#include "MagneticField.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>
#include <cmath>

namespace {
    constexpr float HITBOX_WIDTH = 600.f;
    constexpr float HITBOX_HEIGHT = 600.f;
    constexpr int DAMAGE = 25;
    const AttackTimings MF_TIMINGS = {1.5f, 1.0f, 0.5f};
    const AttackColors MF_COLORS = {
        sf::Color(255, 165, 0, 100),
        sf::Color(0, 0, 139, 200)
    };
}

MagneticField::MagneticField() {
    cooldown = 7.0f;
    currentCooldown = 3.f;
}

bool MagneticField::canExecute(const Boss& boss, const Player& player) const {
    float distance = std::abs(boss.getCenter().x - player.getCenter().x);
    float optimalDistance = 300.0f;
    return currentCooldown <= 0 && distance < optimalDistance;
}

void MagneticField::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    std::cout << "Executing MagneticField pattern!" << std::endl;
    
    m_effectTimer = 0.f;

    // 이 패턴은 보스 자신의 중심을 기준으로 히트박스를 생성합니다.
    float bossCenterX = boss.getCenter().x;
    float bossCenterY = boss.getCenter().y;
    float hitboxX = bossCenterX - (HITBOX_WIDTH / 2.f);
    float hitboxY = bossCenterY - (HITBOX_HEIGHT / 2.f);
    sf::FloatRect finalHitbox({hitboxX, hitboxY}, {HITBOX_WIDTH, HITBOX_HEIGHT});

    // 시각 효과용 사각형의 위치와 크기를 미리 설정해 둡니다.
    m_visualEffectRect.setPosition(finalHitbox.position);
    m_visualEffectRect.setSize({finalHitbox.size.x, finalHitbox.size.y});

    m_helper.start({finalHitbox}, DAMAGE, MF_TIMINGS, MF_COLORS);
}

void MagneticField::update(float dt, Boss& boss, Player& player) {
    m_helper.update(dt, player, boss);

    if (!m_helper.isFinished()) {
        m_effectTimer += dt;
    }
}

void MagneticField::draw(sf::RenderTarget& target) {
    // 공격 활성 구간일 때는 헬퍼의 기본 그리기 대신, 직접 깜빡이는 효과를 그립니다.
    if (m_helper.isAttack()) {
        const sf::Color color1 = sf::Color(0, 0, 139, 200);
        const sf::Color color2 = sf::Color(0, 255, 255, 200);
        const float blinkInterval = 0.08f;

        bool showColor1 = std::fmod(m_effectTimer, blinkInterval * 2) < blinkInterval;

        m_visualEffectRect.setFillColor(showColor1 ? color1 : color2);
        target.draw(m_visualEffectRect);
    } 
    else {
        // 선딜레이나 후딜레이 구간에는 헬퍼의 기본 그리기를 사용합니다.
        m_helper.draw(target);
    }
}

bool MagneticField::isFinished() const {
    return m_helper.isFinished();
}