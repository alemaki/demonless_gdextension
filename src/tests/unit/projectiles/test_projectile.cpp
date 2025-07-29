#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"
#include "projectiles/projectile.hpp"

TEST_SUITE("TestProjectile")
{
    TEST_CASE("Projectile basic")
    {
        Projectile* projectile = memnew(Projectile);
        MovementContext* movement_context = memnew(MovementContext);
        godot::Timer* timer = memnew(godot::Timer);
        projectile->set_movement_context(movement_context);
        projectile->set_lifespan_timer(timer);

        CHECK_EQ(projectile->get_movement_context(), movement_context);
        CHECK_EQ(projectile->get_lifespan_timer(), timer);

        memdelete(projectile);
        memdelete(timer);
    }

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


    TEST_CASE("Test projectile default movement")
    {
        Projectile* projectile = memnew(Projectile);
        ::get_scene_root()->add_child(projectile);
        REQUIRE_FALSE(projectile->get_movement_context().is_null());
        godot::Ref<MovementContext> movement_context = projectile->get_movement_context();

        movement_context->set_direction(godot::Vector3(1, 0, 0));
        movement_context->set_speed(100.0);
        projectile->set_physics_process(true);

        double delta = ::get_current_engine_delta();
        ::simulate(projectile);
        godot::Vector3 expected_position = godot::Vector3(100, 0, 0)*delta;
        CHECK_VECTORS_EQ(projectile->get_position(), expected_position);

        memdelete(projectile);
    }
}