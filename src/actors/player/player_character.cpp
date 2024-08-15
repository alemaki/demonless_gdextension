#include "player_character.hpp"

#include <godot_cpp/classes/engine.hpp>

void PlayerCharacter::_ready()
{
    this->blackboard = godot::Ref<Blackboard>(memnew(Blackboard));
    if (this->decision_tree != nullptr)
    {
        this->task = this->decision_tree->instantiate(this, this->blackboard);
    }
    this->blackboard->set_var("PlayerActionFSM", this->action_fsm);
    this->blackboard->set_var("PlayerMovementFSM", this->movement_fsm);

    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        this->set_process(false);
        this->set_physics_process(false);
    }
}

void PlayerCharacter::_process(double delta)
{
    this->blackboard->set_var("is_attack_pressed", input_component->is_attack_pressed());
    this->blackboard->set_var("is_block_pressed", input_component->is_block_pressed());
    this->blackboard->set_var("direction_input", input_component->get_direction_input());

    if (this->task != nullptr)
    {
        this->task->execute(delta);
    }
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

    BIND_GETTER_SETTER_PROPERTY_OBJECT(PlayerCharacter, health_component, health_component, PropertyHint::PROPERTY_HINT_NODE_TYPE, "HealthComponent", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, HealthComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT(PlayerCharacter, movement_component, movement_component, PropertyHint::PROPERTY_HINT_NODE_TYPE, "CharacterMovementComponent", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, CharacterMovementComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT(PlayerCharacter, input_component, input_component, PropertyHint::PROPERTY_HINT_NODE_TYPE, "CharacterInputComponent", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, CharacterInputComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT(PlayerCharacter, movement_fsm, movement_fsm, PropertyHint::PROPERTY_HINT_NODE_TYPE, "FSM", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, FSM);
    BIND_GETTER_SETTER_PROPERTY_OBJECT(PlayerCharacter, action_fsm, action_fsm, PropertyHint::PROPERTY_HINT_NODE_TYPE, "FSM", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, FSM);
    BIND_GETTER_SETTER_PROPERTY_OBJECT(PlayerCharacter, decision_tree, decision_tree, PropertyHint::PROPERTY_HINT_NODE_TYPE, "BehaviourTree", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, BehaviourTree);
    
}
