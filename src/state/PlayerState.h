#pragma once
#include "IPlayerState.h"
#include "../commands/ICommand.h"


// 대기 상태
class IdleState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void exit(Player& player) override;
};

// 이동 상태
class MoveState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void exit(Player& player) override;
};

// 점프 상태
class JumpState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void exit(Player& player) override;
};

class DoubleJumpState : public IPlayerState {
public:
    DoubleJumpState(float direction);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void exit(Player& player) override;
private:
    float m_direction;
};

// 대시
class DashState : public IPlayerState {
public:
    explicit DashState(float direction);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    void exit(Player& player) override;
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
    void exit(Player& player) override;
private:
    float m_stunDuration;
    float m_timer;
};

// --- 공격 상태들의 기반 클래스 ---
class BaseAttackState : public IPlayerState {
protected:
    float m_timer;
    const int m_attackIndex; // 사용할 공격 데이터의 인덱스

public:
    BaseAttackState(int attackIndex);
    void enter(Player& player) override;
    void exit(Player& player) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    
    std::optional<AttackInfo> getActiveAttackInfo(const Player& player) const;
};

// --- 구체적인 공격 상태 클래스들 ---
class AttackState : public BaseAttackState {
public:
    AttackState();
};

class WeakAttackState : public BaseAttackState {
public:
    WeakAttackState();
};

class ChargeAttackState : public BaseAttackState {
public:
    ChargeAttackState(int attackIndex);
};

class ChargingState : public IPlayerState {
public:
    void enter(Player& player) override;
    void exit(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
    std::optional<float> getChargeProgress() const override;

private:
    // 차지 버튼을 누르고 있는 시간을 기록하기 위한 타이머
    float m_chargeTimer;
    static constexpr float MAX_CHARGE_TIME = 2.0f; 
};