#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include "components/area3d/hurtbox.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hitbox.hpp"
#include "movement_strategies/movement_context.hpp"
#include "movement_strategies/movement_strategy.hpp"

class Projectile : public godot::CharacterBody3D
{
    GDCLASS(Projectile, godot::CharacterBody3D);

protected:
    godot::Ref<MovementContext> movement_context = nullptr;
    MovementStrategy* movement_strategy = nullptr;
    godot::Timer* lifespan_timer = nullptr;
    Hitbox* hitbox = nullptr;

    void _hit(godot::Area3D* area3d);
    virtual void _hit_hurtbox(Hurtbox* hurtbox);
    virtual void _hit_blockbox(HitboxBlocker* hitbox_blocker);

    virtual void _on_timeout();

public:
    Projectile();

    void _ready() override;
    void _physics_process(double delta) override;

    CREATE_GETTER_SETTER_DEFAULT(godot::Timer*, lifespan_timer);
    void set_movement_context(godot::Ref<MovementContext> movement_context);
    godot::Ref<MovementContext> get_movement_context() const
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

#endif // PROJECTILE_HPP