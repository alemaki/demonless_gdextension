#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include <godot_cpp/variant/utility_functions.hpp>
#include "ui/health_bar.hpp"

TEST_SUITE("TestHealthBar")
{
    TEST_CASE("Test health bar updates on health change")
    {
        godot::UtilityFunctions::print("here");
        HealthComponent* health_component = memnew(HealthComponent);
        godot::UtilityFunctions::print("here after hc");
        HealthBar* health_bar = memnew(HealthBar);
        godot::UtilityFunctions::print("here after hb");

        health_bar->set_health_component(health_component);
        godot::UtilityFunctions::print("here0");
        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        CHECK(health_bar->get_value() == doctest::Approx(50));
        godot::UtilityFunctions::print("here1");

        health_component->set_current_hp(75);
        CHECK(health_bar->get_value() == doctest::Approx(75));
        godot::UtilityFunctions::print("here2");
        health_component->take_damage(25);
        CHECK(health_bar->get_value() == doctest::Approx(50));
        godot::UtilityFunctions::print("here3");
        health_component->heal(10);
        CHECK(health_bar->get_value() == doctest::Approx(60));

        memdelete(health_bar);
        memdelete(health_component);
        godot::UtilityFunctions::print("here");
    }
}