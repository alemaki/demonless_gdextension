#include "player_character.hpp"

#include <godot_cpp/classes/engine.hpp>

PlayerCharacter::~PlayerCharacter()
{

}

void PlayerCharacter::_ready()
{
    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        this->set_process(false);
        this->set_physics_process(false);
        return;
    }

    this->action_fsm = memnew(FSM);
    this->movement_fsm = memnew(FSM);

    this->action_idle = this->action_fsm->create_state();
    this->action_attack = this->action_fsm->create_state();
    this->action_block = this->action_fsm->create_state();
    this->action_fsm->set_initial_state(this->action_idle);
    this->action_block->add_enter_callable(callable_mp(this, &PlayerCharacter::_enter_block));
    this->action_block->add_exit_callable(callable_mp(this, &PlayerCharacter::_exit_block));

    this->movement_idle = this->movement_fsm->create_state();
    this->movement_run = this->movement_fsm->create_state();
    this->movement_fsm->set_initial_state(this->movement_idle);

    if (this->mesh_instance != nullptr)
    {
        this->animation_player = godot::Object::cast_to<AnimationPlayer>(this->mesh_instance->find_child("AnimationPlayer", true));
    }

    this->add_child(action_fsm);
    this->add_child(movement_fsm);

    this->movement_fsm->initialize();
    this->action_fsm->initialize();
}

void PlayerCharacter::process_action_state()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->action_fsm->get_state() != this->action_idle)
    {
        return;
    }
    if (this->input_component->is_attack_pressed())
    {
        this->action_fsm->transition_to_state(this->action_attack);
    }
    if (this->input_component->is_block_pressed())
    {
        this->action_fsm->transition_to_state(this->action_block);
    }
}

void PlayerCharacter::process_movement_state()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->action_fsm->get_state() == this->action_attack)
    {
        this->movement_fsm->transition_to_state(this->movement_idle);
    }
    godot::Vector3 dir = this->input_component->get_direction_input();
    if ((this->movement_fsm->get_state() == this->movement_idle)
        && (this->action_fsm->get_state() != this->action_attack)
        && !(dir.is_zero_approx()))
    {
        this->movement_fsm->transition_to_state(this->movement_run);
    }
    if (dir.is_zero_approx())
    {
        this->movement_fsm->transition_to_state(this->movement_idle);
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
    this->process_movement();
}

void PlayerCharacter::_enter_block()
{
    ERR_FAIL_NULL(this->hitbox_blocker);
    this->add_child(this->hitbox_blocker);
}

void PlayerCharacter::_exit_block()
{
    ERR_FAIL_NULL(this->hitbox_blocker);
    this->remove_child(this->hitbox_blocker);
}

void PlayerCharacter::process_action()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->action_fsm->get_state() == this->action_attack)
    {
        //TODO:
        this->action_fsm->transition_to_state(this->action_idle);
    }
    else if (this->action_fsm->get_state() == this->action_block)
    {
        //TODO:
        if (!this->input_component->is_block_pressed())
        {
            this->action_fsm->transition_to_state(this->action_idle);
        }
    }
}

void PlayerCharacter::process_movement()
{
    ERR_FAIL_NULL(this->animation_player);
    ERR_FAIL_NULL(this->movement_component);
    ERR_FAIL_NULL(this->input_component);
    if (this->movement_fsm->get_state() == this->movement_idle)
    {
        this->animation_player->set_current_animation("Idle");
        this->movement_component->set_target_direction(godot::Vector3(0, 0, 0));
    }
    else if (this->movement_fsm->get_state() == this->movement_run)
    {
        this->animation_player->set_current_animation("Running");
        godot::Vector3 direction = this->input_component->get_direction_input();
        this->movement_component->set_target_direction(direction);
        this->look_at(this->get_position() + direction);
    }
}


void PlayerCharacter::_physics_process(double delta)
{
    this->process_action();
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

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, health_component, PROPERTY_HINT_NODE_TYPE, "HealthComponent", PROPERTY_USAGE_DEFAULT, HealthComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, movement_component, PROPERTY_HINT_NODE_TYPE, "CharacterMovementComponent", PROPERTY_USAGE_DEFAULT, CharacterMovementComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, input_component, PROPERTY_HINT_NODE_TYPE, "CharacterInputComponent", PROPERTY_USAGE_DEFAULT, CharacterInputComponent);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(PlayerCharacter, mesh_instance, PROPERTY_HINT_NODE_TYPE, "Node3D", PROPERTY_USAGE_DEFAULT, Node3D);
}
