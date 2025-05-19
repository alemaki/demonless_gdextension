#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "attacks/melee_attack.hpp"
#include "utils/utils.hpp"

struct MeleeAttackFixture
{
    MeleeAttack* attack = memnew(MeleeAttack);
    godot::Node3D* dummy_source = memnew(godot::Node3D);
    Hitbox* hitbox = memnew(Hitbox);
    MeleeAttackFixture()
    {
        ::get_scene_root()->add_child(dummy_source);
        attack->set_actor_source(dummy_source);
        attack->set_duration(1);
        attack->set_early_cancel_endpoint(0.3);
        attack->set_late_cancel_startpoint(0.7);
        attack->set_hit_startpoint(0.4);
        attack->set_hit_endpoint(0.6);
        attack->set_direction({1, 0, 0});
        attack->reset();
        attack->add_child(hitbox);
        hitbox->set_name("Hitbox");
        ::get_scene_root()->add_child(attack);
    }

    ~MeleeAttackFixture()
    {
        memdelete(attack);
        memdelete(dummy_source);
    }
};


TEST_SUITE("TestComboStageAttack")
{
    TEST_CASE_FIXTURE(MeleeAttackFixture, "Basic")
    {
        CHECK_EQ(attack->get_actor_source(), dummy_source);

        attack->set_early_cancel_endpoint(0.3);
        CHECK_EQ(attack->get_early_cancel_endpoint(), doctest::Approx(0.3));

        attack->set_late_cancel_startpoint(0.6);
        CHECK_EQ(attack->get_late_cancel_startpoint(), doctest::Approx(0.6));
    }

    TEST_CASE_FIXTURE(MeleeAttackFixture, "Cancel window returns proper results during Action")
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

    TEST_CASE_FIXTURE(MeleeAttackFixture, "Attack activates the hitbox in set window")
    {
        /* Hitbox shouldn't be active */
        CHECK_FALSE(hitbox->is_monitoring());

        /* 0.1 sec */
        attack->step(0.1);
        CHECK_FALSE(hitbox->is_monitoring());

        /* 0.2 sec */
        attack->step(0.1);
        CHECK_FALSE(hitbox->is_monitoring());

        /* 0.4 sec */
        attack->step(0.2);
        CHECK(hitbox->is_monitoring());

        /* 0.5 sec */
        attack->step(0.1);
        CHECK(hitbox->is_monitoring());

        /* 0.599 sec */
        attack->step(0.099);
        CHECK(hitbox->is_monitoring());

        /* 0.6499 sec (gets into end window) */
        attack->step(0.05);
        CHECK_FALSE(hitbox->is_monitoring());

        /* 0.8 sec */
        attack->step(0.151);
        CHECK_FALSE(hitbox->is_monitoring());

        /* 1 sec */
        attack->step(2);
        CHECK_FALSE(hitbox->is_monitoring());
    }
}

TEST_SUITE("[errors] TestComboStageAttack")
{
    TEST_CASE_FIXTURE(MeleeAttackFixture, "_step fails when actor_source is null")
    {
        MeleeAttack* attack = memnew(MeleeAttack);
        attack->set_actor_source(nullptr);
        CHECK_GODOT_ERROR(attack->step(0.1)); // actor_source is null;
    }

    TEST_CASE_FIXTURE(MeleeAttackFixture, "_step fails when hitbox is null")
    {
        MeleeAttack* attack = memnew(MeleeAttack);
        attack->set_hitbox(nullptr);
        CHECK_GODOT_ERROR(attack->step(0.1)); // hitbox is null
    }

    TEST_CASE_FIXTURE(MeleeAttackFixture, "set_directino fails when zero vector passed")
    {
        MeleeAttack* attack = memnew(MeleeAttack);
        CHECK_GODOT_ERROR(attack->set_direction({0, 0, 0}));
        CHECK_GODOT_ERROR(attack->step(0.1)); // hitbox is null
    }
}
