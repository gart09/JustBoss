#pragma once
#include "../IPattern.h"
#include "../../bossPatternHelper/WarningAttackHelper.h"

class MagneticField : public IPattern {
public:
    MagneticField();
    
    // IPattern으로부터 상속받은 함수들을 재정의
    bool canExecute(const Boss& boss, const Player& player) const override;
    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    void draw(sf::RenderTarget& target) override;
    bool isFinished() const override;

private:
    WarningAttackHelper m_helper;
    float m_effectTimer;
    sf::RectangleShape m_visualEffectRect;
};