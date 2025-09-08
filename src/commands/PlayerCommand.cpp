#include "PlayerCommand.h"
#include "../entities/Player.h"

// --- MoveCommand ---
MoveCommand::MoveCommand(float direction) : m_direction(direction) {}

void MoveCommand::execute(Player& player) {
    // 플레이어 객체의 move 함수를 호출하여 실제 행동을 위임합니다.
    player.move(m_direction);
}

// --- JumpCommand ---
void JumpCommand::execute(Player& player) {
    player.jump();
}

// --- AttackCommand ---
void AttackCommand::execute(Player& player) {
    player.attack();
}

// --- DashCommand ---
void DashCommand::execute(Player& player) {
    player.dash();
}

// --- ParryCommand ---
void ParryCommand::execute(Player& player) {
    player.parryAction();
}

// --- WeakPointAttackCommand ---
void WeakPointAttackCommand::execute(Player& player) {
    player.weakPointAttack();
}

