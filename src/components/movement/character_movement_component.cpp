#include "character_movement_component.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

void CharacterMovementComponent::_physics_process(double delta)
{
    ERR_FAIL_NULL(this->character);

    this->current_velocity += (this->target_velocity - this->current_velocity) * this->friction;
    character->set_velocity(this->current_velocity);
    character->move_and_slide();
}

void CharacterMovementComponent::set_target_velocity(godot::Vector2 target_velocity)
{
    this->target_velocity = target_velocity;
}

void CharacterMovementComponent::set_speed(float speed)
{
    speed = godot::Math::clamp<float>(speed, 0, speed);
    this->speed = speed;
}

void CharacterMovementComponent::set_friction(float friction)
{
    friction = godot::Math::clamp<float>(friction, 0, 1);
    this->friction = friction;
}

void CharacterMovementComponent::set_character(godot::CharacterBody2D* character)
{
    this->character = character;
}

void CharacterMovementComponent::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &CharacterMovementComponent::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &CharacterMovementComponent::get_speed);
    ClassDB::bind_method(D_METHOD("set_friction", "friction"), &CharacterMovementComponent::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &CharacterMovementComponent::get_friction);
    ClassDB::bind_method(D_METHOD("set_character", "character"), &CharacterMovementComponent::set_character);
    ClassDB::bind_method(D_METHOD("get_character"), &CharacterMovementComponent::get_character);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "friction"), "set_friction", "get_friction");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "character", PROPERTY_HINT_NODE_TYPE, "CharacterBody2D"), "set_character", "get_character");
}
