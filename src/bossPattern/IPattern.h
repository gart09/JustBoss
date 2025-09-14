#pragma once
#include "../entities/Boss.h"
#include "../entities/Player.h"

class IPattern {
protected:
    bool finished = false;
    float cooldown = 0.0f;
    float currentCooldown = 0.0f;
    bool isHitboxActive_ = false; // 현재 히트박스가 활성화 상태인지 여부
    sf::FloatRect activeHitbox_;  // 활성화된 히트박스의 사각형 정보

public:
    virtual ~IPattern() {}

    virtual void execute(Boss& boss, Player& player) { 
        finished = false; 
        currentCooldown = cooldown;
        isHitboxActive_ = false; // 패턴 시작 시에는 항상 비활성화
    }
    virtual void update(float dt, Boss& boss, Player& player) = 0;
    virtual void updateCooldown(float dt) {
        if (currentCooldown > 0) {
            currentCooldown -= dt;
        }
    }
    virtual void draw(sf::RenderTarget& target) = 0;
    virtual bool isFinished() const { return finished; }
    virtual bool canExecute(const Boss& boss, const Player& player) const = 0;
    virtual std::optional<sf::FloatRect> getWeakPointHitbox() const {
        return std::nullopt;
    }
    bool isHitboxActive() const { return isHitboxActive_; }
    const sf::FloatRect& getHitbox() const { return activeHitbox_; }
};