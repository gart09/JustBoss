#pragma once
#include "ICommand.h"
#include "../entities/Player.h"


class PlayerCommand : public ICommand {
public:
    virtual void execute(Player& player) override;
};

// 데이터가 필요 없는 커맨드들
class JumpCommand : public PlayerCommand {
public:
    JumpCommand(float direction) : m_direction(direction) {}
    float getDirection() const { return m_direction; }
private:
    float m_direction;
};
class AttackCommand : public PlayerCommand {};
class StartChargeCommand : public PlayerCommand {};
class ReleaseChargeCommand : public PlayerCommand {};
class WeakPointAttackCommand : public PlayerCommand {};

// 데이터가 필요한 커맨드들
class MoveCommand : public PlayerCommand {
public:
    MoveCommand(float direction) : m_direction(direction) {}
    float getDirection() const { return m_direction; }
private:
    float m_direction;
};

class DashCommand : public PlayerCommand {
public:
    DashCommand(float direction) : m_direction(direction) {}
    float getDirection() const { return m_direction; }
private:
    float m_direction;
};
