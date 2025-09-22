#ifndef SEQUENTIAL_MOVEMENT_STRATEGY_HPP
#define SEQUENTIAL_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class SequentialMovementStrategy : public MovementStrategy
{
    GDCLASS(SequentialMovementStrategy, MovementStrategy);

private:
    int current_strategy = 0;

    virtual void _apply(godot::Ref<MovementContext> context, double delta) override;

public:
    godot::TypedArray<MovementStrategy> get_movement_strategies() const;
    bool is_done() const override;

protected:
    static void _bind_methods();
};

#endif // SEQUENTIAL_MOVEMENT_STRATEGY_HPP