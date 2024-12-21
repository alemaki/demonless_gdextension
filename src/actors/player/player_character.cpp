#include "player_character.hpp"

#include <godot_cpp/classes/engine.hpp>

void PlayerCharacter::_ready()
{
    if (this->action_fsm != nullptr)
    {

    }
    if (this->movement_fsm != nullptr)
    {

    }
    if (this->mesh_instance != nullptr)
    {
        this->animation_player = godot::Object::cast_to<AnimationPlayer>(this->mesh_instance->find_child("AnimationPlayer", true));
    }

    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        this->set_process(false);
        this->set_physics_process(false);
    }
}

void PlayerCharacter::_process(double delta)
{

}

void PlayerCharacter::_physics_process(double delta)
{

}

void PlayerCharacter::set_movement_component(CharacterMovementComponent* movement_component)
{
    this->movement_component = movement_component;
    if (this->movement_component != nullptr)
    {
        this->movement_component->set_character(this);
    }
}

void PlayerCharacter::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, health_component, PropertyHint::PROPERTY_HINT_NODE_TYPE, "HealthComponent", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, HealthComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, movement_component, PropertyHint::PROPERTY_HINT_NODE_TYPE, "CharacterMovementComponent", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, CharacterMovementComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, input_component, PropertyHint::PROPERTY_HINT_NODE_TYPE, "CharacterInputComponent", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, CharacterInputComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, movement_fsm, PropertyHint::PROPERTY_HINT_NODE_TYPE, "FSM", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, FSM);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, action_fsm, PropertyHint::PROPERTY_HINT_NODE_TYPE, "FSM", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, FSM);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, mesh_instance, PropertyHint::PROPERTY_HINT_NODE_TYPE, "Node3D", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, Node3D);
}
