#pragma once
#include "ICommand.h"

// PlayerCommand는 Player와 관련된 모든 커맨드들의 부모 클래스 역할을 합니다.
class PlayerCommand : public ICommand {
public:
    // ICommand의 execute 함수를 재정의하여 공통 로직을 구현합니다.
    virtual void execute(Player& player) override;
};


// --- 데이터가 필요 없는 '신호' 역할의 커맨드들 ---

class AttackCommand : public PlayerCommand {};        // 일반 공격
class StartChargeCommand : public PlayerCommand {};   // 차지 시작
class ReleaseChargeCommand : public PlayerCommand {}; // 차지 발사
class WeakPointAttackCommand : public PlayerCommand {}; // 약점 공격


// --- 추가 데이터가 필요한 커맨드들 ---

// 점프 커맨드 (어느 방향으로 점프했는지 방향 값을 가짐)
class JumpCommand : public PlayerCommand {
public:
    JumpCommand(float direction) : m_direction(direction) {}
    float getDirection() const { return m_direction; }
private:
    float m_direction;
};

// 이동 커맨드 (이동 방향 값을 가짐)
class MoveCommand : public PlayerCommand {
public:
    MoveCommand(float direction) : m_direction(direction) {}
    float getDirection() const { return m_direction; }
private:
    float m_direction;
};

// 대쉬 커맨드 (대쉬 방향 값을 가짐)
class DashCommand : public PlayerCommand {
public:
    DashCommand(float direction) : m_direction(direction) {}
    float getDirection() const { return m_direction; }
private:
    float m_direction;
};