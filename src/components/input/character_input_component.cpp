#include "character_input_component.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

CharacterInputComponent::CharacterInputComponent()
{
    
}

void CharacterInputComponent::evaluate_input()
{
    godot::Input* input = godot::Input::get_singleton();
    godot::Vector2 direction_input= input->get_vector("move_left", "move_right", "move_up", "move_down");
    this->direction_input = {direction_input.x, 0, direction_input.y};

    this->attack_pressed = input->is_action_pressed("attack");

    this->block_pressed = input->is_action_pressed("block");
}

void CharacterInputComponent::_process(double delta)
{
    this->evaluate_input();
}

void CharacterInputComponent::_ready()
{
    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        this->set_process(false);
        this->set_physics_process(false);
    }
}

void CharacterInputComponent::_bind_methods()
{
    using namespace godot;
    
    ClassDB::bind_method(D_METHOD("get_direction_input"),&CharacterInputComponent::get_direction_input);
    ClassDB::bind_method(D_METHOD("is_attack_pressed"),&CharacterInputComponent::is_attack_pressed);
    ClassDB::bind_method(D_METHOD("is_block_pressed"),&CharacterInputComponent::is_block_pressed);
}