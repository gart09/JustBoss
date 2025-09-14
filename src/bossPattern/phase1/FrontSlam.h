#pragma once
#include "../IPattern.h"
#include "../../bossPatternHelper/WarningAttackHelper.h" 

// 보스의 전방 내려찍기 패턴
class FrontSlam : public IPattern {
public:
    FrontSlam();

    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    bool canExecute(const Boss& boss, const Player& player) const override;
    bool isFinished() const override;
    void draw(sf::RenderTarget& target) override;

private:
    // 패턴의 실행(타이머, 시각 효과, 공격 판정)을 위임받아 처리하는 헬퍼 객체
    WarningAttackHelper m_helper;
};