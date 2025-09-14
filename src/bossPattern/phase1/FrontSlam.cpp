#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "FrontSlam.h"
#include "../../entities/Boss.h" // 보스 정보에 접근하기 위해 필요

constexpr float HITBOX_WIDTH = 200.f;
constexpr float HITBOX_HEIGHT = 720.f;
constexpr float WORLD_MIN_X = 0.f;
constexpr float WORLD_MAX_X = 1280.f; 
constexpr int DAMAGE = 10;
constexpr AttackTimings FS_TIMINGS = {1.f, 0.1f, 0.8f};
const AttackColors FS_COLORS = {
    sf::Color(255, 165, 0, 100), // 경고 색상 (주황)
    sf::Color(139, 0, 0, 200)    // 공격 색상 (검붉은색)
};


FrontSlam::FrontSlam() {
    cooldown = 4.0f;
    currentCooldown = 3.f;
}

bool FrontSlam::canExecute(const Boss& boss, const Player& player) const {
    float distance = abs(boss.getPosition().x + boss.getSize() / 2 - (player.getPosition().x + player.getSize() / 2)); // x축 거리만 계산
    float optimalDistance = 100.0f; // 보스가 유지하려는 최적 거리
    return currentCooldown <= 0 && distance < optimalDistance;
}

void FrontSlam::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    boss.setVelocity({0.f, 0.f});
    std::cout << "Executing FrontSlam pattern!" << std::endl;

    // 1. 'execute'가 호출된 시점의 정보를 바탕으로 히트박스를 '한 번만' 계산합니다.
    float playerCenterX = player.getPosition().x + player.getSize() / 2.f;
    float idealX = playerCenterX - (HITBOX_WIDTH / 2.f);
    
    // 월드 경계값을 벗어나지 않도록 좌표를 보정(Clamp)
    float finalX = std::max(WORLD_MIN_X, idealX);
    finalX = std::min(finalX, WORLD_MAX_X - HITBOX_WIDTH);
    
    // 최종 히트박스 생성
    sf::FloatRect finalHitbox({finalX, 0.f}, {HITBOX_WIDTH, HITBOX_HEIGHT});

    // 2. 계산된 최종 정보를 Helper에게 넘겨주고 모든 실행을 위임합니다.
    // FrontSlam은 히트박스가 하나뿐이므로, {finalHitbox} 처럼 vector에 하나만 담아서 전달합니다.
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