#pragma once
#include "Rush.h"
#include "../../entities/Boss.h"
#include "../../entities/Player.h" // 플레이어 위치에 접근하기 위해 필요

Rush::Rush() {
    cooldown = 15.0f; // 기획서의 쿨타임 15초
}

// 🤖 돌진은 특별한 사용 조건을 가집니다.
bool Rush::canExecute(const Boss& boss, const Player& player) const {
    // 1. 기본 쿨타임이 되지 않았으면 실행 불가
    if (currentCooldown > 0) return false;

    // 2. 플레이어와 거리가 일정 이상 멀어지면 쿨타임과 상관없이 실행 가능 (0초 쿨타임 조건)
    float distance = std::abs(boss.getPosition().x - player.getPosition().x);
    if (distance > 300.0f) { // 예시: 400px 이상 멀어지면 발동
        return true;
    }
    
    // 3. 위 조건이 아니라면, 기본 쿨타임이 다 되었을 때만 실행 가능 (15초 쿨타임 조건)
    return currentCooldown <= 0;
}

void Rush::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player);
    
    // 상태와 타이머 초기화
    stateTimer_ = 0.0f;

    // 1. 목표 지점 계산: 플레이어를 지나쳐 반대편까지 가도록 설정
    float bossX = boss.getPosition().x;
    float playerX = player.getPosition().x;
    float direction = (playerX > bossX) ? 1.0f : -1.0f;

    const float LEFT_WALL_X = 0.0f;
    const float RIGHT_WALL_X = 1280.0f;
    const float BOSS_WIDTH = 250.f;


    // 바라보는 방향에 따라 벽 끝을 목표 지점으로 설정
    if (direction > 0) { // 오른쪽을 보고 있을 때
        targetPosition_ = sf::Vector2f({RIGHT_WALL_X - BOSS_WIDTH / 2, boss.getPosition().y}); // 보스 크기 절반만큼 안쪽으로
    } else { // 왼쪽을 보고 있을 때
        targetPosition_ = sf::Vector2f({LEFT_WALL_X + BOSS_WIDTH / 2, boss.getPosition().y});
    }

    // 2. 약점 노출: 보스 객체에 약점 상태를 활성화시킵니다.
    // boss.exposeWeakPoint("하단"); // Boss 클래스에 이런 함수가 있다고 가정
    
    // 3. 경고 표시: 돌진 경로를 미리 보여줍니다.
}

void Rush::update(float dt, Boss& boss, Player& player) {
}