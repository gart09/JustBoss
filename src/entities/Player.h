#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Player
{
public:
    Player();

    // 키가 눌리는 '순간'의 이벤트를 처리합니다 (점프, 공격, 스킬)
    void handleInput(const sf::Event& event);

    // 매 프레임마다 호출되어 플레이어의 상태를 업데이트합니다 (이동, 중력)
    void update(sf::Time deltaTime);

    // 플레이어를 화면에 그립니다.
    void draw(sf::RenderWindow& window);
    
    float* getAirControlForcePtr() { return &m_airControlForce; }
    float* getSpeed() { return &m_speed; }

private:
    // 내부적으로 사용될 함수들
    void attack();
    void jump();
    void useSkill(const std::string& skillName);

    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;

    // --- 플레이어 능력치 및 물리 상수 ---
    float m_speed = 300.f;      // 초당 이동 속도 (픽셀)
    float m_gravity = 1800.f;   // 중력 가속도
    float m_jumpStrength = 600.f; // 점프 시의 수직 속도
    float m_airControlForce = 0.1f; 

    enum class FacingDirection { Left, Right };
    FacingDirection m_facingDirection; // 현재 바라보는 방향

    // --- 플레이어 상태 변수 ---
    bool m_canJump;      // 지상에서 점프가 가능한가?
    bool m_canDoubleJump; // 공중에서 더블점프가 가능한가?
};

