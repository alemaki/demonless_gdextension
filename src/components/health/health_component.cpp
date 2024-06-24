#include "health_component.hpp"
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void HealthComponent::recalculate_percentage()
{
    double new_percentage = 100;
    if (this->max_hp != 0)
    {
        new_percentage = (this->current_hp*100)/this->max_hp;
    }
    if (!(godot::Math::is_equal_approx(this->percentage, new_percentage)))
    {
        this->percentage = new_percentage;
        this->emit_signal("perc_health_changed");
    }
}

void HealthComponent::set_max_hp(double max_hp)
{
    max_hp = godot::Math::clamp<double>(max_hp, 0, max_hp);
    this->max_hp = max_hp;
    this->set_current_hp(this->current_hp);
}

void HealthComponent::set_current_hp(double current_hp)
{
    current_hp = godot::Math::clamp<double>(current_hp, 0, this->max_hp);
    this->current_hp = current_hp;
    this->recalculate_percentage();
}

void HealthComponent::apply_amount(double amount)
{
    this->current_hp += amount;
    if (this->current_hp <= 0)
    {
        this->current_hp = 0;
        this->emit_signal("health_depleted");
    }
    if (this->current_hp > this->max_hp)
    {
        this->current_hp = this->max_hp;
    }
    this->recalculate_percentage();
}

void HealthComponent::take_damage(double amount)
{
    amount = godot::Math::clamp<double>(amount, 0, amount);
    this->apply_amount(-amount);
}

void HealthComponent::heal(double amount)
{
    amount = godot::Math::clamp<double>(amount, 0, amount);
    this->apply_amount(amount);
}

void HealthComponent::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("take_damage"), &HealthComponent::take_damage);
    ClassDB::bind_method(D_METHOD("heal"), &HealthComponent::heal);


    ClassDB::bind_method(D_METHOD("set_current_hp", "current_hp"), &HealthComponent::set_current_hp);
    ClassDB::bind_method(D_METHOD("set_max_hp", "max_hp"), &HealthComponent::set_max_hp);
    ClassDB::bind_method(D_METHOD("get_current_hp"), &HealthComponent::get_current_hp);
    ClassDB::bind_method(D_METHOD("get_max_hp"), &HealthComponent::get_max_hp);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_hp"), "set_max_hp", "get_max_hp");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "current_hp"), "set_current_hp", "get_current_hp");

    ADD_SIGNAL(MethodInfo("health_depleted"));
    ADD_SIGNAL(MethodInfo("perc_health_changed"));
}


