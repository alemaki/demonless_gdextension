#include "player_character.hpp"



void PlayerCharacter::_ready()
{
    this->blackboard = godot::Ref<Blackboard>(memnew(Blackboard));
    this->task = this->behaviour_tree->instantiate(this, godot::Ref<Blackboard>(memnew(Blackboard)));
}

void PlayerCharacter::_process(double delta)
{

}

void PlayerCharacter::_physics_process(double delta)
{
    this->task->execute(delta);
}

void PlayerCharacter::set_health_component(HealthComponent* health_component)
{
    this->health_component = health_component;
}

void PlayerCharacter::set_input_component(CharacterInputComponent* input_component)
{
    this->input_component = input_component;
}

void PlayerCharacter::set_movement_fsm(FSM* movement_fsm)
{
    this->movement_fsm = movement_fsm;
}

void PlayerCharacter::set_action_fsm(FSM* action_fsm)
{
    this->action_fsm = action_fsm;
}

void PlayerCharacter::set_behaviour_tree(BehaviourTree* behaviour_tree)
{
    this->behaviour_tree = behaviour_tree;
}

void PlayerCharacter::set_movement_speed(double movement_speed)
{
    this->movement_speed = movement_speed;
}

void PlayerCharacter::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_movement_speed", "movement_speed"), &PlayerCharacter::set_movement_speed);
    ClassDB::bind_method(D_METHOD("get_movement_speed"), &PlayerCharacter::get_movement_speed);
    ClassDB::bind_method(D_METHOD("set_health_component", "health_component"), &PlayerCharacter::set_health_component);
    ClassDB::bind_method(D_METHOD("get_health_component"), &PlayerCharacter::get_health_component);
    ClassDB::bind_method(D_METHOD("set_input_component", "input_component"), &PlayerCharacter::set_input_component);
    ClassDB::bind_method(D_METHOD("get_input_component"), &PlayerCharacter::get_input_component);
    ClassDB::bind_method(D_METHOD("set_movement_fsm", "movement_fsm"), &PlayerCharacter::set_movement_fsm);
    ClassDB::bind_method(D_METHOD("get_movement_fsm"), &PlayerCharacter::get_movement_fsm);
    ClassDB::bind_method(D_METHOD("set_action_fsm", "action_fsm"), &PlayerCharacter::set_action_fsm);
    ClassDB::bind_method(D_METHOD("get_action_fsm"), &PlayerCharacter::get_action_fsm);
    ClassDB::bind_method(D_METHOD("set_behaviour_tree", "behaviour_tree"), &PlayerCharacter::set_behaviour_tree);
    ClassDB::bind_method(D_METHOD("get_behaviour_tree"), &PlayerCharacter::get_behaviour_tree);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed"), "set_movement_speed", "get_movement_speed");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "health_component"), "set_health_component", "get_health_component");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_component"), "set_input_component", "get_input_component");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_fsm"), "set_movement_fsm", "get_movement_fsm");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "action_fsm"), "set_action_fsm", "get_action_fsm");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "behaviour_tree"), "set_behaviour_tree", "get_behaviour_tree");
}
