#pragma once
#include "ICommand.h"

/**
 * @brief 이동 커맨드. 지속적인 입력을 처리합니다.
 */
class MoveCommand : public ICommand {
public:
    // float로부터의 암시적 형변환을 막기 위해 explicit 키워드를 사용합니다.
    explicit MoveCommand(float direction);
    void execute(Player& player) override;
private:
    float m_direction; // -1.0f (왼쪽), 1.0f (오른쪽)
};

/**
 * @brief 점프 커맨드.
 */
class JumpCommand : public ICommand {
public:
    void execute(Player& player) override;
};

/**
 * @brief 기본 공격 커맨드. 3연타 콤보를 시작하거나 이어갑니다.
 */
class AttackCommand : public ICommand {
public:
    void execute(Player& player) override;
};

/**
 * @brief 대쉬(순간이동) 커맨드.
 */
class DashCommand : public ICommand {
public:
    void execute(Player& player) override;
};

/**
 * @brief 패링 커맨드.
 */
class ParryCommand : public ICommand {
public:
    void execute(Player& player) override;
};

/**
 * @brief 약점 공격 커맨드.
 * 현재는 일반 공격과 동일한 로직을 트리거하지만, 별도 스킬로 확장될 수 있습니다.
 */
class WeakPointAttackCommand : public ICommand {
public:
    void execute(Player& player) override;
};

