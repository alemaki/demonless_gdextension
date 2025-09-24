#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"
#include "movement_strategies/movement_context.hpp"

struct MovementContextFixture {
    MovementContext* context = nullptr;
    godot::Node3D* dummy_target = nullptr;

    MovementContextFixture() {
        context = memnew(MovementContext);
        dummy_target = memnew(godot::Node3D);
    }

    ~MovementContextFixture() {
        memdelete(context);
        memdelete(dummy_target);
    }
};

TEST_SUITE("TestMovementContext")
{
    TEST_CASE_FIXTURE(MovementContextFixture, "Default values")
    {
        CHECK_VECTORS_NE(context->get_direction(), godot::Vector3(0, 0, 0));
        CHECK_GE(context->get_speed(), doctest::Approx(0.0));
        CHECK_EQ(context->get_target(), static_cast<godot::Node3D*>(nullptr));
    }

    TEST_CASE_FIXTURE(MovementContextFixture, "Setters and getters function properly")
    {
        context->set_position(godot::Vector3(2, 4, 6));
        context->set_direction(godot::Vector3(-1, 0, 0));
        context->set_speed(2.718);
        context->set_target(dummy_target);

        CHECK_EQ(context->get_position(), godot::Vector3(2, 4, 6));
        CHECK_EQ(context->get_direction(), godot::Vector3(-1, 0, 0));
        CHECK_EQ(context->get_speed(), doctest::Approx(2.718));
        CHECK_EQ(context->get_target(), dummy_target);
    }

    TEST_CASE_FIXTURE(MovementContextFixture, "Speed does not accept negative values")
    {
        context->set_speed(-10.0);
        CHECK_GE(context->get_speed(), doctest::Approx(0.0));
    }

    TEST_CASE_FIXTURE(MovementContextFixture, "Direction is always normalised")
    {
        CHECK(context->get_direction().is_normalized());
        context->set_direction(godot::Vector3(0, 1, 1));

        CHECK_VECTORS_EQ(context->get_direction(), godot::Vector3(0, 1, 1).normalized());
    }
}