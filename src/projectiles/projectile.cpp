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
    if (this->movement_strategy)
    {
        this->movement_strategy->queue_free();
    }
    this->movement_strategy = movement_strategy;
    ERR_FAIL_NULL(movement_strategy); /* set strategy to null so no segfaults can happen and fail after so it can alert */
    if (this->movement_strategy->get_parent() != this)
    {
        this->add_child(movement_strategy);
    }
    if (this->movement_strategy->get_owner() != this) /* make sure the strategy is saved in the editor when added. */
    {
        movement_strategy->set_owner(this);
    }
}

void Projectile::set_direction(const godot::Vector3& direction)
{
    ERR_FAIL_COND(direction.is_zero_approx());
    this->movement_context->set_direction(direction);
    this->look_at(this->get_position() + direction);
}

void Projectile::_hit(godot::Area3D* area3d)
{
    if (auto* hurtbox = Object::cast_to<Hurtbox>(area3d))
    {
        this->_hit_hurtbox(hurtbox);
    }
    if (auto* hitbox_blocker = Object::cast_to<HitboxBlocker>(area3d))
    {
        this->_hit_blockbox(hitbox_blocker);
    }
}

void Projectile::_hit_hurtbox(Hurtbox *hurtbox)
{
    /* Does nothing for now. */
}

void Projectile::_hit_blockbox(HitboxBlocker *hitbox_blocker)
{
    this->queue_free();
}

void Projectile::_on_timeout()
{
    this->queue_free();
}

void Projectile::_ready()
{
    if (!this->lifespan_timer)
    {
        this->lifespan_timer = memnew(godot::Timer);
        this->add_child(this->lifespan_timer);
        this->lifespan_timer->set_autostart(true);
        this->lifespan_timer->set_one_shot(true);
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
    this->move_and_slide();
}


void Projectile::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_DEFAULT(Projectile, lifespan_timer);
    ClassDB::bind_method(D_METHOD("_on_timeout"), &Projectile::_on_timeout);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lifespan_timer", PROPERTY_HINT_NODE_TYPE, "Timer", PROPERTY_USAGE_DEFAULT, "Timer"), "set_lifespan_timer", "get_lifespan_timer");

    BIND_GETTER_SETTER_DEFAULT(Projectile, movement_context);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_context", PROPERTY_HINT_RESOURCE_TYPE, "Movement context.", PROPERTY_USAGE_DEFAULT), "set_movement_context", "get_movement_context");

    BIND_GETTER_SETTER_DEFAULT(Projectile, movement_strategy);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_strategy", PROPERTY_HINT_NODE_TYPE, "Movement strategy.", PROPERTY_USAGE_EDITOR, "MovementStrategy"), "set_movement_strategy", "get_movement_strategy");
}
