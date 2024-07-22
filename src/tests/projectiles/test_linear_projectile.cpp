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

        CHECK_VECTORS_EQ(projectile->get_direction(), godot::Vector2(0, 0));
        CHECK_EQ(projectile->get_speed(), doctest::Approx(0));

        godot::Vector2 direction = godot::Vector2(1, 1); // Vector2.RIGHT + Vector2.UP
        float speed = 100.0;
        projectile->set_direction(direction);
        projectile->set_speed(speed);

        CHECK_VECTORS_EQ(projectile->get_direction(), direction.normalized());
        CHECK_EQ(projectile->get_speed(), doctest::Approx(speed));

        memfree(projectile);
    }

    TEST_CASE("Test linear projectile movement")
    {
        LinearProjectile* projectile = memnew(LinearProjectile);

        projectile->set_direction(godot::Vector2(1, 0)); // Vector2.RIGHT
        projectile->set_speed(100.0);
        projectile->set_physics_process(true);

        double delta = ::get_current_engine_delta();
        ::simulate(projectile);
        godot::Vector2 expected_position = godot::Vector2(100, 0)*delta;
        CHECK_VECTORS_EQ(projectile->get_position(), expected_position);

        memfree(projectile);
    }
}