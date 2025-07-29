#ifndef MOVEMENT_STRATEGY_HPP
#define MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include "movement_strategies/movement_context.hpp"

class MovementStrategy : public godot::Resource
{
    GDCLASS(MovementStrategy, godot::Resource);

public:
    virtual void apply(MovementContext* context, double delta) const = 0;

protected:
    static void _bind_methods();
};

#endif // MOVEMENT_STRATEGY_HPP