#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// 공격 패턴의 데이터를 담는 구조체
struct AttackData {
    int damage;           // 공격력
    float preDelay;         // 선딜레이 시간
    float activeDuration;   // 공격 판정 지속 시간
    float postDelay;        // 후딜레이 시간
    sf::FloatRect hitbox;    // 공격 범위 (플레이어 기준 상대 위치)

    float getTotalDuration() const {
        return preDelay + activeDuration + postDelay;
    }
};