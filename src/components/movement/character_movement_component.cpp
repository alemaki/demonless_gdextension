#include "character_movement_component.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

void CharacterMovementComponent::_physics_process(double delta)
{
    ERR_FAIL_NULL(this->character);

    this->current_velocity += (this->target_velocity - this->current_velocity) * this->friction;
    character->set_velocity(this->current_velocity);
    character->move_and_slide();
}

void CharacterMovementComponent::set_target_direction(godot::Vector3 direction)
{
    this->target_velocity = direction.normalized()*this->speed;
}

void CharacterMovementComponent::set_friction(double friction)
{
    friction = godot::Math::clamp<double>(friction, 0, 1);
    this->friction = friction;
}

void CharacterMovementComponent::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CharacterMovementComponent, FLOAT, speed);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CharacterMovementComponent, FLOAT, friction);
    
    BIND_GETTER_SETTER_DEFAULT(CharacterMovementComponent, character);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "character", PROPERTY_HINT_NODE_TYPE, "CharacterBody3D"), "set_character", "get_character");
}
