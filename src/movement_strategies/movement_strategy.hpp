#ifndef MOVEMENT_STRATEGY_HPP
#define MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_context.hpp"

class MovementStrategy : public godot::Node
{
    GDCLASS(MovementStrategy, godot::Node);

private:
    /**
     * @brief Applies the movement strategy to the context based on delta time variable.
     *
     * @param context reference to the movement context to apply the strategy to.
     * @param delta the time variable for the movement strategy.
     */
    virtual void _apply(godot::Ref<MovementContext> context, double delta);

public:
    /**
     * @brief Checks if the strategy is done if yes - applies the default linear behaviour implemented by
     * MovementStrategy to context based on delta time variable, if not - applies _apply() of the strategy.
     *
     * @param context reference to the movement context to apply the strategy to.
     * @param delta the time variable for the movement strategy.
     */
    void apply(godot::Ref<MovementContext> context, double delta);
    /**
     * @brief Checks if the movement strategy is finished with executing itself.
     * Child classes define different meanings of is_done.
     * @return true if done
     * @return false if not done
     */
    virtual bool is_done() const;

protected:
    static void _bind_methods();
};

#endif /* MOVEMENT_STRATEGY_HPP */