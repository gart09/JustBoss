#include "PlayerState.h"
#include "../entities/Player.h"
#include "../entities/Boss.h"
#include "../commands/PlayerCommand.h"
#include <iostream>

// --- IdleState ---
void IdleState::enter(Player& player) {
    player.move(0.f);
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

void IdleState::exit(Player& player) {
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

void MoveState::exit(Player& player) {
}

// --- JumpingState ---
void JumpState::enter(Player& player) {
    player.takeJump();
}

std::unique_ptr<IPlayerState> JumpState::handleInput(Player& player, ICommand* command) {
    if (dynamic_cast<AttackCommand*>(command)) 
        return std::make_unique<AttackState>();
    if (dynamic_cast<WeakPointAttackCommand*>(command)) 
        return std::make_unique<WeakAttackState>();
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

void JumpState::exit(Player& player) {
}

// --- DoubleJumpeState ---
DoubleJumpState::DoubleJumpState(float direction) : m_direction(direction) {}

void DoubleJumpState::enter(Player& player) {
    player.takeDoubleJump(m_direction);
}

std::unique_ptr<IPlayerState> DoubleJumpState::handleInput(Player& player, ICommand* command) {
    if (auto* dash = dynamic_cast<DashCommand*>(command)) 
        return std::make_unique<DashState>(dash->getDirection());
    if (dynamic_cast<AttackCommand*>(command)) 
        return std::make_unique<AttackState>();
    if (dynamic_cast<WeakPointAttackCommand*>(command)) 
        return std::make_unique<WeakAttackState>();
    return nullptr;
}

std::unique_ptr<IPlayerState> DoubleJumpState::update(Player& player, float dt) {
    if (player.isOnGround()) {
        return std::make_unique<IdleState>();
    }
    return nullptr;
}

void DoubleJumpState::exit(Player& player) {
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

void DashState::exit(Player& player) {
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

void HitStunState::exit(Player& player) {
}

// --- BaseAttackState (공격 상태 공통 로직) ---
BaseAttackState::BaseAttackState(int attackIndex)
    : m_timer(0.f), m_hasDealtDamage(false), m_attackIndex(attackIndex) {}

void BaseAttackState::enter(Player& player) {
    m_timer = 0.f;
    m_hasDealtDamage = false;
    player.setVelocityX(0); // 공격 중에는 수평 이동을 멈춤
}

void BaseAttackState::exit(Player& player) {
    player.clearActiveHitbox(); // 상태가 종료될 때 히트박스를 확실히 제거
}

std::unique_ptr<IPlayerState> BaseAttackState::update(Player& player, float dt) {
    m_timer += dt;

    // 공격 상태의 전체 지속 시간이 끝났는지 확인한다.
    const auto& attackData = player.getAttackDataList()[m_attackIndex];
    if (m_timer >= attackData.getTotalDuration()) {
        if (player.isOnGround())
            return std::make_unique<IdleState>();
        else{
            return std::make_unique<JumpState>();
        }
    }
    // 아직 공격이 진행 중이면 현재 상태를 유지한다 (nullptr 반환).
    return nullptr;
}

std::unique_ptr<IPlayerState> BaseAttackState::handleInput(Player& player, ICommand* command) {
    if (auto* dash = dynamic_cast<DashCommand*>(command)) {
        return std::make_unique<DashState>(dash->getDirection());
    }
    return nullptr;
}

std::optional<AttackInfo> BaseAttackState::getActiveAttackInfo(const Player& player) const {
    const auto& attackData = player.getAttackDataList()[m_attackIndex];
    const float preDelayEnd = attackData.preDelay;
    const float activePhaseEnd = preDelayEnd + attackData.activeDuration;

    // 타이머가 공격의 '액티브' 구간에 있을 때만 공격 정보를 반환
    if (m_timer >= preDelayEnd && m_timer < activePhaseEnd) {
        sf::FloatRect worldHitbox = attackData.hitbox;
        sf::Vector2f playerPos = player.getPosition();
        
        if (player.getFacingDirection() == FacingDirection::Left) {
            worldHitbox.position.x = playerPos.x - attackData.hitbox.size.x;
        } else {
            worldHitbox.position.x += playerPos.x;
        }
        worldHitbox.position.y += playerPos.y;

        return AttackInfo{worldHitbox, attackData.damage};
    }
    
    return std::nullopt;
}

// BaseAttackState에 notifyAttackHit 구현 추가
void BaseAttackState::notifyAttackHit() {
    // Player로부터 공격이 적중했다는 알림을 받으면 플래그를 true로 설정
    m_hasDealtDamage = true;
}


// --- AttackState (일반 공격) ---
AttackState::AttackState() : BaseAttackState(0) {} // 0번 인덱스 = 일반 찌르기


// --- WeakAttackState (약점 공격) ---
WeakAttackState::WeakAttackState() : BaseAttackState(1) {} // 1번 인덱스 = 약점 공격


// --- ChargeAttackState (차지 공격) ---
ChargeAttackState::ChargeAttackState(int attackIndex) : BaseAttackState(attackIndex) {}


// --- ChargingState (차지 중 상태) ---
void ChargingState::enter(Player& player) { 
    m_chargeTimer = 0.f; 
    player.setVelocityX(0); 
}

void ChargingState::exit(Player& player) {} // 추가

std::unique_ptr<IPlayerState> ChargingState::update(Player& player, float dt) {
    m_chargeTimer += dt;
    // 차지 이펙트 등을 여기서 업데이트 할 수 있음
    return nullptr;
}

std::unique_ptr<IPlayerState> ChargingState::handleInput(Player& player, ICommand* command) {
    // 차지 중 공격 키를 떼면 차지 공격 상태로 전환
    if (dynamic_cast<ReleaseChargeCommand*>(command)) {
        int attackIndex;
        if(m_chargeTimer < MAX_CHARGE_TIME){
            attackIndex = 2; // 기본 1단계 차지 공격 인덱스
            std::cout << "Charge time: " << m_chargeTimer << " seconds, " << "Charge 1" << std::endl;
        }
        else{ 
            attackIndex = 3;
            std::cout << "Charge time: " << m_chargeTimer << " seconds, " << "Charge 2" << std::endl;
        }
        return std::make_unique<ChargeAttackState>(attackIndex);
    }

    // 차지 중 대쉬로 캔슬 가능
    if (auto* dash = dynamic_cast<DashCommand*>(command)) {
        return std::make_unique<DashState>(dash->getDirection());
    }

    return nullptr;
}

std::optional<float> ChargingState::getChargeProgress() const {
    // 현재 시간을 최대 시간으로 나누어 진행도(%)를 계산
    // std::min을 사용해 1.0f (100%)를 넘지 않도록 함
    return std::min(m_chargeTimer / MAX_CHARGE_TIME, 1.0f);
}