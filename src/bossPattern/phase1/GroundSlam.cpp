#include "GroundSlam.h"
#include "../../entities/Player.h"
#include "../../entities/Boss.h"
#include <iostream>
#include <algorithm>

constexpr float HITBOX_WIDTH = 500.f;
constexpr float HITBOX_HEIGHT = 50.f;
constexpr float WORLD_MIN_X = 0.f;
constexpr float WORLD_MAX_X = 1280.f; 
constexpr float GROUND_Y = 620.f;
constexpr int DAMAGE = 15;

const AttackTimings GS_TIMINGS = {1.2f, 0.1f, 1.0f};
const AttackColors GS_COLORS = {
    sf::Color(255, 165, 0, 100), // 경고 색상 (주황)
    sf::Color(139, 0, 0, 200)    // 공격 색상 (검붉은색)
};

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

    float playerCenterX = player.getPosition().x + player.getSize() / 2.f;
    
    // GroundSlam의 히트박스 너비(HITBOX_WIDTH)를 사용하여 이상적인 X좌표 계산
    float idealX = playerCenterX - (HITBOX_WIDTH / 2.f); 
    
    // 월드 경계를 벗어나지 않도록 좌표를 보정
    float finalX = std::max(WORLD_MIN_X, idealX);
    finalX = std::min(finalX, WORLD_MAX_X - HITBOX_WIDTH);
    
    // 히트박스가 바닥에 붙어있도록 Y좌표를 계산
    float hitboxY = GROUND_Y - HITBOX_HEIGHT;
    
    // 최종 히트박스 생성
    sf::FloatRect finalHitbox({finalX, hitboxY}, {HITBOX_WIDTH, HITBOX_HEIGHT});

    // 계산된 정보를 Helper에게 넘겨주고 실행 시작
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