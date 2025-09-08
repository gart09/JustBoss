#pragma once

// 전방 선언 (Forward Declaration)으로 Player.h 포함을 피하여 컴파일 의존성을 낮춥니다.
class Player;

/**
 * @brief 모든 커맨드 클래스가 상속받는 인터페이스입니다.
 * execute 함수 하나만을 정의하며, 이를 통해 특정 행동을 요청합니다.
 */
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Player& player) = 0;
};

