#ifndef ACCELERATING_MOVEMENT_STRATEGY_HPP
#define ACCELERATING_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class AcceleratingMovementStrategy : public MovementStrategy
{
    GDCLASS(AcceleratingMovementStrategy, MovementStrategy);

private:
    double last_speed_call = 0;
    double acceleration_per_second = 1;
    double min_speed = 0;
    double max_speed = 100;

    virtual void _apply(godot::Ref<MovementContext> context, double delta) override;

public:
    CREATE_GETTER_SETTER_DEFAULT(double, acceleration_per_second);
    CREATE_GETTER_SETTER_DEFAULT(double, min_speed);
    CREATE_GETTER_SETTER_DEFAULT(double, max_speed);
    bool is_done() const override;

    virtual void _ready() override;

protected:
    static void _bind_methods();
};

#endif // ACCELERATING_MOVEMENT_STRATEGY_HPP