#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#include "utils/utils.hpp"
#include "projectiles/laser.hpp"

struct Fixture
{
    Laser* laser = nullptr;
    Hitbox* hitbox = nullptr;
    godot::RayCast3D* ray_cast = nullptr;
    godot::Ref<MovementContext> movement_context = nullptr;
    godot::Timer* timer = nullptr;

    Fixture()
    {
        laser = memnew(Laser);
        hitbox = memnew(Hitbox);
        ray_cast = memnew(godot::RayCast3D);
        movement_context.instantiate();
        timer = memnew(godot::Timer);
        laser->set_hitbox(hitbox);
        laser->add_child(hitbox);
        laser->set_ray_cast(ray_cast);
        laser->add_child(ray_cast);
        laser->set_movement_context(movement_context);
        laser->set_lifespan_timer(timer);
        ::get_scene_root()->add_child(laser);
    }

    ~Fixture()
    {
        memdelete(laser);
    }
};

TEST_SUITE("TestLaser")
{
    TEST_CASE("Laser constructor does not add a child before ray_cast is resolved")
    {
        Laser* laser = memnew(Laser);
        CHECK_EQ(laser->get_child_count(), 0);
        memdelete(laser);
    }

    TEST_CASE_FIXTURE(Fixture, "Laser basic")
    {
        CHECK_EQ(laser->get_movement_context(), movement_context);
        CHECK_EQ(laser->get_lifespan_timer(), timer);
        CHECK_EQ(laser->get_hitbox(), hitbox);
        CHECK_EQ(laser->get_ray_cast(), ray_cast);
    }

    TEST_CASE_FIXTURE(Fixture, "Laser lifespan timer emits signals only on timeout.")
    {
        SignalObserver::watch_signals(timer);
        timer->set_wait_time(5);
        laser->set_lifespan_timer(timer);

        laser->get_lifespan_timer()->_process(1);
        CHECK_FALSE(laser->is_queued_for_deletion());

        laser->get_lifespan_timer()->_process(5);
        laser->get_lifespan_timer()->emit_signal("timeout");

        CHECK(SignalObserver::signal_emitted(laser->get_lifespan_timer(), godot::String("timeout")));
        CHECK(laser->is_queued_for_deletion());
    }

    TEST_CASE_FIXTURE(Fixture, "Test laser default movement")
    {
        REQUIRE_FALSE(laser->get_movement_context().is_null());
        godot::Ref<MovementContext> movement_context = laser->get_movement_context();

        movement_context->set_direction(godot::Vector3(1, 0, 0));
        movement_context->set_speed(100.0);
        laser->set_physics_process(true);

        double delta = ::get_current_engine_delta();
        ::simulate(laser);

        godot::Vector3 expected_position = godot::Vector3(100, 0, 0) * delta;
        CHECK_VECTORS_EQ(laser->get_position(), expected_position);
    }

    TEST_CASE_FIXTURE(Fixture, "actor_source set before entering the tree still reaches the hitbox's actor_source")
    {
        godot::Node* source = memnew(godot::Node);

        /* laser is already inside the tree from the fixture, so re-run the scenario
           on a fresh laser that hasn't had _ready() called yet. */
        Laser* fresh_laser = memnew(Laser);
        Hitbox* fresh_hitbox = memnew(Hitbox);
        godot::RayCast3D* fresh_ray_cast = memnew(godot::RayCast3D);
        fresh_laser->set_hitbox(fresh_hitbox);
        fresh_laser->add_child(fresh_hitbox);
        fresh_laser->set_ray_cast(fresh_ray_cast);
        fresh_laser->add_child(fresh_ray_cast);

        fresh_laser->set_actor_source(source);
        CHECK_EQ(fresh_hitbox->get_actor_source(), source);

        ::get_scene_root()->add_child(fresh_laser);
        CHECK_EQ(fresh_hitbox->get_actor_source(), source);

        memdelete(fresh_laser);
        memdelete(source);
    }

    TEST_CASE_FIXTURE(Fixture, "actor_source set after entering the tree updates the hitbox's actor_source immediately")
    {
        godot::Node* source = memnew(godot::Node);

        laser->set_actor_source(source);
        CHECK_EQ(hitbox->get_actor_source(), source);
        CHECK_EQ(laser->get_actor_source(), source);

        memdelete(source);
    }

    TEST_CASE_FIXTURE(Fixture, "Laser does not queue_free when its hitbox hits a hurtbox")
    {
        Hurtbox* target_hurtbox = memnew(Hurtbox);
        ::get_scene_root()->add_child(target_hurtbox);

        hitbox->emit_signal("area_entered", target_hurtbox);

        CHECK_FALSE(laser->is_queued_for_deletion());

        memdelete(target_hurtbox);
    }

    TEST_CASE_FIXTURE(Fixture, "A hurtbox overlapping the beam receives hurtbox_hit exactly once across multiple physics steps")
    {
        Hurtbox* target_hurtbox = memnew(Hurtbox);
        ::get_scene_root()->add_child(target_hurtbox);

        SignalObserver::watch_signals(target_hurtbox);

        /* Simulates the engine's one-shot area_entered transition - only fired once,
           even though the hurtbox keeps "overlapping" across the following steps. */
        hitbox->emit_signal("area_entered", target_hurtbox);

        movement_context->set_direction(godot::Vector3(1, 0, 0));
        laser->set_physics_process(true);
        ::simulate(laser);
        ::simulate(laser);
        ::simulate(laser);

        CHECK_EQ(SignalObserver::get_signal_emitted_count(target_hurtbox, "hurtbox_hit"), 1);

        memdelete(target_hurtbox);
    }
}

// TODO: needs more testing.
