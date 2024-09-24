
#include "bt_save_mouse_position.hpp"
#include "actors/player/player_character.hpp"

BTTask::Status BTSaveMousePosition::_tick(double delta)
{
    PlayerCharacter* player = godot::Object::cast_to<PlayerCharacter>(this->get_actor());
    TASK_FAIL_COND_COMP_MSG(player == nullptr, "Actor is not a player character.");

    CharacterInputComponent* input_cp = player->get_input_component();
    TASK_FAIL_COND_COMP_MSG(input_cp == nullptr, "Player doesn't have an input component.");

    TASK_FAIL_COND_COMP(this->mouse_position_var.is_empty());
    
    this->get_blackboard()->set_var(this->mouse_position_var, input_cp->get_mouse_casted_position());

    TASK_SUCCEED();
}

void BTSaveMousePosition::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSaveMousePosition, STRING_NAME, mouse_position_var); 
}