#pragma once
#include "../IPattern.h"
#include "../../bossPatternHelper/WarningAttackHelper.h" 


class FrontSlam : public IPattern {
private:
    WarningAttackHelper m_helper;

public:
    FrontSlam();
    virtual void execute(Boss& boss, Player& player) override;
    virtual void update(float dt, Boss& boss, Player& player) override;
    virtual bool canExecute(const Boss& boss, const Player& player) const override;
    bool isFinished() const override;
    void draw(sf::RenderTarget& target) override;
};