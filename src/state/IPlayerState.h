#pragma once
#include <memory>
#include "../commands/ICommand.h"
#include <optional>
#include <SFML/Graphics.hpp>

// 전방 선언
class Player;
class Boss;
class ICommand;

struct AttackInfo {
    sf::FloatRect hitbox;
    int damage;
};

/**
 * @brief 모든 플레이어 상태 클래스가 상속받는 인터페이스입니다.
 * 상태 패턴의 핵심으로, 상태별로 다른 행동을 정의합니다.
 */
class IPlayerState {
public:
    virtual ~IPlayerState() = default;

    // 상태에 진입할 때 한 번 호출됩니다.
    virtual void enter(Player& player) = 0;
    // 입력을 처리하고, 상태 전환이 필요하면 새로운 상태 객체를 반환합니다.
    virtual std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) = 0;
    // 매 프레임 호출되며, 상태 전환이 필요하면 새로운 상태 객체를 반환합니다.
    virtual std::unique_ptr<IPlayerState> update(Player& player, float dt) = 0;
    virtual void exit(Player& player) = 0;

    virtual std::optional<AttackInfo> getActiveAttackInfo(const Player& player) const { 
        return std::nullopt; 
    }
    virtual void notifyAttackHit() {}
    virtual std::optional<float> getChargeProgress() const {
        return std::nullopt;
    }
};

