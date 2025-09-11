#pragma once
#include "../IPattern.h"

class FrontSlam : public IPattern {
private:
    float patternTimer_ = 0.0f; // 패턴 내부의 시간을 재는 타이머

    // 패턴의 각 단계별 시간 (조정 가능)
    const float WIND_UP_DURATION = 0.5f; // 선딜레이 (준비 동작)
    const float ACTIVE_DURATION = 0.3f;  // 공격 판정 활성화 시간
    const float RECOVERY_DURATION = 0.5f;// 후딜레이 (회수 동작)

public:
    FrontSlam();
    virtual void execute(Boss& boss, Player& player) override;
    virtual void update(float dt, Boss& boss, Player& player) override;
};