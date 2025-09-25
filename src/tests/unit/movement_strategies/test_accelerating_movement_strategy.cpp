#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"

#include "movement_strategies/accelerating_movement_strategy.hpp"
#include "movement_strategies/movement_context.hpp"

using namespace godot;

struct AcceleratingFixture
{
    AcceleratingMovementStrategy* strat = memnew(AcceleratingMovementStrategy);
    Ref<MovementContext> ctx = memnew(MovementContext);

    AcceleratingFixture()
    {
        ctx->set_speed(0.0);
        ctx->set_position(Vector3(0, 0, 0));
        ctx->set_direction(Vector3(1, 0, 0));
    }

    ~AcceleratingFixture()
    {
        memdelete(strat);
    }
};

TEST_SUITE("AcceleratingMovementStrategy Tests")
{
    TEST_CASE_FIXTURE(AcceleratingFixture, "Positive acceleration increases speed until max_speed")
    {
        strat->set_acceleration_per_second(1.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(3.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(1.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(2.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(3.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(3.0));

        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Negative acceleration decreases speed until min_speed")
    {
        ctx->set_speed(5.0);

        strat->set_acceleration_per_second(-2.0);
        strat->set_min_speed(1.0);
        strat->set_max_speed(10.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(3.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(1.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(1.0));

        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Position updates correctly with delta and direction")
    {
        ctx->set_direction(Vector3(0, 0, 1)); // move in +Z

        strat->set_acceleration_per_second(2.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(10.0);

        strat->apply(ctx, 0.5);
        CHECK_EQ(ctx->get_position(), Vector3(0, 0, 1));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_position(), Vector3(0, 0, 5));
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "is_done false while under max_speed (positive acceleration)")
    {
        strat->set_acceleration_per_second(1.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(5.0);

        strat->apply(ctx, 1.0);
        CHECK_FALSE(strat->is_done());
        strat->apply(ctx, 1.0);
        CHECK_FALSE(strat->is_done());
        strat->apply(ctx, 1.0);
        CHECK_FALSE(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "is_done true when speed reaches max_speed (positive)")
    {
        ctx->set_speed(4.0);

        strat->set_acceleration_per_second(2.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(5.0);

        strat->apply(ctx, 1.0);
        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "is_done true when speed reaches min_speed (negative)")
    {
        ctx->set_speed(2.0);

        strat->set_acceleration_per_second(-5.0);
        strat->set_min_speed(2.0);
        strat->set_max_speed(10.0);

        strat->apply(ctx, 1.0);
        CHECK(strat->is_done());
    }
}

TEST_SUITE("[errors] AcceleratingMovementStrategy Error Tests")
{
    TEST_CASE("Apply fails if context is null")
    {
        AcceleratingMovementStrategy* strat = memnew(AcceleratingMovementStrategy);
        CHECK_GODOT_ERROR(strat->apply(nullptr, 1));
        memdelete(strat);
    }

    TEST_CASE("Ready fails if min_speed is bigger than max_speed")
    {
        AcceleratingMovementStrategy* strat = memnew(AcceleratingMovementStrategy);
        strat->set_min_speed(1.0);
        strat->set_max_speed(0.0);
        CHECK_GODOT_ERROR(::get_scene_root()->add_child(strat));
        memdelete(strat);
    }

    TEST_CASE("Ready fails if last speed is approx max_speed at start")
    {
        AcceleratingMovementStrategy* strat = memnew(AcceleratingMovementStrategy);
        strat->set_acceleration_per_second(0.0);
        CHECK_GODOT_ERROR(::get_scene_root()->add_child(strat));
        memdelete(strat);
    }
}