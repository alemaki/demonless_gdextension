#include "bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

BTTask::Status BTEvaluateMovementInput::_tick(double delta)
{
    PlayerCharacter* player = godot::Object::cast_to<PlayerCharacter>(this->get_actor());
    TASK_FAIL_COND_MSG(player == nullptr, this->get_name() + ": actor is not a player character.");

    CharacterInputComponent* input_cp = player->get_input_component();
    TASK_FAIL_COND_MSG(input_cp == nullptr, this->get_name() + ": actor doesn't have an input component.");

    player->set_velocity(input_cp->get_direction_input()*player->get_movement_speed());
    TASK_SUCCEED_COND(true);
}

void BTEvaluateMovementInput::set_complain(bool complain)
{
    this->complain = complain;
}

void BTEvaluateMovementInput::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_complain", "complain"), &BTEvaluateMovementInput::set_complain);
    ClassDB::bind_method(D_METHOD("get_complain"), &BTEvaluateMovementInput::get_complain);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "complain"), "set_complain", "get_complain");
}