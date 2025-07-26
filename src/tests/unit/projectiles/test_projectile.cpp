#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"
#include "projectiles/projectile.hpp"

TEST_SUITE("Lifespan tests.")
{
    TEST_CASE("Projectile lifespan timer emmits signals only on timeout.")
    {
        Projectile* projectile = memnew(Projectile);
        godot::Timer* timer = memnew(godot::Timer);
        SignalWatcher::watch_signals(timer);
        timer->set_wait_time(5);
        projectile->set_lifespan_timer(timer);
        ::get_scene_root()->add_child(projectile);
        projectile->get_lifespan_timer()->_process(1);
        CHECK_FALSE(projectile->is_queued_for_deletion());
        projectile->get_lifespan_timer()->_process(5);
        projectile->get_lifespan_timer()->emit_signal("timeout");
        CHECK(SignalWatcher::signal_emitted(projectile->get_lifespan_timer(), godot::String("timeout")));
        CHECK(projectile->is_queued_for_deletion());
    }
};


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