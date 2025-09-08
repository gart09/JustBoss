#include "PlayerState.h"
#include "../entities/Player.h"
#include "../entities/Boss.h"
#include "../commands/PlayerCommand.h"
#include <iostream>

// --- IdleState ---
void IdleState::enter(Player& player) {
    std::cout << "State: Idle" << std::endl;
}

std::unique_ptr<IPlayerState> IdleState::handleInput(Player& player, CommandType type)
{
    switch (type) {
        case CommandType::Jump:
            player.jump();
            return std::make_unique<JumpState>();

        case CommandType::Attack:
        case CommandType::WeakPointAttack:
            return std::make_unique<AttackState>(0);

        case CommandType::Dash:
            player.dash();
            return nullptr;

        case CommandType::Parry:
            return std::make_unique<ParryingState>(0.3f);

        default:
            return nullptr;
    }
}

std::unique_ptr<IPlayerState> IdleState::update(Player& player, float dt) {
    // 바닥에서 떨어지면 점프(낙하) 상태로
    if (player.m_shape.getPosition().y < 720.f - player.m_shape.getSize().y / 2.f) {
        player.m_canJump = false;
        return std::make_unique<JumpState>();
    }
    return nullptr;
}

// --- JumpingState ---
void JumpState::enter(Player& player) {
    player.takeJump();
}

std::unique_ptr<IPlayerState> JumpState::handleInput(Player& player, CommandType command) {
    switch(command)
    {
        case CommandType::Parry:
            return std::make_unique<ParryingState>(0.3f);
        case CommandType::Jump:
            return std::make_unique<DoubleJumpState>();
        default:
            return nullptr;
    }
    return nullptr;
}

std::unique_ptr<IPlayerState> JumpState::update(Player& player, float dt) {
    if (player.m_canJump) { // 착지했다면
        return std::make_unique<IdleState>();
    }
    return nullptr;
}

// --- DoubleJumpeState ---
void DoubleJumpState::enter(Player& player) {
    player.takeDoubleJump();
}

std::unique_ptr<IPlayerState> DoubleJumpState::handleInput(Player& player, CommandType command) {
    switch(command)
    {
        case CommandType::Parry:
            return std::make_unique<ParryingState>(0.3f);
        default:
            return nullptr;
    }
    return nullptr;
}

std::unique_ptr<IPlayerState> DoubleJumpState::update(Player& player, float dt) {
    if (player.m_canJump) { // 착지했다면
        return std::make_unique<IdleState>();
    }
    return nullptr;
}

// --- AttackState ---
AttackState::AttackState(int comboIndex)
    : m_comboIndex(comboIndex), m_timer(0.f), m_canChain(false), m_hasDealtDamage(false) {}

void AttackState::enter(Player& player) {
    std::cout << "State: Attack " << m_comboIndex + 1 << std::endl;
    player.m_velocity.x = 0;
}

std::unique_ptr<IPlayerState> AttackState::handleInput(Player& player, CommandType command) {
    if (m_canChain && command == CommandType::Attack || command == CommandType::WeakPointAttack) {
        if (m_comboIndex < player.getComboData().size() - 1) {
            return std::make_unique<AttackState>(m_comboIndex + 1);
        }
    }
    return nullptr;
}

std::unique_ptr<IPlayerState> AttackState::update(Player& player, float dt) {
    return nullptr;
}

// --- HitStunState ---
HitStunState::HitStunState(float duration) : m_stunDuration(duration), m_timer(0.f) {}
void HitStunState::enter(Player& player) {
    std::cout << "State: Hit Stun" << std::endl;
}
std::unique_ptr<IPlayerState> HitStunState::handleInput(Player& player, CommandType command) {
    return nullptr;
}
std::unique_ptr<IPlayerState> HitStunState::update(Player& player, float dt) {
    m_timer += dt;
    if (player.m_canJump) { // 경직 중 착지하면 바로 Idle로
        return std::make_unique<IdleState>();
    }
    if (m_timer >= m_stunDuration) {
        return std::make_unique<JumpState>(); // 경직 후 공중 상태로 전환
    }
    return nullptr;
}

// --- ParryingState ---
ParryingState::ParryingState(float duration) : m_parryWindowDuration(duration), m_timer(0.f) {}
void ParryingState::enter(Player& player) {
    std::cout << "State: Parrying" << std::endl;
    player.m_velocity.x = 0;
}
std::unique_ptr<IPlayerState> ParryingState::handleInput(Player& player, CommandType command) {
    return nullptr;
}
std::unique_ptr<IPlayerState> ParryingState::update(Player& player, float dt) {
    m_timer += dt;
    if (m_timer >= m_parryWindowDuration) {
        return std::make_unique<IdleState>();
    }
    return nullptr;
}

// --- ParrySuccessState ---
ParrySuccessState::ParrySuccessState(float duration) : m_stunDuration(duration), m_timer(0.f) {}
void ParrySuccessState::enter(Player& player) {
    std::cout << "State: Parry Success Stun" << std::endl;
}
std::unique_ptr<IPlayerState> ParrySuccessState::handleInput(Player& player, CommandType command) {
    return nullptr;
}
std::unique_ptr<IPlayerState> ParrySuccessState::update(Player& player, float dt) {
    m_timer += dt;
    if (player.m_canJump) {
        return std::make_unique<IdleState>();
    }
    if (m_timer >= m_stunDuration) {
        return std::make_unique<JumpState>();
    }
    return nullptr;
}

