#ifndef LASER_HPP
#define LASER_HPP

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include "components/area3d/hurtbox.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hitbox.hpp"
#include "movement_strategies/movement_context.hpp"
#include "movement_strategies/movement_strategy.hpp"

class Laser : public godot::CharacterBody3D
{
    GDCLASS(Laser, godot::CharacterBody3D);

protected:
    double length = 1000;
    godot::Ref<MovementContext> movement_context = nullptr;
    MovementStrategy* movement_strategy = nullptr;
    godot::Timer* lifespan_timer = nullptr;
    godot::RayCast3D* ray_cast = nullptr;
    Hitbox* hitbox = nullptr;
    godot::Node* actor_source = nullptr;
    godot::MeshInstance3D* mesh = nullptr;

    virtual void _hit_hurtbox(Hurtbox* hurtbox);
    virtual void _hit_blocker(HitboxBlocker* hitbox_blocker);

    virtual void _on_timeout();

public:
    Laser();

    void _ready() override;
    void _physics_process(double delta) override;

    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, length);
    CREATE_GETTER_SETTER_DEFAULT(godot::Timer*, lifespan_timer);
    CREATE_GETTER_SETTER_DEFAULT(godot::RayCast3D*, ray_cast);
    CREATE_GETTER_SETTER_DEFAULT(Hitbox*, hitbox);
    CREATE_GETTER_SETTER_DEFAULT(godot::MeshInstance3D*, mesh)

    void set_actor_source(godot::Node* actor_source);
    _FORCE_INLINE_ godot::Node* get_actor_source() const
    {
        return this->actor_source;
    }

    void set_movement_context(godot::Ref<MovementContext> movement_context);
    _FORCE_INLINE_ godot::Ref<MovementContext> get_movement_context() const
    {
        return this->movement_context;
    }

    void set_movement_strategy(MovementStrategy* movement_strategy);
    _FORCE_INLINE_ MovementStrategy* get_movement_strategy() const
    {
        return this->movement_strategy;
    }

    void set_direction(const godot::Vector3& direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->movement_context->get_direction();
    }

protected:
    static void _bind_methods();
};

#endif // LASER_HPP