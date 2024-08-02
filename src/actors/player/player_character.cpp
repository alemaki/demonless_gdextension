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

void PlayerCharacter::set_health_component(HealthComponent* health_component)
{
    this->health_component = health_component;
}

void PlayerCharacter::set_input_component(CharacterInputComponent* input_component)
{
    this->input_component = input_component;
}

void PlayerCharacter::set_movement_component(CharacterMovementComponent* movement_component)
{
    this->movement_component = movement_component;
    if (this->movement_component != nullptr)
    {
        this->movement_component->set_character(this);
    }
}

void PlayerCharacter::set_movement_fsm(FSM* movement_fsm)
{
    this->movement_fsm = movement_fsm;
}

void PlayerCharacter::set_action_fsm(FSM* action_fsm)
{
    this->action_fsm = action_fsm;
}

void PlayerCharacter::set_decision_tree(BehaviourTree* decision_tree)
{
    this->decision_tree = decision_tree;
}

void PlayerCharacter::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_health_component", "health_component"), &PlayerCharacter::set_health_component);
    ClassDB::bind_method(D_METHOD("get_health_component"), &PlayerCharacter::get_health_component);
    ClassDB::bind_method(D_METHOD("set_movement_component", "movement_component"), &PlayerCharacter::set_movement_component);
    ClassDB::bind_method(D_METHOD("get_movement_component"), &PlayerCharacter::get_movement_component);
    ClassDB::bind_method(D_METHOD("set_input_component", "input_component"), &PlayerCharacter::set_input_component);
    ClassDB::bind_method(D_METHOD("get_input_component"), &PlayerCharacter::get_input_component);
    ClassDB::bind_method(D_METHOD("set_movement_fsm", "movement_fsm"), &PlayerCharacter::set_movement_fsm);
    ClassDB::bind_method(D_METHOD("get_movement_fsm"), &PlayerCharacter::get_movement_fsm);
    ClassDB::bind_method(D_METHOD("set_action_fsm", "action_fsm"), &PlayerCharacter::set_action_fsm);
    ClassDB::bind_method(D_METHOD("get_action_fsm"), &PlayerCharacter::get_action_fsm);
    ClassDB::bind_method(D_METHOD("set_decision_tree", "decision_tree"), &PlayerCharacter::set_decision_tree);
    ClassDB::bind_method(D_METHOD("get_decision_tree"), &PlayerCharacter::get_decision_tree);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "health_component", PropertyHint::PROPERTY_HINT_NODE_TYPE, "HealthComponent"), "set_health_component", "get_health_component");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_component", PropertyHint::PROPERTY_HINT_NODE_TYPE, "CharacterMovementComponent"), "set_movement_component", "get_movement_component");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_component", PropertyHint::PROPERTY_HINT_NODE_TYPE, "CharacterInputComponent"), "set_input_component", "get_input_component");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_fsm", PropertyHint::PROPERTY_HINT_NODE_TYPE, "FSM"), "set_movement_fsm", "get_movement_fsm");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "action_fsm", PropertyHint::PROPERTY_HINT_NODE_TYPE, "FSM"), "set_action_fsm", "get_action_fsm");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "decision_tree", PropertyHint::PROPERTY_HINT_NODE_TYPE, "BehaviourTree"), "set_decision_tree", "get_decision_tree");
}
