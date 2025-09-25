#ifndef COMPOSITE_MOVEMENT_STRATEGY_HPP
#define COMPOSITE_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class CompositeMovementStrategy : public MovementStrategy
{
    GDCLASS(CompositeMovementStrategy, MovementStrategy);

private:
    virtual void _apply(godot::Ref<MovementContext> context, double delta) override;

public:
    godot::TypedArray<MovementStrategy> get_movement_strategies() const;
    bool is_done() const override;

    void _ready() override;

protected:
    static void _bind_methods();
};

#endif // COMPOSITE_MOVEMENT_STRATEGY_HPP