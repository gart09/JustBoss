#pragma once
#include "IBossPhaseState.h"
#include "../bossPattern/IPattern.h"
#include <vector>
#include <memory>

class BossPhase1State : public IBossPhaseState {
public:
    BossPhase1State();
    void enter(Boss& boss) override; // 상태 진입 시 초기화
    void update(Boss& boss, float dt, Player& player) override;
    void exit(Boss& boss) override; // 상태 종료 시 정리
    void draw(sf::RenderTarget& target) override;
private:
    std::vector<std::unique_ptr<IPattern>> generalPatterns;
    std::vector<std::unique_ptr<IPattern>> specialPatterns;

    IPattern* currentPattern = nullptr;
    float thinkTimer = 0.0f;
    float THINK_DELAY = 2.0f; // 패턴 선택 간격
    
    IPattern* choosePattern(Boss& boss, Player& player);
};