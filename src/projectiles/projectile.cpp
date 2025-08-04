#include "projectile.hpp"

void Projectile::set_movement_strategies(godot::TypedArray<MovementStrategy> movement_strategies)
{
    this->movement_strategies = movement_strategies;
    for (int i = 0, size = this->movement_strategies.size(); i < size; i++)
    {
        MovementStrategy* movement_strategy = godot::Object::cast_to<MovementStrategy>(this->movement_strategies[i]);
        if (!movement_strategy)
        {
            continue;
        }
        if (movement_strategy->get_parent() != this)
        {
            this->add_child(movement_strategy);
        }
        if (movement_strategy->get_owner() != this) /* make sure the strategy is saved in the editor when added. */
        {
            movement_strategy->set_owner(this);
        }
    }
}

godot::TypedArray<MovementStrategy> Projectile::get_movement_strategies() const
{
    return this->movement_strategies;
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
        this->movement_context = godot::Ref<MovementContext>(memnew(MovementContext));
    }
}

void Projectile::_physics_process(double delta)
{
    ERR_FAIL_NULL(this->movement_context);
    this->movement_context->set_position(this->get_position());
    for (int i = 0, size = this->movement_strategies.size(); i < size; i++)
    {
        MovementStrategy* movement_strategy = godot::Object::cast_to<MovementStrategy>(this->movement_strategies[i]);
        ERR_CONTINUE(movement_strategy == nullptr);
        movement_strategy->apply(this->movement_context, delta);
    }
    this->set_velocity(this->movement_context->get_speed() * this->movement_context->get_direction());
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
    
    BIND_GETTER_SETTER_DEFAULT(Projectile, movement_strategies);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "movement_strategies", PROPERTY_HINT_ARRAY_TYPE, "Movement strategies.", PROPERTY_USAGE_ARRAY), "set_movement_strategies", "get_movement_strategies");
}
