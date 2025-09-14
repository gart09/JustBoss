#pragma once
#include "IBossPhaseState.h"
#include "../bossPattern/IPattern.h"
#include <vector>
#include <memory>

class BossPhase1State : public IBossPhaseState {
public:
    BossPhase1State();
    void enter(Boss& boss) override;
    void update(Boss& boss, float dt, Player& player) override;
    void exit(Boss& boss) override;
    void draw(sf::RenderTarget& target, Boss& boss) override;
    IPattern* getCurrentPattern() const override { return m_currentPattern; }
    PhaseID BossPhase1State::getPhaseID() const { return PhaseID::Phase1; }

private:
    std::vector<std::unique_ptr<IPattern>> m_generalPatterns;
    std::vector<std::unique_ptr<IPattern>> m_specialPatterns;

    IPattern* m_currentPattern = nullptr;
    float m_thinkTimer = 0.0f;
    float m_thinkDelay = 2.0f;
    float m_decisionTimer;
    static constexpr float DECISION_INTERVAL = 0.5f;
    
    IPattern* choosePattern(Boss& boss, Player& player);
};