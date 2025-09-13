#pragma once
#include "../IPattern.h"
#include "../../bossPatternHelper/WarningAttackHelper.h"

class GroundSlam : public IPattern {
public:
    GroundSlam();
    
    bool canExecute(const Boss& boss, const Player& player) const override;
    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    void draw(sf::RenderTarget& target) override;
    bool isFinished() const override;

private:
    WarningAttackHelper m_helper;
};