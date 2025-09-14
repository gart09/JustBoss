#pragma once
#include "../entities/Boss.h"
#include "../entities/Player.h"
#include <optional> // std::optional 사용

// 모든 보스 공격 패턴의 기반이 되는 인터페이스 클래스
class IPattern {
protected:
    // --- 상태 변수 ---
    bool finished = true;           // 패턴이 완전히 종료되었는지 여부
    float cooldown = 0.0f;          // 이 패턴의 최대 쿨타임
    float currentCooldown = 0.0f;   // 현재 남은 쿨타임
    
    // --- 실시간 판정 정보 ---
    bool isHitboxActive_ = false;   // 현재 프레임에 공격 판정이 활성화되었는지 여부
    sf::FloatRect activeHitbox_;    // 활성화된 공격 판정의 범위

public:
    // 가상 소멸자 (자식 클래스가 안전하게 소멸될 수 있도록 보장)
    virtual ~IPattern() {}

    // 패턴이 시작될 때 공통적으로 호출되는 초기화 함수
    virtual void execute(Boss& boss, Player& player) { 
        finished = false; 
        currentCooldown = cooldown;
        isHitboxActive_ = false;
    }

    // 매 프레임 호출되는 패턴의 핵심 로직 (자식 클래스에서 반드시 구현)
    virtual void update(float dt, Boss& boss, Player& player) = 0;

    // 매 프레임 호출되는 쿨타임 감소 로직
    virtual void updateCooldown(float dt) {
        if (currentCooldown > 0) {
            currentCooldown -= dt;
        }
    }

    // 패턴의 시각 효과를 그리는 함수 (자식 클래스에서 반드시 구현)
    virtual void draw(sf::RenderTarget& target) = 0;

    // 패턴의 종료 여부를 반환하는 함수
    virtual bool isFinished() const { return finished; }

    // 현재 패턴을 사용할 수 있는 조건인지 확인하는 함수 (자식 클래스에서 반드시 구현)
    virtual bool canExecute(const Boss& boss, const Player& player) const = 0;

    // 패턴이 약점을 노출하는 경우, 그 범위를 반환하는 함수
    virtual std::optional<sf::FloatRect> getWeakPointHitbox() const {
        return std::nullopt; // 기본적으로는 약점이 없음
    }

    // 현재 공격 판정이 활성화되었는지 여부를 반환
    bool isHitboxActive() const { return isHitboxActive_; }

    // 현재 활성화된 공격 판정의 범위를 반환
    const sf::FloatRect& getHitbox() const { return activeHitbox_; }
};