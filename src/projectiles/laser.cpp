#include "laser.hpp"

void Laser::_hit_hurtbox(Hurtbox *hurtbox)
{
    /* Laser stays alive on hit, unlike Projectile - it relies on Area3D's
       one-shot-per-overlap area_entered semantics for "hits once". */
}

void Laser::_hit_blocker(HitboxBlocker *hitbox_blocker)
{
    /* TODO: What happens when a laser hits the blocker */
}

void Laser::_on_timeout()
{
    this->queue_free();
}

Laser::Laser()
{

}

void Laser::_ready()
{
    DISABLE_EDITOR_PROCESSING();

    utils::ensure_node(ray_cast, this, "RayCast3D");
    if (this->ray_cast)
    {
        this->ray_cast->set_physics_process(false);
    }

    utils::ensure_node_silent(this->hitbox, this, "Hitbox");
    if (this->hitbox)
    {
        this->hitbox->set_actor_source(this->actor_source);
        this->hitbox->set_monitoring(true);
        this->hitbox->connect("hit_hurtbox", callable_mp(this, &Laser::_hit_hurtbox));
        this->hitbox->connect("hit_blocker", callable_mp(this, &Laser::_hit_blocker));
    }

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
    if (!(this->movement_strategy))
    {
        this->set_movement_strategy(memnew(MovementStrategy));
        this->add_child(movement_strategy);
    }
}

void Laser::_physics_process(double delta)
{
    ERR_FAIL_NULL_MSG(this->ray_cast, vformat("%s: Laser must have a RayCast3D.", this->get_name()));
    ERR_FAIL_NULL_MSG(this->movement_context, vformat("%s: Laser must have a MovementContext.", this->get_name()));
    ERR_FAIL_NULL_MSG(this->movement_strategy, vformat("%s: Laser must have a MovementStrategy.", this->get_name()));
    this->movement_context->set_position(this->get_position());
    this->movement_strategy->apply(this->movement_context, delta);
    godot::Vector3 displacement = this->movement_context->get_position() - this->get_position();
    this->set_velocity(displacement / delta); // velocity to reach in one frame
    this->look_at(this->get_position() + displacement);
    this->move_and_slide();
    this->ray_cast->force_raycast_update();
    double real_length;
    if (this->ray_cast->is_colliding())
    {
        real_length = (this->get_global_position() - this->ray_cast->get_collision_point()).length();
    }
    else
    {
        real_length = this->length;
    }
    if (this->mesh)
    {
        this->mesh->set_scale(godot::Vector3(1, 1, real_length));
    }
    if (this->hitbox)
    {
        this->hitbox->set_scale(godot::Vector3(1, 1, real_length));
    }
}

void Laser::set_actor_source(godot::Node* _actor_source)
{
    this->actor_source = _actor_source;
    if (this->hitbox)
    {
        this->hitbox->set_actor_source(_actor_source);
    }
}

void Laser::set_movement_context(godot::Ref<MovementContext> movement_context)
{
    ERR_FAIL_COND(movement_context.is_null());
    this->movement_context = movement_context;
}

void Laser::set_direction(const godot::Vector3& direction)
{
    ERR_FAIL_COND(direction.is_zero_approx());
    this->movement_context->set_direction(direction);
}

void Laser::set_movement_strategy(MovementStrategy *movement_strategy)
{
    if (!movement_strategy)
    {
        return;
    }
    this->movement_strategy = movement_strategy;
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

void Laser::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("_on_timeout"), &Laser::_on_timeout);

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(Laser, FLOAT, length);

    BIND_GETTER_SETTER_DEFAULT(Laser, lifespan_timer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lifespan_timer", PROPERTY_HINT_NODE_TYPE, "Timer", PROPERTY_USAGE_DEFAULT, "Timer"), "set_lifespan_timer", "get_lifespan_timer");

    BIND_GETTER_SETTER_DEFAULT(Laser, ray_cast);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ray_cast", PROPERTY_HINT_NODE_TYPE, "RayCast3D", PROPERTY_USAGE_DEFAULT, "RayCast3D"), "set_ray_cast", "get_ray_cast");

    BIND_GETTER_SETTER_DEFAULT(Laser, mesh);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_NODE_TYPE, "MeshInstance3D", PROPERTY_USAGE_DEFAULT, "MeshInstance3D"), "set_mesh", "get_mesh");

    BIND_GETTER_SETTER_DEFAULT(Laser, movement_context);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_context", PROPERTY_HINT_RESOURCE_TYPE, "MovementContext", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ALWAYS_DUPLICATE, "MovementContext"), "set_movement_context", "get_movement_context");

    BIND_GETTER_SETTER_DEFAULT(Laser, movement_strategy);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "movement_strategy", PROPERTY_HINT_NODE_TYPE, "MovementStrategy", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ALWAYS_DUPLICATE, "MovementStrategy"), "set_movement_strategy", "get_movement_strategy");
}
