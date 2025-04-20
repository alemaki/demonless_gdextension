#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "skills/combo_skill_action.hpp"
#include "attacks/melee_attack.hpp"
#include "utils/utils.hpp"

struct ComboSkillActionFixture
{
    ComboSkillAction* combo = memnew(ComboSkillAction);
    godot::Node3D* dummy_source = memnew(godot::Node3D);
    MeleeAttack* stage1 = memnew(MeleeAttack);
    MeleeAttack* stage2 = nullptr;
    MeleeAttack* stage3 = nullptr;

    ComboSkillActionFixture()
    {
        ::get_scene_root()->add_child(dummy_source);

        stage1->set_duration(1);
        stage1->set_early_cancel_endpoint(0.2);
        stage1->set_late_cancel_startpoint(0.8);
        stage1->set_actor_source(dummy_source);
        stage1->set_direction({1, 0, 0});

        stage2 = godot::Object::cast_to<MeleeAttack>(stage1->duplicate());
        stage3 = godot::Object::cast_to<MeleeAttack>(stage1->duplicate());

        combo->add_child(stage1);
        combo->add_child(stage2);
        combo->add_child(stage3);

        combo->set_actor_source(dummy_source);
        combo->set_direction({1, 0, 0});
        combo->set_buffer_timespan_before_action_end(0.5);

        ::get_scene_root()->add_child(combo);
        combo->_ready();
        combo->reset();
    }

    ~ComboSkillActionFixture()
    {
        combo->remove_child(stage1);
        combo->remove_child(stage2);
        combo->remove_child(stage3);
        ::get_scene_root()->remove_child(combo);
        ::get_scene_root()->remove_child(dummy_source);

        memdelete(stage1);
        memdelete(stage2);
        memdelete(stage3);
        memdelete(combo);
        memdelete(dummy_source);
    }
};

TEST_SUITE("TestComboSkillAction")
{
    TEST_CASE_FIXTURE(ComboSkillActionFixture, "Basic")
    {
        CHECK_EQ(combo->get_actor_source(), dummy_source);

        combo->set_buffer_timespan_before_action_end(0.4);
        CHECK_EQ(combo->get_buffer_timespan_before_action_end(), doctest::Approx(0.4));
    }

    TEST_CASE("_ready sums every stage's duration plus a safety margin")
    {
        ComboSkillAction* combo = memnew(ComboSkillAction);
        godot::Node3D* dummy_source = memnew(godot::Node3D);
        MeleeAttack* stage1 = memnew(MeleeAttack);
        stage1->set_actor_source(dummy_source);
        stage1->set_duration(1);

        MeleeAttack* stage2 = godot::Object::cast_to<MeleeAttack>(stage1->duplicate());
        MeleeAttack* stage3 = godot::Object::cast_to<MeleeAttack>(stage1->duplicate());
        stage2->set_duration(2);
        stage3->set_duration(3);

        combo->add_child(stage1);
        combo->add_child(stage2);
        combo->add_child(stage3);

        ::get_scene_root()->add_child(dummy_source);
        ::get_scene_root()->add_child(combo);
        combo->_ready();

        CHECK(combo->get_duration() > doctest::Approx(6.0));

        combo->remove_child(stage1);
        combo->remove_child(stage2);
        combo->remove_child(stage3);
        ::get_scene_root()->remove_child(combo);
        ::get_scene_root()->remove_child(dummy_source);
        memdelete(stage1);
        memdelete(stage2);
        memdelete(stage3);
        memdelete(combo);
        memdelete(dummy_source);
    }

    TEST_CASE_FIXTURE(ComboSkillActionFixture, "Finishing a stage without a buffered input ends the whole combo")
    {
        combo->step(1.0);

        CHECK(stage1->is_finished());
        CHECK(combo->is_finished());
        /* stage2/stage3 were never touched */
        CHECK(stage2->is_none());
        CHECK(stage3->is_none());
    }

    TEST_CASE_FIXTURE(ComboSkillActionFixture, "Buffering the next action chains into the next stage on the cancel window")
    {
        combo->step(0.6);
        CHECK_FALSE(stage1->is_cancellable());
        CHECK(combo->try_buffer_next_action());

        /* Crosses into the cancel window and then chains into stage2 */
        combo->step(0.2);

        CHECK(stage1->is_finished());
        CHECK(stage2->is_none());
        CHECK_FALSE(combo->is_finished());

        /* should not auto-chain into stage3 */
        combo->step(1.0);

        CHECK(stage2->is_finished());
        CHECK(combo->is_finished());
        CHECK(stage3->is_none());
    }

    TEST_CASE_FIXTURE(ComboSkillActionFixture, "Buffering a cancel ends the combo immediately without starting the next stage")
    {
        combo->step(0.6);
        CHECK(combo->try_buffer_cancel());

        /* Crosses into the cancel window and then combo ends instead of chaining */
        combo->step(0.2);

        CHECK(combo->is_finished());
        CHECK(stage2->is_none());
    }

    TEST_CASE_FIXTURE(ComboSkillActionFixture, "try_buffer_next_action fails while far from the end of the stage")
    {
        combo->step(0.1);
        CHECK_FALSE(combo->try_buffer_next_action());
        CHECK_FALSE(combo->try_buffer_cancel());
    }

    TEST_CASE_FIXTURE(ComboSkillActionFixture, "reset() after a completed combo allows a second full run")
    {
        combo->step(1.0);
        REQUIRE(combo->is_finished());
        REQUIRE(stage1->is_finished());

        combo->reset();

        CHECK_FALSE(stage1->is_finished());
        CHECK(stage1->is_none());

        combo->step(0.1);

        CHECK_FALSE(combo->is_finished());
        CHECK_EQ(stage1->get_time_accumulated(), doctest::Approx(0.1));
    }

    TEST_CASE_FIXTURE(ComboSkillActionFixture, "reset() while mid-stage ends the interrupted stage cleanly")
    {
        combo->step(0.6);
        CHECK(combo->try_buffer_next_action());
        combo->step(0.2);
        REQUIRE(stage2->is_none());

        combo->step(0.3);
        CHECK(stage2->is_active());

        /* Simulate an external interrupt */
        combo->reset();

        CHECK(stage2->is_finished());
        /* current stage rewound back to stage1, freshly reset and ready to go again */
        CHECK(stage1->is_none());
    }
}

TEST_SUITE("[errors] TestComboSkillAction")
{
    TEST_CASE("_ready fails when there are no SkillAction children")
    {
        ComboSkillAction* combo = memnew(ComboSkillAction);
        CHECK_GODOT_ERROR(::get_scene_root()->add_child(combo));
        CHECK_GODOT_ERROR(combo->step(0.1));

        memdelete(combo);
    }
}
