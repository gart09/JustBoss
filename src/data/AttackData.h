#pragma once
#include <SFML/Graphics.hpp>
#include <string>

/**
 * @brief 플레이어의 3연타 콤보 각 공격에 대한 정보를 담는 구조체입니다.
 * 데이터 기반 설계를 통해 밸런스 조절을 용이하게 합니다.
 */
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

/**
 * @brief 보스의 공격 정보를 담는 구조체입니다.
 */
struct BossAttackData {
    sf::FloatRect hitbox;       // 공격 범위 (보스 기준 상대 좌표)
    float damage;               // 공격력
    bool isReflectable = false; // 패링으로 반사 가능한 공격인지 여부
};

