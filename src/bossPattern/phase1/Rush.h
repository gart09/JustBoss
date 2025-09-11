#pragma once
#include "../IPattern.h"

class Rush : public IPattern {
private:
    float dashDuration = 1.5f;
    float timer = 0.0f;

    float stateTimer_ = 0.0f; // 각 상태의 지속 시간을 재는 타이머

    sf::Vector2f targetPosition_; // 돌진 목표 지점

    const float WIND_UP_DURATION = 0.8f;  // 선딜레이
    const float DASH_SPEED = 1200.0f;     // 돌진 속도 (매우 빠르게)
    const float RECOVERY_DURATION = 0.5f; // 후딜레이

public:
    Rush();

    virtual bool canExecute(const Boss& boss, const Player& player) const override;
    virtual void execute(Boss& boss, Player& player) override;
    virtual void update(float dt, Boss& boss, Player& player) override;
};