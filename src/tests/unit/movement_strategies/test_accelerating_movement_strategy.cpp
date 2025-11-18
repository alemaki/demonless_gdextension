
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
        strat->set_duration(0);
        ctx->set_speed(0.0);
        ctx->set_position(Vector3(0, 0, 0));
        ctx->set_direction(Vector3(1, 0, 0));
    }

    ~AcceleratingFixture()
    {
        memdelete(strat);
    }
};

TEST_SUITE("AcceleratingMovementStrategy Full Acceleration Tests")
{
    TEST_CASE_FIXTURE(AcceleratingFixture, "Positive acceleration until max_speed")
    {
        strat->set_acceleration_per_second(2.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(5.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(2.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(4.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(5.0)); // hits max speed

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(5.0)); // stays at max

        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Negative acceleration until min_speed")
    {
        ctx->set_speed(6.0);

        strat->set_acceleration_per_second(-3.0);
        strat->set_min_speed(2.0);
        strat->set_max_speed(10.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(3.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(2.0)); // hits min speed

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(2.0)); // stays at min

        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Zero acceleration keeps speed constant")
    {
        ctx->set_speed(3.0);
        strat->set_acceleration_per_second(0.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(5.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(3.0));

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(3.0));

        CHECK_FALSE(strat->is_done()); // done only when hitting limit
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Acceleration hitting max_speed mid-frame")
    {
        ctx->set_speed(4.0);
        strat->set_acceleration_per_second(3.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(5.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(5.0)); // clamps correctly
        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Acceleration hitting min_speed mid-frame")
    {
        ctx->set_speed(3.0);
        strat->set_acceleration_per_second(-4.0);
        strat->set_min_speed(2.0);
        strat->set_max_speed(10.0);

        strat->apply(ctx, 1.0);
        CHECK_EQ(ctx->get_speed(), doctest::Approx(2.0)); // clamps correctly
        CHECK(strat->is_done());
    }

    TEST_CASE_FIXTURE(AcceleratingFixture, "Position updates correctly")
    {
        ctx->set_direction(Vector3(0, 0, 1));
        strat->set_acceleration_per_second(2.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(10.0);

        strat->apply(ctx, 0.5);
        /* displacement = s*t + 0.5*a*t^2 = 0*0.5 + 0.5*2*0.25 = 0.25 */
        CHECK_VECTORS_EQ(ctx->get_position(), Vector3(0, 0, 0.25));

        strat->apply(ctx, 1.0);
        /* speed at start 1.0? displacement = s*t + 0.5*a*t^2 = 1*1 + 0.5*2*1 = 2 */
        CHECK_VECTORS_EQ(ctx->get_position(), Vector3(0, 0, 2.25));
    }
    TEST_CASE_FIXTURE(AcceleratingFixture, "Displacement correct when hitting max_speed mid-frame")
    {
        // Start close to max_speed so it hits mid-frame
        ctx->set_speed(4.0);
        strat->set_acceleration_per_second(3.0);
        strat->set_min_speed(0.0);
        strat->set_max_speed(5.0);

        double delta = 1.0;
        strat->apply(ctx, delta);

        double time_accelerating = (5.0 - 4.0) / 3.0;
        double time_at_max = delta - time_accelerating;
        double displacement = 4.0 * time_accelerating + 0.5 * 3.0 * time_accelerating * time_accelerating; // during acceleration
        displacement += 5.0 * time_at_max;

        CHECK_VECTORS_EQ(ctx->get_position(), Vector3(displacement, 0, 0));

        CHECK_EQ(ctx->get_speed(), doctest::Approx(5.0));

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