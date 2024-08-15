#include "bt_character_input_pressed.hpp"

#include "actors/player/player_character.hpp"
#include "utils/utils.hpp"

BTTask::Status BTCharacterInputPressed::_tick(double delta)
{
    bool is_attack_pressed = this->get_blackboard()->get_var("is_attack_pressed", godot::Variant(), this->is_complain_enabled());
    bool is_block_pressed = this->get_blackboard()->get_var("is_block_pressed", godot::Variant(), this->is_complain_enabled());
    godot::Vector2 direction_input =  this->get_blackboard()->get_var("direction_input", godot::Variant(), this->is_complain_enabled());

    TASK_SUCCEED_COND((is_attack_pressed && this->check_attack_pressed)             
                      || (is_block_pressed && this->check_block_pressed)
                      || ((direction_input != godot::Vector2(0, 0)) && this->check_direction_pressed));

    TASK_FAIL();
}

void BTCharacterInputPressed::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTCharacterInputPressed, STRING, check_attack_pressed);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTCharacterInputPressed, STRING, check_block_pressed);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTCharacterInputPressed, STRING, check_direction_pressed);
}