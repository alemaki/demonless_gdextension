#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"

#include "movement_strategies/composite_movement_strategy.hpp"
#include "movement_strategies/movement_strategy.hpp"
#include "movement_strategies/movement_context.hpp"

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

TEST_SUITE("CompositeMovementStrategy Tests")
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

        bool is_done() const override
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
}

TEST_SUITE("[errors] CompositeMovementStrategy Error Tests")
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