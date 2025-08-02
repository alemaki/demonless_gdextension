#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include "movement_strategies/movement_context.hpp"
#include "movement_strategies/movement_strategy.hpp"

class Projectile : public godot::CharacterBody3D
{
    GDCLASS(Projectile, godot::CharacterBody3D);

protected:
    godot::Ref<MovementContext> movement_context = nullptr;
    MovementStrategy* movement_strategy;
    godot::Timer* lifespan_timer = nullptr;

public:
    Projectile();

    virtual void _on_timeout();
    void _ready() override;
    void _physics_process(double delta) override;

    CREATE_GETTER_SETTER_DEFAULT(godot::Ref<MovementContext>, movement_context)
    CREATE_GETTER_SETTER_DEFAULT(godot::Timer*, lifespan_timer)
    void set_movement_strategy(MovementStrategy* movement_strategy);
    MovementStrategy* get_movement_strategy() const;

protected:
    static void _bind_methods();
};

#endif // PROJECTILE_HPP