#include "health_bar.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

HealthBar::HealthBar()
{
    this->set_show_percentage(false);
}

void HealthBar::_on_change()
{
    ERR_FAIL_NULL(this->health_component);
    double max_health = this->health_component->get_max_hp();
    double current_health = this->health_component->get_current_hp();

    double percentage;
    if (max_health == 0)
    {
        percentage = 0;
    }
    else
    {
        percentage = (current_health*100)/max_health;
    }

    this->set_value(percentage);
}

void HealthBar::set_health_component(HealthComponent* health_component)
{
    if (this->health_component != nullptr) // !!!TODO: check_instance_is_valid
    {
        godot::UtilityFunctions::print("disconnecting");
        this->health_component->disconnect("health_changed", callable_mp(this, &HealthBar::_on_change));
    }

    this->health_component = health_component;

    if (this->health_component != nullptr)
    {
        this->health_component->connect("health_changed", callable_mp(this, &HealthBar::_on_change));
    }
}

void HealthBar::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_health_component", "health_component"), &HealthBar::set_health_component);
    ClassDB::bind_method(D_METHOD("get_health_component"), &HealthBar::get_health_component);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "health_component"), "set_health_component", "get_health_component");
}
