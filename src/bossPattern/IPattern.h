#pragma once
#include "../entities/Boss.h"
#include "../entities/Player.h"

class IPattern {
protected:
    bool finished = true;
    float cooldown = 0.0f;
    float currentCooldown = 0.0f;

public:
    virtual ~IPattern() {}

    virtual void execute(Boss& boss, Player& player) { 
        finished = false; 
        currentCooldown = cooldown;
    }
    virtual void update(float dt, Boss& boss, Player& player) {
        // 모든 패턴은 기본적으로 쿨타임이 흐름
        if (currentCooldown > 0) {
            currentCooldown -= dt;
        }
    }
    virtual bool isFinished() const { return finished; }
    virtual bool canExecute(const Boss& boss, const Player& player) const {
        return currentCooldown <= 0;
    }
};