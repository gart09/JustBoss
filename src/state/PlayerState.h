#pragma once
#include "IPlayerState.h"
#include "../commands/ICommand.h"

// 대기 상태
class IdleState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};

// 이동 상태
class MoveState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};

// 점프/낙하 상태
class JumpState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};

class DoubleJumpState : public IPlayerState {
public:
    DoubleJumpState(float direction);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_direction;
};

// 일반 찌르기 공격
class AttackState : public IPlayerState {
public:
    AttackState();
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void notifyAttackHit() { m_hasDealtDamage = true; }
    bool hasDealtDamage() const { return m_hasDealtDamage; }
private:
    float m_timer;
    bool m_hasDealtDamage;
};

// 약점 공격
class WeakAttackState : public IPlayerState {
public:
    WeakAttackState();
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void notifyAttackHit() { m_hasDealtDamage = true; }
    bool hasDealtDamage() const { return m_hasDealtDamage; }
private:
    float m_timer;
    bool m_hasDealtDamage;
};

// 차지 중
class ChargingState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_chargeTimer;
};

// 대시
class DashState : public IPlayerState {
public:
    DashState(float direction);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_direction;
    float m_duration;
    float m_timer;
};

// 피격 경직 상태
class HitStunState : public IPlayerState {
public:
    explicit HitStunState(float duration);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_stunDuration;
    float m_timer;
};