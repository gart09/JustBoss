#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "../state/IPlayerState.h"
#include "../data/AttackData.h"


// 전방 선언
class IPlayerState;
class ICommand;
class Boss;
enum class FacingDirection { Left, Right };
struct ActiveAttackInfo {
    sf::FloatRect worldHitbox;
    float damage;
};

class Player
{
public:
    Player();

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    void changeState(std::unique_ptr<IPlayerState> newState);

    //행동접수 함수
    void move(float direction);
    void jump();
    void attack();
    void dash(float direction);
    void weakPointAttack();

    //행동실행 함수
    void takeJump();
    void takeDoubleJump(float horizontal_input);

    // 피격
    void takeDamage(int damage, sf::Vector2f damageSourcePosition);

    // 게터 함수들
    std::optional<ActiveAttackInfo> getActiveAttackInfo() { return m_activeAttack; }
    const std::vector<AttackData>& getComboData() { return m_basicComboData; }
    sf::Vector2f getPosition() { return m_shape.getPosition(); }
    sf::FloatRect getHitbox() { return m_shape.getGlobalBounds(); }
    FacingDirection getFacingDirection() { return m_facingDirection; }
    float* getAirControlForcePtr() { return &m_airControlForce; }
    float* getSpeed() { return &m_speed; }
    int getHP() { return m_hp; }
    int getMaxHP() { return m_hp; }
    void getDamage(int damage) { m_hp -= damage; }

    bool m_canJump;
    bool m_canDoubleJump;
    std::optional<ActiveAttackInfo> m_activeAttack; // 현재 활성화된 공격 정보
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
private:
    void applyPhysics(float dt);
    void applyKnockback(sf::Vector2f knockbackVelocity);
    void handleDashCooldown(float dt);

    std::unique_ptr<IPlayerState> m_currentState;
    FacingDirection m_facingDirection;

    // --- 플레이어 능력치 및 물리 상수 ---
    float m_speed = 300.f;      // 초당 이동 속도 (픽셀)
    float m_gravity = 1800.f;   // 중력 가속도
    float m_jumpStrength = 600.f; // 점프 시의 수직 속도
    float m_airControlForce = 0.1f;
    int m_hp = 100;
    int m_maxHp = 100;

    // 대쉬 관련
    static constexpr float DASH_DISTANCE = 250.f;
    static constexpr float DASH_COOLDOWN_TIME = 3.0f;
    float m_dashCooldown;

    // 넉백 관련
    static constexpr float KNOCKBACK_POWER_X = 250.f;
    static constexpr float KNOCKBACK_POWER_Y = 300.f;

    // 공격 관련
    std::vector<AttackData> m_basicComboData;
    sf::RectangleShape m_debugAttackBox; // 공격 판정 시각화용
};

