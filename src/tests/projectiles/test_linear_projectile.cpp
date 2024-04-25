#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/godot.hpp>
#include "projectiles/linear_projectile.hpp"

using namespace godot;

bool vectors_almost_equal(Vector2 vec1, Vector2 vec2, Vector2 tolerance)
{
    return Math::abs(vec1.x - vec2.x) <= tolerance.x &&
           Math::abs(vec1.y - vec2.y) <= tolerance.y;
}

bool floats_almost_equal(float value1, float value2, float tolerance)
{
    return Math::abs(value1 - value2) <= tolerance;
}

void simulate_physics_process(LinearProjectile* projectile, float delta)
{
    projectile->_physics_process(delta);
}

TEST_SUITE("TestLinearProjectile")
{

    TEST_CASE("Test linear projectile basic")
    {
        LinearProjectile* projectile = memnew(LinearProjectile);

        CHECK(vectors_almost_equal(projectile->get_direction(), Vector2(0, 0), Vector2(0.001, 0.001)));
        CHECK(floats_almost_equal(projectile->get_speed(), 0.0, 0.001));

        Vector2 direction = Vector2(1, 1); // Vector2.RIGHT + Vector2.UP
        float speed = 100.0;
        projectile->set_direction(direction);
        projectile->set_speed(speed);

        CHECK(vectors_almost_equal(projectile->get_direction(), direction.normalized(), Vector2(0.001, 0.001)));
        CHECK(floats_almost_equal(projectile->get_speed(), speed, 0.001));

        memfree(projectile);
    }

    TEST_CASE("Test linear projectile movement")
    {
        LinearProjectile* projectile = memnew(LinearProjectile);

        projectile->set_direction(Vector2(1, 0)); // Vector2.RIGHT
        projectile->set_speed(100.0);
        projectile->set_physics_process(true);

        simulate_physics_process(projectile, 0.1);
        Vector2 expected_position = Vector2(10, 0);
        CHECK(vectors_almost_equal(projectile->get_position(), expected_position, Vector2(0.001, 0.001)));

        memfree(projectile);
    }
}