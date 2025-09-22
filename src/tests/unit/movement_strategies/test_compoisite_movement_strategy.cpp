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

        ::get_scene_root()->add_child(composite);
        composite->add_child(strategy1);
        composite->add_child(strategy2);
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
    TEST_CASE_FIXTURE(CompositeStrategyFixture, "get_movement_strategies returns all MovementStrategy children")
    {
        TypedArray<MovementStrategy> result = composite->get_movement_strategies();

        CHECK_EQ(result.size(), 2);
        CHECK_EQ(result[0], godot::Variant(strategy1));
        CHECK_EQ(result[1], godot::Variant(strategy2));
    }

    TEST_CASE_FIXTURE(CompositeStrategyFixture, "apply calls apply on all MovementStrategy children")
    {
        /* Note to self: CAN DO THIS if GDCLASS is not defined in child. */
        struct TestStrategy : public MovementStrategy
        {
            int apply_call_count = 0;

            void _apply(Ref<MovementContext>, double) override
            {
                apply_call_count++;
            }
        };

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
}