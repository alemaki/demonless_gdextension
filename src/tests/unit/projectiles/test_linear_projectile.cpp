#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/godot.hpp>

#include "projectiles/linear_projectile.hpp"
#include "tests/test_utils/test_utils.hpp"


TEST_SUITE("TestLinearProjectile")
{

    TEST_CASE("Test linear projectile basic")
    {
        LinearProjectile* projectile = memnew(LinearProjectile);

        CHECK_EQ(projectile->get_speed(), doctest::Approx(0));

        godot::Vector3 direction = godot::Vector3(1, 0, 1); // Vector3.RIGHT + Vector3.FORWARD
        float speed = 100.0;
        projectile->set_direction(direction);
        projectile->set_speed(speed);

        CHECK_VECTORS_EQ(projectile->get_direction(), direction.normalized());
        CHECK_EQ(projectile->get_speed(), doctest::Approx(speed));

        memdelete(projectile);
    }

    TEST_CASE("Test linear projectile movement")
    {
        LinearProjectile* projectile = memnew(LinearProjectile);

        projectile->set_direction(godot::Vector3(1, 0, 0)); // Vector3.RIGHT
        projectile->set_speed(100.0);
        projectile->set_physics_process(true);

        double delta = ::get_current_engine_delta();
        ::simulate(projectile);
        godot::Vector3 expected_position = godot::Vector3(100, 0, 0)*delta;
        CHECK_VECTORS_EQ(projectile->get_position(), expected_position);

        memdelete(projectile);
    }
}