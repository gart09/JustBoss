#pragma once
#include "../IPattern.h"
#include "../../bossPatternHelper/WarningAttackHelper.h"

// 보스 주변에 자기장을 생성하는 광역 패턴
class MagneticField : public IPattern {
public:
    MagneticField();
    
    bool canExecute(const Boss& boss, const Player& player) const override;
    void execute(Boss& boss, Player& player) override;
    void update(float dt, Boss& boss, Player& player) override;
    void draw(sf::RenderTarget& target) override;
    bool isFinished() const override;

private:
    // 패턴의 기본적인 실행(타이머, 판정)을 처리하는 헬퍼 객체
    WarningAttackHelper m_helper;

    // 이 패턴만의 고유한 '깜빡이는' 시각 효과를 위한 변수
    float m_effectTimer;
    sf::RectangleShape m_visualEffectRect;
};