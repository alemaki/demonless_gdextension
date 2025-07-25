#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "attacks/circle_wave_attack.hpp"
#include "utils/utils.hpp"

struct CircleWaveAttackFixture 
{
    CircleWaveAttack* attack = memnew(CircleWaveAttack);
    godot::Node3D* dummy_source = memnew(godot::Node3D);
    Projectile* dummy_projectile = memnew(Projectile);

    CircleWaveAttackFixture()
    {
        attack->set_actor_source(dummy_source);
        attack->set_projectile(dummy_projectile);
    }

    ~CircleWaveAttackFixture()
    {
        memdelete(attack);
        memdelete(dummy_projectile);
        memdelete(dummy_source);
    }
};


TEST_SUITE("TestCircleWaveAttack")
{
    TEST_CASE_FIXTURE(CircleWaveAttackFixture, "CircleWaveAttack basic")
    {
        attack->set_wave_cooldown(2.0);
        CHECK(attack->get_wave_cooldown() == doctest::Approx(2.0));

        attack->set_waves(3);
        CHECK(attack->get_waves() == 3);

        attack->set_projectiles_per_wave(8);
        CHECK(attack->get_projectiles_per_wave() == 8);

        attack->set_projectile(dummy_projectile);
        CHECK(attack->get_projectile() == dummy_projectile);

        godot::Vector3 dir = {1, 0, 1};
        attack->set_direction(dir);
        CHECK_VECTORS_EQ(attack->get_direction(), dir);
    }

    TEST_CASE_FIXTURE(CircleWaveAttackFixture, "Fires waves on time accumulation")
    {
        attack->set_waves(2);
        attack->set_projectiles_per_wave(0);
        attack->set_duration(2.0);
        attack->step(1.0);
        CHECK(attack->get_time_accumulated() == doctest::Approx(1.0));
        CHECK(attack->get_waves_fired() == doctest::Approx(1.0));
        attack->step(1.0);
        CHECK(attack->get_time_accumulated() == doctest::Approx(2.0));
        CHECK(attack->get_waves_fired() == doctest::Approx(2.0));
    }

    TEST_CASE_FIXTURE(CircleWaveAttackFixture, "Reset clears waves_fired")
    {
        attack->set_projectile(dummy_projectile);
        attack->set_actor_source(dummy_source);
        attack->step(1.0);
        CHECK(attack->get_time_accumulated() == doctest::Approx(1.0));
        attack->reset();
        CHECK(attack->get_time_accumulated() == doctest::Approx(0.0));
        CHECK(attack->get_waves_fired() == doctest::Approx(0.0));
    }
}

TEST_SUITE("[errors] TestCircleWaveAttack")
{
    TEST_CASE("_step fails on nulls and zero direction")
    {
        CircleWaveAttack* attack = memnew(CircleWaveAttack);
        CHECK_GODOT_ERROR(attack->step(0.1)); // actor_source, projectile are null

        godot::Node3D* dummy_source = memnew(godot::Node3D);
        attack->set_actor_source(dummy_source);
        CHECK_GODOT_ERROR(attack->step(0.1)); // projectile still null

        Projectile* dummy_projectile = memnew(Projectile);
        attack->set_projectile(dummy_projectile);
        CHECK_GODOT_ERROR(attack->set_direction({0, 0, 0})); // Zero direction

        attack->set_wave_cooldown(0);
        CHECK_GODOT_ERROR(attack->step(0.1)); // divison by cooldown 0

        attack->set_wave_cooldown(1);

        attack->step(1); // No error

        memdelete(dummy_source);
        memdelete(dummy_projectile);
        memdelete(attack);
    }
}