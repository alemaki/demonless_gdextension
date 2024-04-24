#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/health/health_component.hpp"
#include "tests/test_utils/signal_watcher.hpp"

TEST_SUITE("HealthComponentTests")
{

    TEST_CASE("Test health component basics")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        CHECK(health_component->get_max_hp() == 0);
        CHECK(health_component->get_current_hp() == 0);

        health_component->set_max_hp(100);
        CHECK(health_component->get_max_hp() == 100);

        health_component->set_max_hp(-10);
        CHECK(health_component->get_max_hp() == 0);

        health_component->set_current_hp(-10);
        CHECK(health_component->get_current_hp() == 0);

        health_component->set_max_hp(10);
        health_component->set_current_hp(10);
        health_component->set_max_hp(5);
        CHECK(health_component->get_current_hp() == 5);

        health_component->set_max_hp(10);
        health_component->set_current_hp(20);
        CHECK(health_component->get_current_hp() == 10);

        health_component->set_current_hp(-1);
        CHECK(health_component->get_current_hp() == 0);

        memfree(health_component);
    }

    TEST_CASE("Test health component take damage")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        health_component->take_damage(20);
        CHECK(health_component->get_current_hp() == 30);

        health_component->take_damage(40);
        CHECK(health_component->get_current_hp() == 0);

        health_component->take_damage(-20);
        CHECK(health_component->get_current_hp() == 0);

        memfree(health_component);
    }

    TEST_CASE("Test health component heal")
    {
        HealthComponent* health_component = memnew(HealthComponent);

        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        health_component->heal(20);
        CHECK(health_component->get_current_hp() == 70);

        health_component->heal(31);
        CHECK(health_component->get_current_hp() == 100);

        health_component->set_current_hp(50);
        health_component->heal(-20);
        CHECK(health_component->get_current_hp() == 50);

        memfree(health_component);
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
        CHECK(SignalWatcher::get_signal_emitted_count(health_component, godot::String("health_depleted")) == 0);

        health_component->take_damage(1);
        CHECK(SignalWatcher::signal_emitted(health_component, godot::String("health_depleted")));

        memfree(health_component);
    }

}