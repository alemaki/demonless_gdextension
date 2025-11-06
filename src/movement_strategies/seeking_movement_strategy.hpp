#ifndef SEEKING_MOVEMENT_STRATEGY_HPP
#define SEEKING_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class SeekingMovementStrategy : public MovementStrategy
{
    GDCLASS(SeekingMovementStrategy, MovementStrategy);

protected:
    double duration = 5; /* in seconds */
    bool never_ending = false;
    double time_remaining = 5;

private:
    virtual void _apply(godot::Ref<MovementContext> context, double delta) override;

public:
    bool is_done() const override;
    void _ready() override;

    CREATE_GETTER_SETTER_BOOL_DEFAULT(never_ending);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);

protected:
    static void _bind_methods();
};

#endif // SEEKING_MOVEMENT_STRATEGY_HPP