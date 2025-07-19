#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "projectiles/projectile.hpp"
#include "utils/utils.hpp"


TEST_SUITE("TestLinearProjectile")
{

    TEST_CASE("Projectile basic")
    {
        Projectile* projectile = memnew(Projectile);

        CHECK_VECTORS_EQ(projectile->get_direction(), godot::Vector3(1, 0, 0));

        godot::Vector3 direction = godot::Vector3(1, 0, 1); // Vector3.RIGHT + Vector3.FORWARD
        projectile->set_direction(direction);

        CHECK_VECTORS_EQ(projectile->get_direction(), direction.normalized());

        memdelete(projectile);
    }
}