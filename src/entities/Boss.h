#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class IBossPhaseState; // 전방 선언
class Player;

class Boss
{
public:
    Boss();
    ~Boss();
    void update(sf::Time deltaTime, Player& player);
    void draw(sf::RenderWindow& window);
    int getHP() { return m_hp; }
    int getMaxHP() { return m_maxHp; }
    sf::Vector2f getPosition() const { return m_shape.getPosition(); }

    // 플레이어가 충돌을 확인할 수 있도록 자신의 히트박스를 반환하는 함수
    sf::FloatRect getHitbox() const;

    void takeDamage(int damage);
    void changeState(std::unique_ptr<IBossPhaseState> newState);
    void wander(float dt, float moveSpeed);
    void move(float vx, float vy); 

private:
    sf::RectangleShape m_shape; // 임시로 사용할 사각형 모양
    int m_hp = 500;
    int m_maxHp = 500;
    float wanderDirection_ = 1.0f; // 1.0f: 오른쪽, -1.0f: 왼쪽
    float wanderTimer_ = 0.0f; // 방향 전환 타이머
    std::unique_ptr<IBossPhaseState> m_currentState;

    sf::Color m_originalColor;     // 원래 색상
    float m_hitEffectTimer;        // 하얗게 점멸하는 효과가 지속되는 시간
};
