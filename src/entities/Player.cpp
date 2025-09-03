#include "Player.h"

Player::Player()
{
    // 플레이어를 임시로 파란색 사각형으로 표현합니다.
    m_shape.setSize({50.f, 100.f});
    m_shape.setFillColor(sf::Color::Blue);
    
    // y좌표 620을 임시 "땅"으로 가정하고 그 위에 서 있도록 위치를 설정합니다.
    // (창 높이 720 - 플레이어 높이 100)
    m_shape.setPosition({200.f, 720.f - 100.f});

    m_canJump = true; // 처음에는 땅에 있으므로 점프 가능
}

void Player::handleInput(const sf::Event& event)
{
    // --- ❗️ SFML 3.0.0 이벤트 처리 방식 ❗️ ---
    // getIf<>() 템플릿 함수로 특정 키 입력 이벤트를 가져옵니다.
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        // keyPressed->code 로 키 코드를 확인합니다.
        if (keyPressed->code == sf::Keyboard::Key::D)
        {
            jump();
        }
        else if (keyPressed->code == sf::Keyboard::Key::A)
        {
            attack();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Q)
        {
            useSkill("Q");
        }
        else if (keyPressed->code == sf::Keyboard::Key::W)
        {
            useSkill("W");
        }
        else if (keyPressed->code == sf::Keyboard::Key::E)
        {
            useSkill("E");
        }
        else if (keyPressed->code == sf::Keyboard::Key::R)
        {
            useSkill("R");
        }
    }
}

void Player::update(sf::Time deltaTime)
{
    m_velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_velocity.x -= m_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_velocity.x += m_speed;
    }

    // 중력 적용
    m_velocity.y += m_gravity * deltaTime.asSeconds();
    m_shape.move(m_velocity * deltaTime.asSeconds());

    // 간단한 바닥 충돌 처리 (예시)
    if (m_shape.getPosition().y > 720.f - 100.f)
    {
        m_shape.setPosition({m_shape.getPosition().x, 720.f - 100.f});
        m_velocity.y = 0;
        m_canJump = true;
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}


// --- Private Member Functions ---

void Player::attack()
{
    // TODO: 여기에 실제 공격 애니메이션, 충돌 판정 등의 코드를 구현합니다.
    std::cout << "Action: Basic Attack!" << std::endl;
}

void Player::jump()
{
    // 점프 가능한 상태일 때만 점프합니다.
    if (m_canJump)
    {
        m_velocity.y = -m_jumpStrength; // y속도에 음수 값을 주어 위로 튀어 오르게 함
        m_canJump = false; // 점프를 했으므로 땅에 닿기 전까지 다시 점프 불가능
        std::cout << "Action: Jump!" << std::endl;
    }
}

void Player::useSkill(const std::string& skillName)
{
    // TODO: 각 스킬에 맞는 이펙트 생성, 재사용 대기시간 등의 코드를 구현합니다.
    std::cout << "Action: Used Skill [" << skillName << "]!" << std::endl;
}

