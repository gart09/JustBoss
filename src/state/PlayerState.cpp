#include "PlayerState.h"
#include "../entities/Player.h"
#include "../entities/Boss.h"
#include "../commands/PlayerCommand.h"
#include <iostream>

// --- IdleState ---
void IdleState::enter(Player& player) {
}

std::unique_ptr<IPlayerState> IdleState::handleInput(Player& player, ICommand* command) {
    if (dynamic_cast<AttackCommand*>(command)) 
        return std::make_unique<AttackState>();
    if (dynamic_cast<WeakPointAttackCommand*>(command)) 
        return std::make_unique<WeakAttackState>();
    if (dynamic_cast<StartChargeCommand*>(command)) 
        return std::make_unique<ChargingState>();
    if (dynamic_cast<JumpCommand*>(command)) 
        return std::make_unique<JumpState>();
    if (auto* move = dynamic_cast<MoveCommand*>(command)) {
        if (move->getDirection() != 0.0f) {
            player.move(move->getDirection());
            return std::make_unique<MoveState>();
        }
    }
    if (auto* dash = dynamic_cast<DashCommand*>(command)) 
        return std::make_unique<DashState>(dash->getDirection());
    return nullptr;
}

std::unique_ptr<IPlayerState> IdleState::update(Player& player, float dt) {
    return nullptr;
}


// --- MoveState ---
void MoveState::enter(Player& player) {
}

std::unique_ptr<IPlayerState> MoveState::handleInput(Player& player, ICommand* command) {
    if (dynamic_cast<AttackCommand*>(command)) return std::make_unique<AttackState>();
    if (dynamic_cast<WeakPointAttackCommand*>(command)) return std::make_unique<WeakAttackState>();
    if (dynamic_cast<StartChargeCommand*>(command)) return std::make_unique<ChargingState>();
    if (auto* jump = dynamic_cast<JumpCommand*>(command)) return std::make_unique<JumpState>();
    if (auto* move = dynamic_cast<MoveCommand*>(command)) {
        if (move->getDirection() == 0.0f) return std::make_unique<IdleState>();
        player.move(move->getDirection());
    }
    if (auto* dash = dynamic_cast<DashCommand*>(command)) return std::make_unique<DashState>(dash->getDirection());
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
        if(player.canDoubleJump()) {
            return std::make_unique<DoubleJumpState>(jump->getDirection());
        }
    }
    if (auto* dash = dynamic_cast<DashCommand*>(command)) 
        return std::make_unique<DashState>(dash->getDirection());
    return nullptr;
}

std::unique_ptr<IPlayerState> JumpState::update(Player& player, float dt) {
    if (player.isOnGround()) {
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
    if (auto* dash = dynamic_cast<DashCommand*>(command)) 
        return std::make_unique<DashState>(dash->getDirection());
    return nullptr;
}

std::unique_ptr<IPlayerState> DoubleJumpState::update(Player& player, float dt) {
    if (player.isOnGround()) {
        return std::make_unique<IdleState>();
    }
    return nullptr;
}

// --- AttackState ---
AttackState::AttackState() : m_timer(0.f), m_hasDealtDamage(false) {}
void AttackState::enter(Player& player) {
     m_timer = 0.f; 
     m_hasDealtDamage = false; 
     player.setVelocityX(0); 
}

std::unique_ptr<IPlayerState> AttackState::update(Player& player, float dt) {
    m_timer += dt;
    const auto& attackData = player.getAttackDataList()[0]; // 0번 인덱스 = 일반 찌르기
    if (m_timer >= attackData.attackActiveStart && m_timer <= attackData.attackActiveEnd) {
        sf::FloatRect worldHitbox = attackData.hitbox;
        sf::Vector2f playerPos = player.getPosition();
        if (player.getFacingDirection() == FacingDirection::Left) {
            worldHitbox.position.x = playerPos.x - attackData.hitbox.position.x - attackData.hitbox.size.x;
        } else {
            worldHitbox.position.x += playerPos.x;
        }
        worldHitbox.position.y += playerPos.y;
        player.setActiveHitbox(worldHitbox);
    } else {
        player.clearActiveHitbox();
    }
    if (m_timer >= attackData.duration) return std::make_unique<IdleState>();
    return nullptr;
}

std::unique_ptr<IPlayerState> AttackState::handleInput(Player& player, ICommand* command) {
    if (auto* dash = dynamic_cast<DashCommand*>(command)) return std::make_unique<DashState>(dash->getDirection());
    return nullptr;
}

// --- WeakAttackState ---
WeakAttackState::WeakAttackState() : m_timer(0.f), m_hasDealtDamage(false) {}

void WeakAttackState::enter(Player& player) { 
    m_timer = 0.f; 
    m_hasDealtDamage = false; 
    player.setVelocityX(0); 
}
std::unique_ptr<IPlayerState> WeakAttackState::update(Player& player, float dt) {
    m_timer += dt;
    const auto& attackData = player.getAttackDataList()[1]; // 1번 인덱스 = 약점 공격
    if (m_timer >= attackData.attackActiveStart && m_timer <= attackData.attackActiveEnd) {
        sf::FloatRect worldHitbox = attackData.hitbox;
        sf::Vector2f playerPos = player.getPosition();
        if (player.getFacingDirection() == FacingDirection::Left) {
            worldHitbox.position.x = playerPos.x - attackData.hitbox.position.x - attackData.hitbox.size.x;
        } else {
            worldHitbox.position.x += playerPos.x;
        }
        worldHitbox.position.y += playerPos.y;
        player.setActiveHitbox(worldHitbox);
    } else {
        player.clearActiveHitbox();
    }
    if (m_timer >= attackData.duration) return std::make_unique<IdleState>();
    return nullptr;
}

std::unique_ptr<IPlayerState> WeakAttackState::handleInput(Player& player, ICommand* command) {
    if (auto* dash = dynamic_cast<DashCommand*>(command)) return std::make_unique<DashState>(dash->getDirection());
    return nullptr;
}

// --- ChargingState ---
void ChargingState::enter(Player& player) { 
    m_chargeTimer = 0.f; 
    player.setVelocityX(0); 

}
std::unique_ptr<IPlayerState> ChargingState::update(Player& player, float dt) {
    m_chargeTimer += dt;
    return nullptr;
}
std::unique_ptr<IPlayerState> ChargingState::handleInput(Player& player, ICommand* command) {
    if (dynamic_cast<ReleaseChargeCommand*>(command)) {
        int attackIndex = 2; // 기본 1단계
        if (m_chargeTimer >= 1.5f) { // 1.5초 이상 충전 시 2단계
            attackIndex = 3;
        }
        const auto& attackData = player.getAttackDataList()[attackIndex]; // 차징공격 인덱스    
        sf::FloatRect worldHitbox = attackData.hitbox;
        sf::Vector2f playerPos = player.getPosition();
        if (player.getFacingDirection() == FacingDirection::Left) {
            worldHitbox.position.x = playerPos.x - attackData.hitbox.position.x - attackData.hitbox.size.x;
        } else {
            worldHitbox.position.x += playerPos.x;
        }
        worldHitbox.position.y += playerPos.y;
        player.setActiveHitbox(worldHitbox);
    }
    else {
        player.clearActiveHitbox();
    }
    if (auto* dash = dynamic_cast<DashCommand*>(command)) return std::make_unique<DashState>(dash->getDirection());
    return nullptr;
}

// --- DashState ---
DashState::DashState(float direction) : m_direction(direction), m_duration(0.2f), m_timer(0.f) {}
void DashState::enter(Player& player) {
    m_timer = 0.f;
    float dashDirection = m_direction;
    if (dashDirection == 0.f) dashDirection = (player.getFacingDirection() == FacingDirection::Right) ? 1.f : -1.f;
    player.setVelocityY(0);
    player.setVelocityX(dashDirection * 1500.f);
}

std::unique_ptr<IPlayerState> DashState::update(Player& player, float dt) {
    m_timer += dt;
    if (m_timer >= m_duration) {
        player.setVelocityX(0);
        if (player.isOnGround()) return std::make_unique<IdleState>();
        else return std::make_unique<JumpState>();
    }
    return nullptr;
}

std::unique_ptr<IPlayerState> DashState::handleInput(Player& player, ICommand* command) { 
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
    if (player.isOnGround()) { // 경직 중 착지하면 바로 Idle로
        return std::make_unique<IdleState>();
    }
    if (m_timer >= m_stunDuration) {
        return std::make_unique<JumpState>(); // 경직 후 공중 상태로 전환
    }
    return nullptr;
}