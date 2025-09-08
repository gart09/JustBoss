#pragma once
#include "IPlayerState.h"
#include "../commands/CommandType.h"

// 대기 상태
class IdleState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};

// 점프/낙하 상태
class JumpState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};

class DoubleJumpState : public IPlayerState {
public:
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
};

// 3연타 공격 상태
class AttackState : public IPlayerState {
public:
    explicit AttackState(int comboIndex);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
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
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_stunDuration;
    float m_timer;
};

// 패링 시도 상태
class ParryingState : public IPlayerState {
public:
    explicit ParryingState(float duration);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_parryWindowDuration;
    float m_timer;
};

// 패링 성공 후 경직 상태
class ParrySuccessState : public IPlayerState {
public:
    explicit ParrySuccessState(float duration);
    void enter(Player& player) override;
    std::unique_ptr<IPlayerState> handleInput(Player& player, CommandType type) override;
    std::unique_ptr<IPlayerState> update(Player& player, float dt) override;
private:
    float m_stunDuration;
    float m_timer;
};

