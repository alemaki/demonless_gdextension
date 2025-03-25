#ifndef COMPOSITE_MOVEMENT_STRATEGY_HPP
#define COMPOSITE_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class CompositeMovementStrategy : public MovementStrategy
{
    GDCLASS(CompositeMovementStrategy, MovementStrategy);

private:
    virtual void _apply(godot::Ref<MovementContext> context, double delta) override;

    /**
     * @brief One persistent context per child, indexed like get_child(i). Each child only
     * ever sees its own context, so state it owns (e.g. an AcceleratingMovementStrategy's
     * ramped-up speed) survives across ticks without a sibling's apply() call overwriting it.
     * Position/direction/target are resynced from the real context every tick; speed is left
     * alone so each child's own ramp continues from where it left off.
     */
    godot::Vector<godot::Ref<MovementContext>> child_contexts;

public:
    godot::TypedArray<MovementStrategy> get_movement_strategies() const;
    bool _is_done() const override;

    void _init() override;

protected:
    static void _bind_methods();
};

#endif // COMPOSITE_MOVEMENT_STRATEGY_HPP