#include "player_character.hpp"

#include <godot_cpp/classes/engine.hpp>

PlayerCharacter::~PlayerCharacter()
{

}

void PlayerCharacter::_ready()
{
    DISABLE_EDITOR_PROCESSING();

    this->action_fsm = memnew(FSM);
    this->movement_fsm = memnew(FSM);

    this->blocking_timer = memnew(Timer);
    this->blocking_timer->set_autostart(false);
    this->blocking_timer->set_one_shot(true);
    this->blocking_timer->connect(
        "timeout",
        callable_mp(this, &PlayerCharacter::_on_blocking_react_finished)
        );
    this->add_child(this->blocking_timer);

    this->action_idle = this->action_fsm->create_state();
    this->action_attacking = this->action_fsm->create_state();
    this->action_blocking = this->action_fsm->create_state();
    this->action_blocking_react = this->action_fsm->create_state();
    this->action_fsm->set_initial_state(this->action_idle);

    this->movement_idle = this->movement_fsm->create_state();
    this->movement_idle->set_name("idle");
    this->movement_running = this->movement_fsm->create_state();
    this->movement_running->set_name("running");
    this->movement_fsm->set_initial_state(this->movement_idle);

    ERR_FAIL_NULL(this->movement_idle);
    ERR_FAIL_NULL(this->movement_running);
    ERR_FAIL_NULL(this->action_blocking);
    ERR_FAIL_NULL(this->action_blocking_react);

    this->movement_idle->add_enter_callable(callable_mp(this, &PlayerCharacter::_enter_idle));
    this->movement_running->add_enter_callable(callable_mp(this, &PlayerCharacter::_enter_movement));
    this->movement_running->add_process_callable(callable_mp(this, &PlayerCharacter::_process_movement));
    this->action_blocking->add_enter_callable(callable_mp(this, &PlayerCharacter::_enter_block));
    this->action_blocking->add_process_callable(callable_mp(this, &PlayerCharacter::_process_block));
    this->action_blocking->add_exit_callable(callable_mp(this, &PlayerCharacter::_exit_block));
    this->action_blocking_react->add_enter_callable(callable_mp(this, &PlayerCharacter::_enter_block_react));
    this->action_blocking_react->add_process_callable(callable_mp(this, &PlayerCharacter::_process_block_react));
    this->action_blocking_react->add_exit_callable(callable_mp(this, &PlayerCharacter::_exit_block_react));

    if (this->mesh_instance != nullptr)
    {
        this->animation_player = godot::Object::cast_to<AnimationPlayer>(this->mesh_instance->find_child("AnimationPlayer", true));
    }

    this->add_child(action_fsm);
    this->add_child(movement_fsm);

    this->movement_fsm->initialize();
    this->action_fsm->initialize();

    utils::ensure_node(this->health_component, this, "HealthComponent");
    utils::ensure_node(this->hitbox_blocker, this, "HitboxBlocker");
    utils::ensure_node(this->hurtbox, this, "Hurtbox");

    if (this->hitbox_blocker)
    {
        this->hitbox_blocker->connect("hitbox_blocked", callable_mp(this, &PlayerCharacter::hitbox_blocked));
        this->hitbox_blocker->set_blocker_enabled(false);
    }

    if (this->hurtbox)
    {
        hurtbox->connect("hurtbox_hit", callable_mp(this, &PlayerCharacter::_on_hurtbox_hit));
    }
}

void PlayerCharacter::process_action_state()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->action_fsm->get_state() == this->action_attacking)
    {
        //TODO:
        this->action_fsm->transition_to_state(this->action_idle);
    }
    else if (this->action_fsm->get_state() == this->action_idle)
    {
        if (this->input_component->is_block_pressed())
        {
            this->action_fsm->transition_to_state(this->action_blocking);
        }
    }
    else if (this->action_fsm->get_state() == this->action_blocking)
    {
        if (!this->input_component->is_block_pressed())
        {
            this->action_fsm->transition_to_state(this->action_idle);
        }
    }
}

void PlayerCharacter::process_movement_state()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->action_fsm->get_state() == this->action_attacking)
    {
        this->movement_fsm->transition_to_state(this->movement_idle);
    }
    godot::Vector3 dir = this->input_component->get_direction_input();

    if ((this->movement_fsm->get_state() == this->movement_idle)
        && (this->action_fsm->get_state() != this->action_attacking)
        && (this->action_fsm->get_state() != this->action_blocking_react)
        && !(dir.is_zero_approx()))
    {
        this->movement_fsm->transition_to_state(this->movement_running);
    }
    if (dir.is_zero_approx())
    {
        this->movement_fsm->transition_to_state(this->movement_idle);
    }
}

void PlayerCharacter::_enter_idle()
{
    ERR_FAIL_NULL(this->animation_player);
    ERR_FAIL_NULL(this->movement_component);
    this->animation_player->set_current_animation("Idle");
    this->movement_component->set_target_direction(godot::Vector3(0, 0, 0));
}

void PlayerCharacter::_enter_movement()
{
    ERR_FAIL_NULL(this->animation_player);
    this->animation_player->set_current_animation("Running");
}

void PlayerCharacter::_process_movement(double delta)
{
    ERR_FAIL_NULL(this->movement_component);
    ERR_FAIL_NULL(this->input_component);
    godot::Vector3 direction = this->input_component->get_direction_input();
    if (direction.is_zero_approx())
    {
        this->movement_fsm->transition_to_state(this->movement_idle);
        // TODO: what about lost delta time?
        return;
    }
    this->movement_component->set_target_direction(direction);
    this->look_at(this->get_position() + direction);
}

void PlayerCharacter::_enter_block()
{
    ERR_FAIL_NULL(this->input_component);
    ERR_FAIL_NULL(this->hitbox_blocker);
    this->hitbox_blocker->set_blocker_enabled(true);
    this->movement_component->set_speed(this->movement_component->get_speed()/3.0);
}

void PlayerCharacter::_process_block(double delta)
{
    ERR_FAIL_NULL(this->input_component);
    ERR_FAIL_NULL(this->hitbox_blocker);
    if (!this->input_component->is_block_pressed())
    {
        this->action_fsm->transition_to_state(this->action_idle);
        return;
    }
    godot::Vector3 mouse_position = this->input_component->get_mouse_casted_position();
    ERR_FAIL_COND(mouse_position.is_equal_approx(this->get_position()));
    this->look_at(mouse_position);
    //TODO: unneded blocker attached to player face? this->hitbox_blocker->look_at(mouse_position);
}

void PlayerCharacter::_exit_block()
{
    ERR_FAIL_NULL(this->hitbox_blocker);
    this->hitbox_blocker->set_blocker_enabled(false);
    this->movement_component->set_speed(this->movement_component->get_speed()*3.0);
}

void PlayerCharacter::_enter_block_react()
{
    ERR_FAIL_NULL(this->animation_player);
    this->movement_fsm->transition_to_state(this->movement_idle);
    this->animation_player->play("Idle");
    this->hitbox_blocker->set_blocker_enabled(true);
    if (!(this->blocking_timer->is_stopped()))
    {
        // we are in blocking already
    }
    else
    {
        this->blocking_timer->start(0.5);
    }
}

void PlayerCharacter::_process_block_react(double delta)
{
    ERR_FAIL_NULL(this->input_component);
    /* will not enter her if no hitbox blocker, no need to check */
    // ERR_FAIL_NULL(this->hitbox_blocker);
    godot::Vector3 mouse_position = this->input_component->get_mouse_casted_position();
    ERR_FAIL_COND(mouse_position.is_equal_approx(this->get_position()));
    this->look_at(mouse_position);
}

void PlayerCharacter::_on_blocking_react_finished()
{
    ERR_FAIL_NULL(this->input_component);
    if (this->input_component->is_block_pressed())
    {
        this->action_fsm->transition_to_state(this->action_blocking);
    }
    else
    {
        this->action_fsm->transition_to_state(this->action_idle);
    }
}

void PlayerCharacter::_exit_block_react()
{
    this->hitbox_blocker->set_blocker_enabled(false);
}

void PlayerCharacter::hitbox_blocked(const godot::Area3D* hitbox)
{
    this->action_fsm->transition_to_state(this->action_blocking_react);
}

void PlayerCharacter::_on_hurtbox_hit(const godot::Area3D* _hitbox)
{
    ERR_FAIL_NULL(this->health_component);
    const Hitbox* hitbox = godot::Object::cast_to<Hitbox>(_hitbox);
    ERR_FAIL_NULL(hitbox);
    godot::UtilityFunctions::print(vformat("Character took damage: %d", hitbox->get_damage()));
    this->health_component->take_damage(hitbox->get_damage());
}

void PlayerCharacter::_process(double delta)
{
    this->process_movement_state();
    this->process_action_state();
}

void PlayerCharacter::_physics_process(double delta)
{
    this->movement_fsm->process_state(delta);
    this->movement_component->handle_movement(delta);
    this->action_fsm->process_state(delta);
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
