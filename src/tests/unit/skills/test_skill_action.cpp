#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "skills/skill_action.hpp"
#include "utils/utils.hpp"

TEST_SUITE("TestSkillAction")
{
    TEST_CASE("SkillAction basic getters/setters")
    {
        SkillAction* action = memnew(SkillAction);

        /* Default duration */
        CHECK(action->get_duration() == doctest::Approx(1.0));
        action->set_duration(3.5);
        CHECK(action->get_duration() == doctest::Approx(3.5));

        CHECK(action->is_cancellable());
        action->set_cancellable(false);
        CHECK_FALSE(action->is_cancellable());

        godot::Node3D* dummy = memnew(godot::Node3D);
        action->set_actor_source(dummy);
        CHECK(action->get_actor_source() == dummy);

        action->set_animation("1");
        CHECK_EQ(action->get_animation(), godot::StringName("1"));

        memdelete(dummy);
        memdelete(action);
    }

    TEST_CASE("Reset behavior sets phase and time")
    {
        SkillAction* action = memnew(SkillAction);
        action->set_duration(3.5);
        action->step(1.0);
        CHECK(action->get_time_accumulated() == doctest::Approx(1.0));
        CHECK(action->is_active());

        action->reset();
        CHECK(action->get_time_accumulated() == doctest::Approx(0.0));
        CHECK_FALSE(action->is_active());
        CHECK(action->get_phase() == SkillAction::SkillPhase::None);

        memdelete(action);
    }

    TEST_CASE("Step progression and phase transitions")
    {
        SkillAction* action = memnew(SkillAction);
        action->set_duration(2.0);

        action->step(1.0);
        CHECK(action->is_active());
        CHECK_FALSE(action->is_done());
        CHECK(action->get_phase() == SkillAction::SkillPhase::Active);
        CHECK(action->get_time_accumulated() == doctest::Approx(1.0));

        action->step(1.5);
        CHECK(action->is_done());
        CHECK_FALSE(action->is_active());
        CHECK(action->get_phase() == SkillAction::SkillPhase::Ended);
        CHECK(action->get_time_accumulated() == doctest::Approx(2.5));

        memdelete(action);
    }

    TEST_CASE("End method forces skill to end immediately")
    {
        SkillAction* action = memnew(SkillAction);

        action->step(0.5);
        CHECK(action->is_active());
        CHECK_FALSE(action->is_done());

        action->end();
        CHECK(action->is_done());
        CHECK_FALSE(action->is_active());
        CHECK(action->get_phase() == SkillAction::SkillPhase::Ended);

        memdelete(action);
    }

    TEST_CASE("Direction can only change while allowed or before starting")
    {
        SkillAction* action = memnew(SkillAction);

        action->set_direction({1, 0, 0});
        CHECK_VECTORS_EQ(action->get_direction(), godot::Vector3(1, 0, 0));

        action->set_change_direction_allowed(false);
        action->set_direction({0, 1, 0}); /* Should be able to change direction before start */
        CHECK_VECTORS_EQ(action->get_direction(), godot::Vector3(0, 1, 0));

        action->step(0.1);
        action->set_direction({0, 0, 1});
        CHECK_VECTORS_EQ(action->get_direction(), godot::Vector3(0, 1, 0));

        action->reset();
        action->set_direction({0, 0, 1});
        CHECK_VECTORS_EQ(action->get_direction(), godot::Vector3(0, 0, 1));

        action->set_change_direction_allowed(true);
        action->step(0.1);
        action->set_direction({0, 1, 0});
        CHECK_VECTORS_EQ(action->get_direction(), godot::Vector3(0, 1, 0));

        memdelete(action);
    }
}

TEST_SUITE("[errors] TestSkillAction")
{
    TEST_CASE("set_direction fails on zero direction")
    {
        SkillAction* action = memnew(SkillAction);
        CHECK_GODOT_ERROR(action->set_direction({0, 0, 0}));
        memdelete(action);
    }
}