#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"

#include "movement_strategies/composite_movement_strategy.hpp"
#include "movement_strategies/movement_strategy.hpp"
#include "movement_strategies/movement_context.hpp"
#include "movement_strategies/accelerating_movement_strategy.hpp"

using namespace godot;

struct CompositeStrategyFixture
{
    CompositeMovementStrategy* composite = nullptr;
    MovementStrategy* strategy1 = nullptr;
    MovementStrategy* strategy2 = nullptr;

    CompositeStrategyFixture()
    {
        composite = memnew(CompositeMovementStrategy);
        strategy1 = memnew(MovementStrategy);
        strategy2 = memnew(MovementStrategy);

        composite->add_child(strategy1);
        composite->add_child(strategy2);
        ::get_scene_root()->add_child(composite);
    }

    ~CompositeStrategyFixture()
    {
        composite->remove_child(strategy1);
        composite->remove_child(strategy2);

        memdelete(strategy1);
        memdelete(strategy2);
        memdelete(composite);
    }
};

TEST_SUITE("CompositeMovementStrategy")
{
    /* Note to self: CAN DO THIS if GDCLASS is not defined in child class. */
    struct TestStrategy : public MovementStrategy
    {
        int apply_call_count = 0;
        bool done = false;

        void _apply(Ref<MovementContext>, double) override
        {
            apply_call_count++;
        }

        bool _is_done() const override
        {
            return this->done;
        }
    };

    TEST_CASE_FIXTURE(CompositeStrategyFixture, "get_movement_strategies returns all MovementStrategy children")
    {
        TypedArray<MovementStrategy> result = composite->get_movement_strategies();

        CHECK_EQ(result.size(), 2);
        CHECK_EQ(result[0], godot::Variant(strategy1));
        CHECK_EQ(result[1], godot::Variant(strategy2));
    }

    TEST_CASE_FIXTURE(CompositeStrategyFixture, "apply calls apply on all MovementStrategy children")
    {
        TestStrategy* test_strategy1 = memnew(TestStrategy);
        TestStrategy* test_strategy2 = memnew(TestStrategy);

        composite->add_child(test_strategy1);
        composite->add_child(test_strategy2);

        Ref<MovementContext> dummy_context = memnew(MovementContext);
        composite->apply(dummy_context, 0.016);

        CHECK_EQ(test_strategy1->apply_call_count, 1);
        CHECK_EQ(test_strategy2->apply_call_count, 1);

        composite->remove_child(test_strategy1);
        composite->remove_child(test_strategy2);
        memdelete(test_strategy1);
        memdelete(test_strategy2);
    }

    TEST_CASE_FIXTURE(CompositeStrategyFixture, "is_done returns false if any of the children are not done")
    {
        /* strategy1-2 from the fixtures return true on is_done call. */
        TestStrategy* always_done_strategy = memnew(TestStrategy);
        always_done_strategy->done = true;

        composite->add_child(always_done_strategy);

        REQUIRE_FALSE(composite->is_done());

        Ref<MovementContext> dummy_context = memnew(MovementContext);
        composite->apply(dummy_context, 0.016);

        CHECK_EQ(always_done_strategy->apply_call_count, 0);
        composite->remove_child(always_done_strategy);
        memdelete(always_done_strategy);
    }

    TEST_CASE("is_done returns true if all of the children are done")
    {
        TestStrategy* always_done_strategy1 = memnew(TestStrategy);
        TestStrategy* always_done_strategy2 = memnew(TestStrategy);
        always_done_strategy1->done = true;
        always_done_strategy2->done = true;

        CompositeMovementStrategy* composite = memnew(CompositeMovementStrategy);

        composite->add_child(always_done_strategy1);
        composite->add_child(always_done_strategy2);

        REQUIRE(composite->is_done());

        Ref<MovementContext> dummy_context = memnew(MovementContext);
        composite->apply(dummy_context, 0.016);

        CHECK_EQ(always_done_strategy1->apply_call_count, 0);
        CHECK_EQ(always_done_strategy2->apply_call_count, 0);
        memdelete(composite);
    }

    TEST_CASE_FIXTURE(CompositeStrategyFixture, "apply blends children's displacement instead of chaining position mutation")
    {
        /* strategy1 and strategy2 are both plain MovementStrategy, i.e. default linear motion:
         * each alone would move direction*speed*delta. Composed, they must not stack sequentially
         * (which would double-move the bullet) - they must sum to a single blended displacement. */
        Ref<MovementContext> ctx = memnew(MovementContext);
        ctx->set_position(Vector3(0, 0, 0));
        ctx->set_direction(Vector3(1, 0, 0));
        ctx->set_speed(2.0);

        composite->apply(ctx, 1.0);

        CHECK_EQ(ctx->get_position(), Vector3(4, 0, 0));
    }

    TEST_CASE_FIXTURE(CompositeStrategyFixture, "blend_weight scales a child's contribution to the sum")
    {
        Ref<MovementContext> ctx = memnew(MovementContext);
        ctx->set_position(Vector3(0, 0, 0));
        ctx->set_direction(Vector3(1, 0, 0));
        ctx->set_speed(2.0);

        strategy2->set_blend_weight(0.5);
        composite->apply(ctx, 1.0);

        /* strategy1 contributes (2,0,0) at weight 1.0, strategy2 contributes (2,0,0) at weight 0.5. */
        CHECK_EQ(ctx->get_position(), Vector3(3, 0, 0));
    }

    TEST_CASE("Composed children keep independent state instead of overwriting each other's ramp")
    {
        AcceleratingMovementStrategy* fast = memnew(AcceleratingMovementStrategy);
        fast->set_acceleration_per_second(2.0);
        fast->set_min_speed(0.0);
        fast->set_max_speed(100.0);

        AcceleratingMovementStrategy* slow = memnew(AcceleratingMovementStrategy);
        slow->set_acceleration_per_second(0.5);
        slow->set_min_speed(0.0);
        slow->set_max_speed(100.0);

        CompositeMovementStrategy* composite = memnew(CompositeMovementStrategy);
        composite->add_child(fast);
        composite->add_child(slow);
        ::get_scene_root()->add_child(composite);

        Ref<MovementContext> ctx = memnew(MovementContext);
        ctx->set_position(Vector3(0, 0, 0));
        ctx->set_direction(Vector3(1, 0, 0));
        ctx->set_speed(0.0);

        composite->apply(ctx, 1.0);
        /* fast ramps 0 -> 2.0 this tick (displacement 2.0), slow ramps 0 -> 0.5 (displacement 0.5). */
        CHECK_EQ(ctx->get_position(), Vector3(2.5, 0, 0));

        composite->apply(ctx, 1.0);
        /* fast continues its OWN ramp 2.0 -> 4.0 (displacement 4.0), slow continues 0.5 -> 1.0
         * (displacement 1.0) - neither ramp was reset or clobbered by the other. */
        CHECK_EQ(ctx->get_position(), Vector3(7.5, 0, 0));

        composite->remove_child(fast);
        composite->remove_child(slow);
        memdelete(fast);
        memdelete(slow);
        memdelete(composite);
    }
}

TEST_SUITE("[errors] CompositeMovementStrategy")
{
    TEST_CASE_FIXTURE(CompositeStrategyFixture, "Apply call fails if context is null")
    {
        CHECK_GODOT_ERROR(composite->apply(nullptr, 1));
    }

    TEST_CASE("Apply _ready fails if no children are in composite strategy.")
    {
        CompositeMovementStrategy* composite = memnew(CompositeMovementStrategy);
        CHECK_GODOT_ERROR(::get_scene_root()->add_child(composite));
        memdelete(composite);
    }

    TEST_CASE("One of children is not a MovementStrategy")
    {
        CompositeMovementStrategy* composite = memnew(CompositeMovementStrategy);
        composite->add_child(memnew(MovementStrategy));
        composite->add_child(memnew(Node3D));
        Ref<MovementContext> dummy_context = memnew(MovementContext);
        CHECK_GODOT_ERROR(composite->apply(dummy_context, 1));
        memdelete(composite);
    }
}