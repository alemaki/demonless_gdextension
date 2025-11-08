#ifndef SEEKING_MOVEMENT_STRATEGY_HPP
#define SEEKING_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class SeekingMovementStrategy : public MovementStrategy
{
    GDCLASS(SeekingMovementStrategy, MovementStrategy);

private:
    virtual void _apply(godot::Ref<MovementContext> context, double delta) override;

protected:
    static void _bind_methods();
};

#endif // SEEKING_MOVEMENT_STRATEGY_HPP