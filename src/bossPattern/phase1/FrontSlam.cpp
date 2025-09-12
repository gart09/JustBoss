#pragma once
#include <SFML/Graphics.hpp>
#include "FrontSlam.h"
#include "../../entities/Boss.h" // 보스 정보에 접근하기 위해 필요

FrontSlam::FrontSlam() {
    cooldown = 3.0f; // 기획서대로 일반 기술 쿨타임 3초
}

bool FrontSlam::canExecute(const Boss& boss, const Player& player) const {
    float distance = abs(boss.getPosition().x - player.getPosition().x); // x축 거리만 계산
    float optimalDistance = 50.0f; // 보스가 유지하려는 최적 거리
    return currentCooldown <= 0 && distance < optimalDistance;
}

void FrontSlam::execute(Boss& boss, Player& player) {
    IPattern::execute(boss, player); // 기본 execute 호출 (finished, cooldown 설정)
    patternTimer = 0.0f; // 패턴 타이머 초기화
    currentCooldown = cooldown; // 쿨타임 설정
    std::cout << "Executing FrontSlam pattern!" << std::endl;
}

void FrontSlam::update(float dt, Boss& boss, Player& player) {
    IPattern::update(dt, boss, player); // 기본 쿨타임 감소
    if (finished) return;

    patternTimer += dt;

    // 1. 선딜레이(Wind-up) 구간
    if (patternTimer < WIND_UP_DURATION) {
        // 아직 공격 판정 없음
        isHitboxActive_ = false;
    }
    // 2. 공격(Active) 구간
    else if (patternTimer < WIND_UP_DURATION + ACTIVE_DURATION) {
        // 공격 판정 활성화!
        isHitboxActive_ = true;
        float bossX = boss.getPosition().x;
        float playerX = player.getPosition().x;
        float direction = (playerX > bossX) ? 1.0f : -1.0f;
        sf::Vector2f bossPos = boss.getPosition();
        
        // 보스 앞쪽으로 히트박스 생성 (크기와 위치는 임의로 설정)
        float hitboxX = (direction > 0) ? bossPos.x : bossPos.x - 150.f;
        activeHitbox_ = sf::FloatRect({hitboxX, bossPos.y - 200.f}, {150.f, 200.f});
        // boss.playAnimation("front_slam_active"); // 공격 애니메이션 재생
    }
    // 3. 후딜레이(Recovery) 및 패턴 종료
    else {
        isHitboxActive_ = false; // 공격 판정 비활성화
        // boss.playAnimation("front_slam_recovery"); // 마무리 애니메이션
        if (patternTimer >= WIND_UP_DURATION + ACTIVE_DURATION + RECOVERY_DURATION) {
            finished = true; // 패턴 완전히 종료
        }
    }
}