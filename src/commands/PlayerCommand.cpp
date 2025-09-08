#include "PlayerCommand.h"

void PlayerCommand::execute(Player& player) {
    auto newState = player.getCurrentState()->handleInput(player, this);
    if (newState) {
        player.changeState(std::move(newState));
    }
}