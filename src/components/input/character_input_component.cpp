#include "character_input_component.hpp"
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

CharacterInputComponent::CharacterInputComponent()
{
    godot::InputMap* input_map = godot::InputMap::get_singleton();
    ERR_FAIL_NULL(input_map);

    /* ui_<direction> is added by default */
    if (!(input_map->has_action("ui_attack")))
    {
        input_map->add_action("ui_attack");
    }
    if (!(input_map->has_action("ui_block")))
    {
        input_map->add_action("ui_block");
    }
}

void CharacterInputComponent::evaluate_input()
{
    godot::Input* input = godot::Input::get_singleton();
    this->direction_input = input->get_vector("ui_left", "ui_right", "ui_up", "ui_down");

    this->attack_pressed = input->is_action_pressed("ui_attack");

    this->block_pressed = input->is_action_pressed("ui_block");
}

void CharacterInputComponent::_process(double delta)
{
    this->evaluate_input();
}

void CharacterInputComponent::_bind_methods()
{
    using namespace godot;
    
    ClassDB::bind_method(D_METHOD("get_direction_input"),&CharacterInputComponent::get_direction_input);

    /* ClassDB::bind_method(D_METHOD("_process", "delta"), &CharacterInputComponent::_process); */
}