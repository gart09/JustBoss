#pragma once

#include <SFML/Graphics.hpp>

// Player Ŭ������ ��ü ���Ǹ� �����ϴ� ���, ���� ������ ����Ͽ�
// ������ �ð��� ���̰� ��� ���� �������� ����ϴ�.
class Player; 

// ImGui�� �ʱ�ȭ, ������Ʈ, �������� �����ϴ� ���� Ŭ�����Դϴ�.
class DebugUI
{
public:
    // ������: ���� â�� ���� ������ �޾� ImGui�� �ʱ�ȭ�մϴ�.
    DebugUI(sf::RenderWindow& window);
    
    // �Ҹ���: ImGui�� �����մϴ�.
    ~DebugUI();

    // SFML �̺�Ʈ�� ImGui�� �����Ͽ� ���콺 Ŭ��, Ű���� �Է� ���� ó���մϴ�.
    void processEvent(sf::RenderWindow& window, const sf::Event& event);
    
    // �� �����Ӹ��� ȣ��Ǿ� UI â�� ����(�Է� ���� ��)�� �����մϴ�.
    void update(sf::Time deltaTime);
    
    // ���������� ������ UI�� ȭ�鿡 �׸��ϴ�.
    void render(sf::RenderWindow& window);

    // GameState�κ��� Player ��ü�� ���� �����͸� �޾ƿɴϴ�.
    void setPlayer(Player* player);

private:
    Player* m_player = nullptr; // ������� �÷��̾� ��ü�� ���� ������
};

