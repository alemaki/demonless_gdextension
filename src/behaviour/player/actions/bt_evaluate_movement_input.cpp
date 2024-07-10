#include "bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

BTTask::Status BTEvaluateMovementInput::_tick(double delta)
{
    PlayerCharacter* player = godot::Object::cast_to<PlayerCharacter>(this->get_actor());
    TASK_FAIL_COND_MSG(player == nullptr, this->get_name() + ": actor is not a player character.");

    CharacterInputComponent* input_cp = player->get_input_component();
    TASK_FAIL_COND_MSG(input_cp == nullptr, this->get_name() + ": actor doesn't have an input component.");

    player->move_and_collide(input_cp->get_direction_input()*player->get_movement_speed()*delta);
    TASK_SUCCEED_COND(true);
}

void BTEvaluateMovementInput::_bind_methods()
{
    using namespace godot;
}