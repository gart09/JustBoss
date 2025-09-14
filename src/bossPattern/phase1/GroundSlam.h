#pragma once
#include "../IPattern.h"
#include "../../bossPatternHelper/WarningAttackHelper.h"

// 바닥을 내리쳐 공격하는 패턴
class GroundSlam : public IPattern {
public:
    GroundSlam();
    
    bool canExecute(const Boss& boss, const Player& player) const override;
    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    void draw(sf::RenderTarget& target) override;
    bool isFinished() const override;

private:
    // 패턴의 실행을 위임받아 처리하는 헬퍼 객체
    WarningAttackHelper m_helper;
};