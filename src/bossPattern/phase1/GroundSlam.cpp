#include "GroundSlam.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>
#include <algorithm>

namespace {
    constexpr float HITBOX_WIDTH = 500.f;
    constexpr float HITBOX_HEIGHT = 50.f;
    constexpr float WORLD_MIN_X = 0.f;
    constexpr float WORLD_MAX_X = 1280.f; 
    constexpr float GROUND_Y = 620.f;
    constexpr int DAMAGE = 15;
    const AttackTimings GS_TIMINGS = {1.2f, 0.1f, 1.0f};
    const AttackColors GS_COLORS = {
        sf::Color(255, 165, 0, 100),
        sf::Color(139, 0, 0, 200)
    };
}

GroundSlam::GroundSlam() {
    cooldown = 4.0f;
    currentCooldown = 3.0f;
}

bool GroundSlam::canExecute(const Boss& boss, const Player& player) const {
    return currentCooldown <= 0;
}

void GroundSlam::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    std::cout << "Executing GroundSlam pattern!" << std::endl;

    // 패턴 실행 시점의 플레이어 위치를 기준으로 히트박스를 생성합니다.
    float playerCenterX = player.getCenter().x;
    float idealX = playerCenterX - (HITBOX_WIDTH / 2.f); 
    
    // 히트박스가 월드 경계를 벗어나지 않도록 좌표를 보정합니다.
    float finalX = std::max(WORLD_MIN_X, idealX);
    finalX = std::min(finalX, WORLD_MAX_X - HITBOX_WIDTH);
    
    // 히트박스가 바닥에 붙어있도록 Y좌표를 계산합니다.
    float hitboxY = GROUND_Y - HITBOX_HEIGHT;
    
    sf::FloatRect finalHitbox({finalX, hitboxY}, {HITBOX_WIDTH, HITBOX_HEIGHT});

    m_helper.start({finalHitbox}, DAMAGE, GS_TIMINGS, GS_COLORS);
}

void GroundSlam::update(float dt, Boss& boss, Player& player) {
    m_helper.update(dt, player, boss);
}

void GroundSlam::draw(sf::RenderTarget& target) {
    m_helper.draw(target);
}

bool GroundSlam::isFinished() const {
    return m_helper.isFinished();
}