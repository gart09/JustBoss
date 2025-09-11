#pragma once
#include "../bossPattern/IPattern.h"

class Rush : public IPattern {
private:
    float dashDuration = 1.5f;
    float timer = 0.0f;

public:
    Rush() { this->cooldown = 15.0f; }

    virtual void execute(Boss& boss, Player& player) override {
        IPattern::execute(boss, player);
        timer = 0.0f;
        // 약점 노출 로직 시작, 돌진 시작
    }

    virtual void update(float dt, Boss& boss, Player& player) override {
        IPattern::update(dt, boss, player); // 쿨타임 감소
        if (finished) return;

        timer += dt;
        // ... 돌진 로직 수행 ...
        if (timer >= dashDuration) {
            finished = true;
            // 약점 비활성화
        }
    }
    
    // 돌진은 특별한 사용 조건이 있음
    virtual bool canExecute(const Boss& boss, const Player& player) const override {
        if (!IPattern::canExecute(boss, player)) return false; // 기본 쿨타임 체크

        // 추가 조건: 플레이어와의 거리가 멀 때
        float distance = abs(boss.getPosition().x - player.getPosition().x);
        if (distance > 300.0f) { // 300.0f은 예시 값
             return true;
        }
        return false;
    }
};