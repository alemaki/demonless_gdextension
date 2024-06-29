#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/health/health_component.hpp"
#include "tests/test_utils/signal_watcher.hpp"

TEST_SUITE("HealthComponentTests")
{

    TEST_CASE("Test health component basics")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        CHECK_EQ(health_component->get_max_hp(), 0);
        CHECK_EQ(health_component->get_current_hp(), 0);

        health_component->set_max_hp(100);
        CHECK_EQ(health_component->get_max_hp(), 100);

        health_component->set_max_hp(-10);
        CHECK_EQ(health_component->get_max_hp(), 0);

        health_component->set_current_hp(-10);
        CHECK_EQ(health_component->get_current_hp(), 0);

        health_component->set_max_hp(10);
        health_component->set_current_hp(10);
        health_component->set_max_hp(5);
        CHECK_EQ(health_component->get_current_hp(), 5);

        health_component->set_max_hp(10);
        health_component->set_current_hp(20);
        CHECK_EQ(health_component->get_current_hp(), 10);

        health_component->set_current_hp(-1);
        CHECK_EQ(health_component->get_current_hp(), 0);

        memdelete(health_component);
    }

    TEST_CASE("Test health component take damage")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        health_component->take_damage(20);
        CHECK_EQ(health_component->get_current_hp(), 30);

        health_component->take_damage(40);
        CHECK_EQ(health_component->get_current_hp(), 0);

        health_component->take_damage(-20);
        CHECK_EQ(health_component->get_current_hp(), 0);

        memdelete(health_component);
    }

    TEST_CASE("Test health component heal")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        health_component->heal(20);
        CHECK_EQ(health_component->get_current_hp(), 70);

        health_component->heal(31);
        CHECK_EQ(health_component->get_current_hp(), 100);

        health_component->set_current_hp(50);
        health_component->heal(-20);
        CHECK_EQ(health_component->get_current_hp(), 50);

        memdelete(health_component);
    }

    TEST_CASE("Test health component health_depleted signal")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        health_component->set_max_hp(10);
        health_component->set_current_hp(2);

        REQUIRE(health_component->has_signal("health_depleted"));

        SignalWatcher::watch_signals(health_component);

        health_component->take_damage(1);
        CHECK_FALSE(SignalWatcher::signal_emitted(health_component, godot::String("health_depleted")));

        health_component->take_damage(1);
        CHECK(SignalWatcher::signal_emitted(health_component, godot::String("health_depleted")));
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("health_depleted")), 1);

        memdelete(health_component);
    }

    TEST_CASE("Test health component perc_health_changed signal")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        REQUIRE(health_component->has_signal("perc_health_changed"));

        SignalWatcher::watch_signals(health_component);

        health_component->set_max_hp(10);
        CHECK(SignalWatcher::signal_emitted(health_component, godot::String("perc_health_changed")));
        health_component->set_current_hp(0);
        CHECK(SignalWatcher::signal_emitted(health_component, godot::String("perc_health_changed")));
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 1);

        REQUIRE(health_component->has_signal("perc_health_changed"));

        health_component->take_damage(1);
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 1);

        health_component->set_current_hp(5);
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 2);
        health_component->take_damage(1);
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 3);
        health_component->take_damage(1);
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 4);
        health_component->heal(1);
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 5);

        memdelete(health_component);
    }

}