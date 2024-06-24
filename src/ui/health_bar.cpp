#include "health_bar.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

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

    ClassDB::bind_method(D_METHOD("set_health_component", "health_component"), &HealthBar::set_health_component);
    ClassDB::bind_method(D_METHOD("get_health_component"), &HealthBar::get_health_component);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "health_component"), "set_health_component", "get_health_component");
}
