#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/health/health_component.hpp"
#include "tests/test_utils/signal_watcher.hpp"

struct HealthComponentFixture
{
    HealthComponent* health_component;

    HealthComponentFixture() : health_component(memnew(HealthComponent)){}
    ~HealthComponentFixture()
    {
        memdelete(health_component);
    }
};

TEST_SUITE("HealthComponentTests")
{
    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component basics")
    {
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
    }

    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component take damage")
    {
        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        health_component->take_damage(20);
        CHECK_EQ(health_component->get_current_hp(), 30);

        health_component->take_damage(40);
        CHECK_EQ(health_component->get_current_hp(), 0);

        health_component->take_damage(-20);
        CHECK_EQ(health_component->get_current_hp(), 0);
    }

    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component heal")
    {
        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        health_component->heal(20);
        CHECK_EQ(health_component->get_current_hp(), 70);

        health_component->heal(31);
        CHECK_EQ(health_component->get_current_hp(), 100);

        health_component->set_current_hp(50);
        health_component->heal(-20);
        CHECK_EQ(health_component->get_current_hp(), 50);
    }

    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component health_depleted signal")
    {
        health_component->set_max_hp(10);
        health_component->set_current_hp(2);

        REQUIRE(health_component->has_signal("health_depleted"));

        SignalWatcher::watch_signals(health_component);

        health_component->take_damage(1);
        CHECK_FALSE(SignalWatcher::signal_emitted(health_component, godot::String("health_depleted")));

        health_component->take_damage(1);
        CHECK(SignalWatcher::signal_emitted(health_component, godot::String("health_depleted")));
        CHECK_EQ(SignalWatcher::get_signal_emitted_count(health_component, godot::String("health_depleted")), 1);
    }

    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component perc_health_changed signal")
    {
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
    }
}