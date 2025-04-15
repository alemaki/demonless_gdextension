#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "attacks/combo_attack_stage.hpp"
#include "utils/utils.hpp"

struct ComboAttackStageFixture
{
    ComboAttackStage* attack = memnew(ComboAttackStage);
    godot::Node3D* dummy_source = memnew(godot::Node3D);

    ComboAttackStageFixture()
    {
        ::get_scene_root()->add_child(dummy_source);
        attack->set_actor_source(dummy_source);
        attack->set_duration(1);
        attack->set_until_cancel_window(0.3);
        attack->set_after_cancel_window(0.7);
        attack->set_direction({1, 0, 0});
        attack->reset();
        ::get_scene_root()->add_child(attack);
    }

    ~ComboAttackStageFixture()
    {
        memdelete(attack);
        memdelete(dummy_source);
    }
};


TEST_SUITE("TestComboStageAttack")
{
    TEST_CASE_FIXTURE(ComboAttackStageFixture, "Basic")
    {
        attack->set_direction({1, 0, 0});
        CHECK_VECTORS_EQ(attack->get_direction(), godot::Vector3(1, 0, 0));
        CHECK_EQ(attack->get_actor_source(), dummy_source);
        
        attack->set_until_cancel_window(0.3);
        CHECK_EQ(attack->get_until_cancel_window(), doctest::Approx(0.3));

        attack->set_after_cancel_window(0.6);
        CHECK_EQ(attack->get_after_cancel_window(), doctest::Approx(0.6));
        
        godot::Vector3 dir = {1, 0, 1};
        attack->set_direction(dir);
        godot::Vector3 got = attack->get_direction();
        dir.normalize();
        CHECK_VECTORS_EQ(got, dir);
    }

    TEST_CASE_FIXTURE(ComboAttackStageFixture, "Cancel window returns proper results during Action")
    {
        /* Shouldn't be cancelable if not yet started */
        CHECK_FALSE(attack->is_cancellable());

        /* 0.1 sec */
        attack->step(0.1);
        CHECK(attack->is_cancellable());

        /* 0.2 sec */
        attack->step(0.1);
        CHECK(attack->is_cancellable());

        /* 0.4 sec (got out of window in start) */
        attack->step(0.2);
        CHECK_FALSE(attack->is_cancellable());
        
        /* 0.5 sec */
        attack->step(0.1);
        CHECK_FALSE(attack->is_cancellable());

        /* 0.6 sec */
        attack->step(0.1);
        CHECK_FALSE(attack->is_cancellable());
    
        /* 0.75 sec (gets into end window) */
        attack->step(0.15);
        CHECK(attack->is_cancellable());

        /* 0.9 sec */
        attack->step(0.15);
        CHECK(attack->is_cancellable());

        /* 1 sec (ends, shouldn't be cancelable if ended) */
        attack->step(2);
        CHECK_FALSE(attack->is_cancellable());
    }

    TEST_CASE_FIXTURE(ComboAttackStageFixture, "Change direction")
    {
        attack->set_direction({1, 0, 0});
        CHECK_VECTORS_EQ(attack->get_direction(), godot::Vector3(1, 0, 0));
        
        attack->set_change_direction_allowed(false);
        attack->set_direction({0, 1, 0}); /* Should be able to change direction before start */
        CHECK_VECTORS_EQ(attack->get_direction(), godot::Vector3(0, 1, 0));

        attack->step(0.1);
        attack->set_direction({0, 0, 1});
        CHECK_VECTORS_EQ(attack->get_direction(), godot::Vector3(0, 1, 0));

        attack->reset();
        attack->set_direction({0, 0, 1});
        CHECK_VECTORS_EQ(attack->get_direction(), godot::Vector3(0, 0, 1));

        attack->set_change_direction_allowed(true);
        attack->step(0.1);
        attack->set_direction({0, 1, 0});
        CHECK_VECTORS_EQ(attack->get_direction(), godot::Vector3(0, 1, 0));
    }
}

TEST_SUITE("[errors] TestComboStageAttack")
{
    TEST_CASE("_step fails on nulls and zero direction")
    {
        ComboAttackStage* attack = memnew(ComboAttackStage);
        CHECK_GODOT_ERROR(attack->step(0.1)); // actor_source is null

        godot::Node3D* dummy_source = memnew(godot::Node3D);
        attack->set_actor_source(dummy_source);
        attack->step(0.1);

        CHECK_GODOT_ERROR(attack->set_direction({0, 0, 0})); // Zero direction

        memdelete(dummy_source);
        memdelete(attack);
    }
}