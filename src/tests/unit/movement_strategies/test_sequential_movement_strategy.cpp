#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"

#include "movement_strategies/sequential_movement_strategy.hpp"
#include "movement_strategies/movement_strategy.hpp"
#include "movement_strategies/movement_context.hpp"

using namespace godot;

struct SequentialStrategyFixture
{
    SequentialMovementStrategy* sequential = nullptr;
    MovementStrategy* strategy1 = nullptr;
    MovementStrategy* strategy2 = nullptr;

    SequentialStrategyFixture()
    {
        sequential = memnew(SequentialMovementStrategy);
        strategy1 = memnew(MovementStrategy);
        strategy2 = memnew(MovementStrategy);

        sequential->add_child(strategy1);
        sequential->add_child(strategy2);
        ::get_scene_root()->add_child(sequential);
    }

    ~SequentialStrategyFixture()
    {
        sequential->remove_child(strategy1);
        sequential->remove_child(strategy2);

        memdelete(strategy1);
        memdelete(strategy2);
        memdelete(sequential);
    }
};

TEST_SUITE("SequentialMovementStrategy Tests")
{
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

    TEST_CASE_FIXTURE(SequentialStrategyFixture, "get_movement_strategies returns all MovementStrategy children")
    {
        TypedArray<MovementStrategy> result = sequential->get_movement_strategies();

        CHECK_EQ(result.size(), 2);
        CHECK_EQ(result[0], godot::Variant(strategy1));
        CHECK_EQ(result[1], godot::Variant(strategy2));
    }

    TEST_CASE("apply only calls current child strategy")
    {
        SequentialMovementStrategy* seq = memnew(SequentialMovementStrategy);

        TestStrategy* strat1 = memnew(TestStrategy);
        TestStrategy* strat2 = memnew(TestStrategy);

        seq->add_child(strat1);
        seq->add_child(strat2);

        Ref<MovementContext> context = memnew(MovementContext);

        seq->apply(context, 0.016);

        CHECK_EQ(strat1->apply_call_count, 1);
        CHECK_EQ(strat2->apply_call_count, 0);

        memdelete(seq);
    }

    TEST_CASE("moves to next strategy when current is done")
    {
        SequentialMovementStrategy* seq = memnew(SequentialMovementStrategy);

        TestStrategy* strat1 = memnew(TestStrategy);
        TestStrategy* strat2 = memnew(TestStrategy);

        seq->add_child(strat1);
        seq->add_child(strat2);

        Ref<MovementContext> context = memnew(MovementContext);

        // Mark strat1 as done, should advance to strat2
        strat1->done = true;
        seq->apply(context, 0.016);

        CHECK_EQ(strat1->apply_call_count, 0);
        CHECK_EQ(strat2->apply_call_count, 1);

        memdelete(seq);
    }

    TEST_CASE("is_done returns false if not all strategies are complete")
    {
        SequentialMovementStrategy* seq = memnew(SequentialMovementStrategy);
        TestStrategy* strat = memnew(TestStrategy);

        seq->add_child(strat);

        CHECK_FALSE(seq->is_done());

        memdelete(seq);
    }

    TEST_CASE("is_done returns true after all strategies complete")
    {
        SequentialMovementStrategy* seq = memnew(SequentialMovementStrategy);
        TestStrategy* strat1 = memnew(TestStrategy);
        TestStrategy* strat2 = memnew(TestStrategy);

        strat1->done = true;
        strat2->done = true;

        seq->add_child(strat1);
        seq->add_child(strat2);

        CHECK(seq->is_done());

        memdelete(seq);
    }
}

TEST_SUITE("[errors] SequentialMovementStrategy Error Tests")
{
    TEST_CASE_FIXTURE(SequentialStrategyFixture, "Apply call fails if context is null")
    {
        CHECK_GODOT_ERROR(sequential->apply(nullptr, 1));
    }

    TEST_CASE("Apply _ready fails if no children are in composite strategy.")
    {
        SequentialMovementStrategy* seq = memnew(SequentialMovementStrategy);
        CHECK_GODOT_ERROR(::get_scene_root()->add_child(seq));
        memdelete(seq);
    }

    TEST_CASE("Apply call fails if current child is not a MovementStrategy")
    {
        SequentialMovementStrategy* seq = memnew(SequentialMovementStrategy);
        seq->add_child(memnew(Node3D));
        Ref<MovementContext> context = memnew(MovementContext);
        CHECK_GODOT_ERROR(seq->apply(context, 1));
        memdelete(seq);
    }
}