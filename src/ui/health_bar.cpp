#include "health_bar.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

#include "utils/utils.hpp"

HealthBar::HealthBar()
{
    this->set_show_percentage(false);
}

void HealthBar::_on_change()
{
    ERR_FAIL_NULL(this->health_component);
    this->set_value(this->health_component->get_percentage());
}

void HealthBar::set_health_component(HealthComponent* health_component)
{
    if (this->health_component != nullptr) // godot::UtilityFunctions::is_instance_valid(this->health_component)
    {
        this->health_component->disconnect("perc_health_changed", callable_mp(this, &HealthBar::_on_change));
    }

    this->health_component = health_component;

    if (this->health_component != nullptr)
    {
        this->health_component->connect("perc_health_changed", callable_mp(this, &HealthBar::_on_change));
        this->_on_change();
    }
}

void HealthBar::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(HealthBar, health_component, PROPERTY_HINT_NODE_TYPE, "HealthComponent", PROPERTY_USAGE_DEFAULT, HealthComponent);
}
