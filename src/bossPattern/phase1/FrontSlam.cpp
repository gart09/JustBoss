#include "FrontSlam.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>
#include <algorithm>

namespace {
    constexpr float HITBOX_WIDTH = 200.f;
    constexpr float HITBOX_HEIGHT = 720.f;
    constexpr float WORLD_MIN_X = 0.f;
    constexpr float WORLD_MAX_X = 1280.f; 
    constexpr int DAMAGE = 10;
    const AttackTimings FS_TIMINGS = {1.f, 0.1f, 0.8f};
    const AttackColors FS_COLORS = {
        sf::Color(255, 165, 0, 100),
        sf::Color(139, 0, 0, 200)
    };
}

FrontSlam::FrontSlam() {
    cooldown = 4.0f;
    currentCooldown = 3.f;
}

bool FrontSlam::canExecute(const Boss& boss, const Player& player) const {
    float distance = abs(boss.getCenter().x - player.getCenter().x);
    float optimalDistance = 100.0f;
    return currentCooldown <= 0 && distance < optimalDistance;
}

void FrontSlam::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    std::cout << "Executing FrontSlam pattern!" << std::endl;

    // 패턴 실행 시점의 플레이어 위치를 기준으로 고정된 히트박스를 생성합니다.
    float playerCenterX = player.getCenter().x;
    float idealX = playerCenterX - (HITBOX_WIDTH / 2.f);
    
    // 히트박스가 월드 경계를 벗어나지 않도록 좌표를 보정(Clamp)합니다.
    float finalX = std::max(WORLD_MIN_X, idealX);
    finalX = std::min(finalX, WORLD_MAX_X - HITBOX_WIDTH);
    
    sf::FloatRect finalHitbox({finalX, 0.f}, {HITBOX_WIDTH, HITBOX_HEIGHT});

    m_helper.start({finalHitbox}, DAMAGE, FS_TIMINGS, FS_COLORS);
}

void FrontSlam::update(float dt, Boss& boss, Player& player) {
    m_helper.update(dt, player, boss);
}

void FrontSlam::draw(sf::RenderTarget& target) {
    m_helper.draw(target);
}

bool FrontSlam::isFinished() const {
    return m_helper.isFinished();
}