#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "attacks/attack.hpp"
#include "utils/utils.hpp"


TEST_SUITE("TestAttack")
{
    TEST_CASE("Attack basic")
    {
        Attack* attack = memnew(Attack);

        CHECK(attack->get_duration() == doctest::Approx(1.0));
        attack->set_duration(3.5);
        CHECK(attack->get_duration() == doctest::Approx(3.5));

        godot::Node3D* dummy = memnew(godot::Node3D);

        attack->set_actor_source(dummy);
        CHECK(attack->get_actor_source() == dummy);

        memdelete(dummy);
        memdelete(attack);
    }

    TEST_CASE("Reset behavior")
    {
        Attack* attack = memnew(Attack);

        attack->step(1.0);
        CHECK(attack->get_time_accumulated() == doctest::Approx(1.0));

        attack->reset();
        CHECK(attack->get_time_accumulated() == doctest::Approx(0.0));

        memdelete(attack);
    }

    TEST_CASE("Step and is_finished logic")
    {
        Attack* attack = memnew(Attack);
        attack->set_duration(2.0);

        attack->step(1.0);
        CHECK_FALSE(attack->is_finished());

        attack->step(1.5);
        CHECK(attack->is_finished());
        CHECK(attack->get_time_accumulated() == doctest::Approx(2.5));

        memdelete(attack);
    }
}
