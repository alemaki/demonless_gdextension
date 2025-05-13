#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include <godot_cpp/variant/utility_functions.hpp>
#include "ui/health_bar.hpp"
#include "attacks/damage_info.hpp"

TEST_SUITE("TestHealthBar")
{
    TEST_CASE("Test health bar updates on health change")
    {
        HealthComponent* health_component = memnew(HealthComponent);
        HealthBar* health_bar = memnew(HealthBar);

        health_bar->set_health_component(health_component);
        health_component->set_max_hp(100);
        health_component->set_current_hp(50);
        CHECK_EQ(health_bar->get_value(), doctest::Approx(50));

        health_component->set_current_hp(75);
        CHECK_EQ(health_bar->get_value(), doctest::Approx(75));

        godot::Ref<DamageInfo> damage_info;
        damage_info.instantiate();
        damage_info->set_damage(25);
        health_component->take_damage(damage_info);
        CHECK_EQ(health_bar->get_value(), doctest::Approx(50));

        health_component->heal(10);
        CHECK_EQ(health_bar->get_value(), doctest::Approx(60));

        memdelete(health_bar);
        memdelete(health_component);
    }
}