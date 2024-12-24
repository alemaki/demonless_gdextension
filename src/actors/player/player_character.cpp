#include "player_character.hpp"

#include <godot_cpp/classes/engine.hpp>

PlayerCharacter::~PlayerCharacter()
{

}

void PlayerCharacter::_ready()
{
    this->action_fsm = memnew(FSM);
    this->movement_fsm = memnew(FSM);

    this->action_fsm->add_state("idle");
    this->action_fsm->add_state("attack");
    this->action_fsm->add_state("block");

    this->movement_fsm->add_state("idle");
    this->movement_fsm->add_state("move");

    if (this->mesh_instance != nullptr)
    {
        this->animation_player = godot::Object::cast_to<AnimationPlayer>(this->mesh_instance->find_child("AnimationPlayer", true));
    }

    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        this->set_process(false);
        this->set_physics_process(false);
    }

    this->add_child(action_fsm);
    this->add_child(movement_fsm);
}

void PlayerCharacter::process_action_state()
{
    ERR_FAIL_NULL(this->input_component);
    if ((this->action_fsm->get_state() == "idle") && (this->input_component->is_attack_pressed()))
    {
        this->action_fsm->transition_to("attack");
    }
    if ((this->action_fsm->get_state() == "idle") && (this->input_component->is_block_pressed()))
    {
        this->action_fsm->transition_to("block");
    }
    if ((this->action_fsm->get_state() == "block") && !(this->input_component->is_block_pressed()))
    {
        this->action_fsm->transition_to("idle");
    }
}


void PlayerCharacter::process_movement_state()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->action_fsm->get_state() == "attack")
    {
        this->movement_fsm->transition_to("idle");
    }
    godot::Vector3 dir = this->input_component->get_direction_input();
    if ((this->action_fsm->get_state() != "attack") && !(dir.is_equal_approx(godot::Vector3(0, 0, 0))))
    {
        this->movement_fsm->transition_to("move");
    }
}

void PlayerCharacter::_process(double delta)
{
    if (this->input_component == nullptr)
    {
        return;
    }
    this->process_action_state();
    this->process_movement_state();
}

void PlayerCharacter::process_action()
{
    if (this->action_fsm->get_state() == "attack")
    {
        //TODO
        this->action_fsm->transition_to("idle");
    }
    else if (this->action_fsm->get_state() == "block")
    {
        //TODO
        this->action_fsm->transition_to("idle");
    }
}

void PlayerCharacter::process_movement()
{
    ERR_FAIL_NULL(this->animation_player);
    ERR_FAIL_NULL(this->movement_component);
    ERR_FAIL_NULL(this->input_component);
    if (this->movement_fsm->get_state() == "idle")
    {
        this->animation_player->set_current_animation("idle");
        this->movement_component->set_target_direction(godot::Vector3(0, 0, 0));
    }
    else if (this->movement_fsm->get_state() == "move")
    {
        this->animation_player->set_current_animation("run");
        godot::Vector3 direciton = this->input_component->get_direction_input();
        this->movement_component->set_target_direction(direciton);
        this->look_at(direciton);
    }
}

void PlayerCharacter::_physics_process(double delta)
{
    this->process_action();
    this->process_movement();
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
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, mesh_instance, PropertyHint::PROPERTY_HINT_NODE_TYPE, "Node3D", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, Node3D);
}
