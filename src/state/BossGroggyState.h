#pragma once
#include "IBossPhaseState.h"

class BossGroggyState : public IBossPhaseState {
public:
    BossGroggyState(float duration, PhaseID prevPhaseId);

    void enter(Boss& boss) override;
    void update(Boss& boss, float dt, Player& player) override;
    void exit(Boss& boss) override;
    void draw(sf::RenderTarget& target, Boss& boss) override;
    bool isGroggyState() const override { return true; }
    IPattern* getCurrentPattern() const override { return nullptr; }
    PhaseID getPhaseID() const override; 

private:
    float m_duration; 
    float m_timer;
    PhaseID m_prevPhaseId;
};