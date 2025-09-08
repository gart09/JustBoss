#include "PlayerState.h"
#include "../entities/Player.h"
#include "../entities/Boss.h"
#include "../commands/PlayerCommand.h"
#include <iostream>

// --- IdleState ---
void IdleState::enter(Player& player) {
}

std::unique_ptr<IPlayerState> IdleState::handleInput(Player& player, ICommand* command) {
    // 1. 이 커맨드가 JumpCommand인지 확인 (데이터 필요 없음)
    if (dynamic_cast<JumpCommand*>(command)) {
        return std::make_unique<JumpState>();
    }

    // 2. 이 커맨드가 MoveCommand인지 확인 (데이터 필요!)
    if (auto* move = dynamic_cast<MoveCommand*>(command)) {
        // dynamic_cast 성공! move는 이제 MoveCommand* 타입
        float dir = move->getDirection(); // getter로 데이터 추출
        if (dir != 0.0f) {
            player.move(dir); // 추출한 데이터로 행동 호출
            return std::make_unique<MoveState>();
        } else {
            player.move(dir); // 멈춤
            return nullptr; // 상태 유지
        }
    }

    // 3. 이 커맨드가 DashCommand인지 확인 (데이터 필요!)
    if (auto* dash = dynamic_cast<DashCommand*>(command)) {
        player.dash(dash->getDirection());
    }

    return nullptr;
}

std::unique_ptr<IPlayerState> IdleState::update(Player& player, float dt) {
    return nullptr;
}


// --- MoveState ---
void MoveState::enter(Player& player) {
}

std::unique_ptr<IPlayerState> MoveState::handleInput(Player& player, ICommand* command) {
    if (dynamic_cast<JumpCommand*>(command)) {
        return std::make_unique<JumpState>();
    }
    if (auto* move = dynamic_cast<MoveCommand*>(command)) {
        float dir = move->getDirection();
        if (dir != 0.0f) {
            player.move(dir);
            return nullptr; // 상태 유지
        } else {
            player.move(dir); // 멈춤
            return std::make_unique<IdleState>();
        }
    }
    if (auto* dash = dynamic_cast<DashCommand*>(command)) { 
        player.dash(dash->getDirection());
    }
    return nullptr;
}

std::unique_ptr<IPlayerState> MoveState::update(Player& player, float dt) {
    return nullptr;
}
// --- JumpingState ---
void JumpState::enter(Player& player) {
    player.takeJump();
}

std::unique_ptr<IPlayerState> JumpState::handleInput(Player& player, ICommand* command) {
    if(auto* jump = dynamic_cast<JumpCommand*>(command)) {
        if(player.m_canDoubleJump) {
            return std::make_unique<DoubleJumpState>(jump->getDirection());
        }
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
DoubleJumpState::DoubleJumpState(float direction) : m_direction(direction) {}

void DoubleJumpState::enter(Player& player) {
    player.takeDoubleJump(m_direction);
}

std::unique_ptr<IPlayerState> DoubleJumpState::handleInput(Player& player, ICommand* command) {
    return nullptr;
}

std::unique_ptr<IPlayerState> DoubleJumpState::update(Player& player, float dt) {
    if (player.m_canJump) {
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

std::unique_ptr<IPlayerState> AttackState::handleInput(Player& player, ICommand* command) {
    if (m_canChain && dynamic_cast<AttackCommand*>(command) || dynamic_cast<WeakPointAttackCommand*>(command)) {
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
std::unique_ptr<IPlayerState> HitStunState::handleInput(Player& player, ICommand* command) {
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