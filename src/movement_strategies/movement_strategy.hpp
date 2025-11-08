#ifndef MOVEMENT_STRATEGY_HPP
#define MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_context.hpp"

class MovementStrategy : public godot::Node
{
    GDCLASS(MovementStrategy, godot::Node);

protected:
    /**
     * @brief The total duration (in seconds) that this movement strategy will run before being considered done.
     * If duration is 0, then
     */
    double duration = 0;
    /**
     * @brief Determines whether this movement strategy will ever complete.
     */
    bool never_ending = true;
    /**
     * @brief If true only checks duration, and not the virtual `_is_done`.
     */
    bool only_duration = false; //TODO: enum maybe?

    double time_remaining = 0;

private:
    /**
     * @brief Applies the movement strategy to the context based on delta time variable.
     *
     * @param context reference to the movement context to apply the strategy to.
     * @param delta the time variable for the movement strategy.
     */
    virtual void _apply(godot::Ref<MovementContext> context, double delta);

    /**
     * @brief Internal virtual check to determine if the movement strategy has finished.
     *
     * This method provides a customizable completion condition for derived movement strategies.
     * By default, it always returns `false`, meaning the base `MovementStrategy` never finishes
     * on its own unless controlled by `duration` or `never_ending` flags.
     *
     * @return `true` if the movement strategy should be considered done, `false` otherwise.
     *
     * @note This method is called internally by `is_done()`, which also considers `duration`
     *       and `never_ending` settings when determining overall completion.
     *
     * @see is_done()
     * @see duration
     * @see never_ending
     */
    virtual bool _is_done() const;

    /**
     * @brief Called after _ready(). For child definitinos.
     */
    virtual void _init(){};
public:

    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);
    CREATE_GETTER_SETTER_BOOL_DEFAULT(never_ending);
    CREATE_GETTER_SETTER_BOOL_DEFAULT(only_duration);

    /**
     * @brief Checks if the strategy is done if yes - applies the default linear behaviour implemented by
     * MovementStrategy to context based on delta time variable, if not - applies _apply() of the strategy.
     *
     * @param context reference to the movement context to apply the strategy to.
     * @param delta the time variable for the movement strategy.
     */
    void apply(godot::Ref<MovementContext> context, double delta);
    /**
     * @brief Checks whether the current movement strategy has finished executing.
     *
     * This method determines if the strategy should continue applying its motion or stop.
     * If `never_ending` is set to true, the strategy will *never* be considered done, and
     * this method will always return false.
     * Otherwise, it will check the internal timer and the custom `_is_done()` logic implemented
     * by derived classes to determine completion.
     *
     * @return `true` if the strategy has finished and should stop applying movement, `false` otherwise.
     */
    bool is_done() const;

    virtual void _ready();

protected:
    static void _bind_methods();
};

#endif /* MOVEMENT_STRATEGY_HPP */