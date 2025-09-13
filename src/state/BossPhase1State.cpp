#pragma once
#include "../state/BossPhase1State.h"
#include "../bossPattern/phase1/Rush.h"
#include "../bossPattern/phase1/FrontSlam.h"
#include "../bossPattern/phase1/GroundSlam.h"
#include "../bossPattern/phase1/MagneticField.h"
#include "../bossPattern/phase1/TripleSlam.h"

constexpr float BOSS_SIZE = 200.f;
constexpr float CHAR_SIZE = 50.f;

BossPhase1State::BossPhase1State() {
    // 1페이즈에서 사용할 패턴 객체들을 생성
    generalPatterns.push_back(std::make_unique<FrontSlam>());
    //generalPatterns.push_back(std::make_unique<GroundSlam>());

    //specialPatterns.push_back(std::make_unique<MagneticField>());
    //specialPatterns.push_back(std::make_unique<Rush>());
    //specialPatterns.push_back(std::make_unique<TripleSlam>());
}

void BossPhase1State::enter(Boss& boss)
{
    // 이 상태에 처음 진입했을 때 할 일을 여기에 작성합니다.
    std::cout << "Boss enters Phase 1: Wandering peacefully..." << std::endl;
}

void BossPhase1State::draw(sf::RenderTarget& target) {
    if (currentPattern) {
        currentPattern->draw(target); // 현재 실행 중인 패턴의 draw 함수 호출
    }
}

void BossPhase1State::update(Boss& boss, float dt, Player& player) {
    for (const auto& pattern : generalPatterns) {
        pattern->updateCooldown(dt); // 쿨타임만 감소시키는 함수를 호출 (아래 추가 설명 참조)
    }
    for (const auto& pattern : specialPatterns) {
        pattern->updateCooldown(dt);
    }
    // 1. 현재 패턴을 실행 중인 경우
    if (currentPattern) {
        currentPattern->update(dt, boss, player);
        if (currentPattern->isFinished()) {
            currentPattern = nullptr; // 패턴 종료
            thinkTimer = THINK_DELAY; // 다음 행동 전까지 잠시 대기
        }
        return;
    }
    thinkTimer -= dt;
    if (thinkTimer > 0) {
        // <<< 이동 로직 추가 부분 시작 >>>
        float bossCenterX = boss.getPosition().x + BOSS_SIZE / 2;
        float playerCenterX = player.getPosition().x + CHAR_SIZE / 2;
        float distance = bossCenterX - playerCenterX;
        float optimalDistance = 100.0f; // 보스가 유지하려는 최적 거리 (예시)
        float moveSpeed = 100.0f;

        if (abs(distance) > optimalDistance) {
            // 너무 멀면 플레이어에게 다가감
            boss.move( (distance > 0 ? -1 : 1) * moveSpeed * dt, 0);
        } else {
            // 최적 거리라면 가만히 있거나 좌우로 살짝씩 배회 (Wandering)
            boss.wander(dt, moveSpeed);
        }
        // <<< 이동 로직 추가 부분 끝 >>>
        return; // 아직 생각할 시간이 남았으므로 패턴 선택은 하지 않음
    }

    // 3. 대기가 끝나면 다음 패턴 선택 (기존과 동일)
    currentPattern = choosePattern(boss, player);
    if (currentPattern) {
        currentPattern->execute(boss, player);
    } else {
        // 만약 실행할 패턴이 없다면 다시 대기
        thinkTimer = 1.0f; 
    }
}

void BossPhase1State::exit(Boss& boss)
{
    // 다른 상태로 전환되기 직전에 할 일을 여기에 작성합니다.
    std::cout << "Boss exits Phase 1." << std::endl;
}

// 🤖 여기가 1페이즈 AI의 핵심입니다.
IPattern* BossPhase1State::choosePattern(Boss& boss, Player& player) {
    // 특수 기술 사용 조건 확인 (우선순위 높음)
    std::vector<IPattern*> availableSpecialsPatterns;
    for(auto& pattern : specialPatterns) {
        if (pattern->canExecute(boss, player)) {
            availableSpecialsPatterns.push_back(pattern.get());
        }
    }
    if(!availableSpecialsPatterns.empty()) {
        // 우선순위가 높은 특수 패턴 중 하나를 선택
        int randomIndex = rand() % availableSpecialsPatterns.size();
        return availableSpecialsPatterns[randomIndex];
    }

    // 일반 기술 중 랜덤 선택
    // (이미 사용한 기술은 3초 쿨타임 때문에 canExecute에서 걸러짐)
    std::vector<IPattern*> availableGeneralPatterns;
    for (const auto& pattern : generalPatterns) {
        if (pattern->canExecute(boss, player)) {
            availableGeneralPatterns.push_back(pattern.get());
        }
    }

    if (!availableGeneralPatterns.empty()) {
        int randomIndex = rand() % availableGeneralPatterns.size();
        std::cout << "Boss chooses a general pattern to execute:" << randomIndex << std::endl;
        return availableGeneralPatterns[randomIndex];
    }
    
    return nullptr; // 마땅한 패턴이 없으면 아무것도 안함
}
