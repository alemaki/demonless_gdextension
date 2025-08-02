#include "composite_movement_strategy.hpp"
   
void CompositeMovementStrategy::set_movement_strategies(const godot::TypedArray<MovementStrategy>& movement_strategies)
{
    this->movement_strategies = movement_strategies;
    for (int i = 0, size = this->movement_strategies.size(); i < size; i++)
    {
        MovementStrategy* movement_strategy = Object::cast_to<MovementStrategy>(this->movement_strategies[i]);
        if (!movement_strategy)
        {
            return;
        }
        if (movement_strategy->get_parent() != this)
        {
            this->add_child(movement_strategy);
        }
        if (movement_strategy->get_owner() != this) /* make sure the strategy is saved in the editor when added. */
        {
            movement_strategy->set_owner(this);
        }
    }
}

godot::TypedArray<MovementStrategy> CompositeMovementStrategy::get_movement_strategies() const
{
    return this->movement_strategies;
}

void CompositeMovementStrategy::apply(godot::Ref<MovementContext> context, double delta) const
{
    for (int i = 0, size = this->movement_strategies.size(); i < size; i++)
    {
        MovementStrategy* movement_strategy = godot::Object::cast_to<MovementStrategy>(this->movement_strategies[i]);
        ERR_CONTINUE(movement_strategy == nullptr);
        movement_strategy->apply(context, delta);
    }
}

void CompositeMovementStrategy::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_DEFAULT(CompositeMovementStrategy, movement_strategies);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "movement_strategies", PROPERTY_HINT_ARRAY_TYPE, "Movement strategies.", PROPERTY_USAGE_ARRAY), "set_movement_strategy", "get_movement_strategy");

}