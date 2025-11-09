#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#include "utils/utils.hpp"
#include "projectiles/projectile.hpp"

struct Fixture
{
    Projectile* projectile = nullptr;
    Hitbox* hitbox = nullptr;
    godot::Ref<MovementContext> movement_context = nullptr;
    godot::Timer* timer = nullptr;

    Fixture()
    {
        projectile = memnew(Projectile);
        hitbox = memnew(Hitbox);
        movement_context.instantiate();
        timer = memnew(godot::Timer);
        projectile->set_hitbox(hitbox);
        projectile->add_child(hitbox);
        projectile->set_movement_context(movement_context);
        projectile->set_lifespan_timer(timer);
        ::get_scene_root()->add_child(projectile);
    }

    ~Fixture()
    {
        memdelete(projectile);
    }
};

TEST_SUITE("TestProjectile")
{
    TEST_CASE_FIXTURE(Fixture, "Projectile basic")
    {
        CHECK_EQ(projectile->get_movement_context(), movement_context);
        CHECK_EQ(projectile->get_lifespan_timer(), timer);
    }

    TEST_CASE_FIXTURE(Fixture, "Projectile lifespan timer emits signals only on timeout.")
    {
        SignalObserver::watch_signals(timer);
        timer->set_wait_time(5);
        projectile->set_lifespan_timer(timer);

        projectile->get_lifespan_timer()->_process(1);
        CHECK_FALSE(projectile->is_queued_for_deletion());

        projectile->get_lifespan_timer()->_process(5);
        projectile->get_lifespan_timer()->emit_signal("timeout");

        CHECK(SignalObserver::signal_emitted(projectile->get_lifespan_timer(), godot::String("timeout")));
        CHECK(projectile->is_queued_for_deletion());
    }

    TEST_CASE_FIXTURE(Fixture, "Test projectile default movement")
    {
        REQUIRE_FALSE(projectile->get_movement_context().is_null());
        godot::Ref<MovementContext> movement_context = projectile->get_movement_context();

        movement_context->set_direction(godot::Vector3(1, 0, 0));
        movement_context->set_speed(100.0);
        projectile->set_physics_process(true);

        double delta = ::get_current_engine_delta();
        ::simulate(projectile);

        godot::Vector3 expected_position = godot::Vector3(100, 0, 0) * delta;
        CHECK_VECTORS_EQ(projectile->get_position(), expected_position);
    }
}

// TODO: needs more testing.