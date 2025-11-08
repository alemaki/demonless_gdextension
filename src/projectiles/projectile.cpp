#include "projectile.hpp"

Projectile::Projectile()
{

}

void Projectile::set_movement_context(godot::Ref<MovementContext> movement_context)
{
    ERR_FAIL_COND(movement_context.is_null());
    this->movement_context = movement_context;
}

void Projectile::set_movement_strategy(MovementStrategy* movement_strategy)
{
    this->movement_strategy = movement_strategy;
    if (!this->movement_strategy)
    {
        return;
    }
    if (this->movement_strategy->get_parent() != this)
    {
        this->add_child(this->movement_strategy);
    }
    if (this->movement_strategy->get_owner() != this)
    {
        /* make sure the strategy is saved in the editor when added. */
        movement_strategy->set_owner(this);
    }
}

void Projectile::set_direction(const godot::Vector3& direction)
{
    ERR_FAIL_COND(direction.is_zero_approx());
    this->movement_context->set_direction(direction);
}

void Projectile::_hit_hurtbox(Hurtbox *hurtbox)
{
    this->queue_free();
}

void Projectile::_hit_blocker(HitboxBlocker *hitbox_blocker)
{
    this->queue_free();
}

void Projectile::_on_timeout()
{
    this->queue_free();
}

void Projectile::_ready()
{
    DISABLE_EDITOR_PROCESSING();

    utils::ensure_node(this->hitbox, this, "Hitbox");
    ERR_FAIL_NULL(this->hitbox);

    this->hitbox->set_damage(this->damage);
    this->hitbox->connect("hit_hurtbox", callable_mp(this, &Projectile::_hit_hurtbox));
    this->hitbox->connect("hit_blocker", callable_mp(this, &Projectile::_hit_blocker));

    if (!this->lifespan_timer)
    {
        this->lifespan_timer = memnew(godot::Timer);
        this->lifespan_timer->set_wait_time(10); // 10 seconds default?
        this->lifespan_timer->set_autostart(true);
        this->lifespan_timer->set_one_shot(true);
        this->add_child(this->lifespan_timer);
    }

    this->lifespan_timer->connect("timeout", godot::Callable(this, "_on_timeout"));

    if (this->lifespan_timer->get_parent() != this)
    {
        this->add_child(this->lifespan_timer);
    }
    if (this->movement_context.is_null())
    {
        this->movement_context.instantiate();
    }
    if (!this->movement_strategy)
    {
        this->set_movement_strategy(memnew(MovementStrategy));
    }
}

void Projectile::_physics_process(double delta)
{
    ERR_FAIL_NULL(this->movement_context);
    ERR_FAIL_NULL(this->movement_strategy);
    this->movement_context->set_position(this->get_position());
    this->movement_strategy->apply(this->movement_context, delta);
    godot::Vector3 displacement = this->movement_context->get_position()- this->get_position();
    this->set_velocity(displacement / delta); // velocity to reach in one frame
    this->look_at(this->get_position().direction_to(this->movement_context->get_position()));
    this->move_and_slide();
}


void Projectile::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(Projectile, Variant::FLOAT ,damage);

    BIND_GETTER_SETTER_DEFAULT(Projectile, lifespan_timer);
    ClassDB::bind_method(D_METHOD("_on_timeout"), &Projectile::_on_timeout);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lifespan_timer", PROPERTY_HINT_NODE_TYPE, "Timer", PROPERTY_USAGE_DEFAULT, "Timer"), "set_lifespan_timer", "get_lifespan_timer");

    BIND_GETTER_SETTER_DEFAULT(Projectile, movement_context);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_context", PROPERTY_HINT_RESOURCE_TYPE, "MovementContext", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ALWAYS_DUPLICATE, "MovementContext"), "set_movement_context", "get_movement_context");

    BIND_GETTER_SETTER_DEFAULT(Projectile, movement_strategy);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_strategy", PROPERTY_HINT_NODE_TYPE, "MovementStrategy", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ALWAYS_DUPLICATE, "MovementStrategy"), "set_movement_strategy", "get_movement_strategy");
}
