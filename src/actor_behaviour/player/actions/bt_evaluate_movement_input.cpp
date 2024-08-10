#include "bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

BTTask::Status BTEvaluateMovementInput::_tick(double delta)
{
    PlayerCharacter* player = godot::Object::cast_to<PlayerCharacter>(this->get_actor());
    TASK_FAIL_COND_COMP_MSG(player == nullptr, this->get_name() + ": actor is not a player character.");

    CharacterInputComponent* input_cp = player->get_input_component();
    TASK_FAIL_COND_COMP_MSG(input_cp == nullptr, this->get_name() + ": player doesn't have an input component.");

    CharacterMovementComponent* movement_cp = player->get_movement_component();
    TASK_FAIL_COND_COMP_MSG(movement_cp == nullptr, this->get_name() + ": player doesn't have a movement component.");

    movement_cp->set_target_velocity(input_cp->get_direction_input()*movement_cp->get_speed());
    TASK_SUCCEED();
}

void BTEvaluateMovementInput::_bind_methods()
{
    using namespace godot;
}