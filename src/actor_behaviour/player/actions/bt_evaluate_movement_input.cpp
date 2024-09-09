#include "bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

BTTask::Status BTEvaluateMovementInput::_tick(double delta)
{
    PlayerCharacter* player = godot::Object::cast_to<PlayerCharacter>(this->get_actor());
    TASK_FAIL_COND_COMP_MSG(player == nullptr,"Actor is not a player character.");

    CharacterInputComponent* input_cp = player->get_input_component();
    TASK_FAIL_COND_COMP_MSG(input_cp == nullptr, "Player doesn't have an input component.");

    CharacterMovementComponent* movement_cp = player->get_movement_component();
    TASK_FAIL_COND_COMP_MSG(movement_cp == nullptr, "Player doesn't have a movement component.");

    movement_cp->set_target_direction(input_cp->get_direction_input());
    if (!(input_cp->get_direction_input().is_zero_approx()))
    {
        player->look_at(player->get_position() - input_cp->get_direction_input());
    }
    TASK_SUCCEED();
}

void BTEvaluateMovementInput::_bind_methods()
{
    using namespace godot;
}