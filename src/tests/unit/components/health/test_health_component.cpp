#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/health/health_component.hpp"
#include "attacks/damage_info.hpp"
#include "tests/test_utils/signal_observer.hpp"
#include "utils/utils.hpp"

static godot::Ref<DamageInfo> make_damage_info(double amount, godot::Node* source = nullptr)
{
    godot::Ref<DamageInfo> damage_info;
    damage_info.instantiate();
    damage_info->set_damage(amount);
    damage_info->set_source(source);
    return damage_info;
}

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

        health_component->take_damage(make_damage_info(20));
        CHECK_EQ(health_component->get_current_hp(), 30);

        health_component->take_damage(make_damage_info(40));
        CHECK_EQ(health_component->get_current_hp(), 0);

        health_component->take_damage(make_damage_info(-20));
        CHECK_EQ(health_component->get_current_hp(), 0);
    }

    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component take damage carries source through")
    {
        godot::Node* source = memnew(godot::Node);
        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        godot::Ref<DamageInfo> damage_info = make_damage_info(15, source);
        health_component->take_damage(damage_info);

        CHECK_EQ(health_component->get_current_hp(), 35);
        CHECK_EQ(damage_info->get_source(), source);

        memdelete(source);
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

        SignalObserver::watch_signals(health_component);

        health_component->take_damage(make_damage_info(1));
        CHECK_FALSE(SignalObserver::signal_emitted(health_component, godot::String("health_depleted")));

        health_component->take_damage(make_damage_info(1));
        CHECK(SignalObserver::signal_emitted(health_component, godot::String("health_depleted")));
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("health_depleted")), 1);
    }

    TEST_CASE_FIXTURE(HealthComponentFixture, "Test health component perc_health_changed signal")
    {
        REQUIRE(health_component->has_signal("perc_health_changed"));

        SignalObserver::watch_signals(health_component);

        health_component->set_max_hp(10);
        CHECK(SignalObserver::signal_emitted(health_component, godot::String("perc_health_changed")));
        health_component->set_current_hp(0);
        CHECK(SignalObserver::signal_emitted(health_component, godot::String("perc_health_changed")));
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 1);

        REQUIRE(health_component->has_signal("perc_health_changed"));

        health_component->take_damage(make_damage_info(1));
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 1);

        health_component->set_current_hp(5);
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 2);
        health_component->take_damage(make_damage_info(1));
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 3);
        health_component->take_damage(make_damage_info(1));
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 4);
        health_component->heal(1);
        CHECK_EQ(SignalObserver::get_signal_emitted_count(health_component, godot::String("perc_health_changed")), 5);
    }
}

TEST_SUITE("[errors] HealthComponentTests")
{
    TEST_CASE_FIXTURE(HealthComponentFixture, "take_damage fails with a null damage_info")
    {
        health_component->set_max_hp(100);
        health_component->set_current_hp(50);

        CHECK_GODOT_ERROR(health_component->take_damage(nullptr));
        CHECK_EQ(health_component->get_current_hp(), 50);
    }
}