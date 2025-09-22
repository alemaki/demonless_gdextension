#ifndef MOVEMENT_STRATEGY_HPP
#define MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_context.hpp"

class MovementStrategy : public godot::Node
{
    GDCLASS(MovementStrategy, godot::Node);

private:
    double duration = 5;
    double time_passed = 0;

private:
    virtual void _apply(godot::Ref<MovementContext> context, double delta);

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);

    void apply(godot::Ref<MovementContext> context, double delta);
    virtual bool is_done() const;
protected:
    static void _bind_methods();
};

#endif // MOVEMENT_STRATEGY_HPP