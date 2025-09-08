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

// 3연타 공격 상태
class AttackState : public IPlayerState {
public:
    explicit AttackState(int comboIndex);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    int m_comboIndex;
    float m_timer;
    bool m_canChain; // 다음 콤보로 연계 가능한지 여부
    bool m_hasDealtDamage; // 이번 공격에서 데미지를 주었는지 (중복 방지)
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

class WeakPointAttackState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, ICommand* command) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};