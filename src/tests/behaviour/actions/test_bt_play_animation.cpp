#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include <godot_cpp/classes/animation_library.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "tests/test_utils/test_runner.hpp"

#include "actor_behaviour/actions/bt_play_animation.hpp"

struct BTPlayAnimationFixture
{
    godot::AnimationPlayer* animation_player;
    godot::Ref<BTPlayAnimation> task;
    godot::Node3D* actor;
    godot::Ref<Blackboard> blackboard;

    BTPlayAnimationFixture() : task(memnew(BTPlayAnimation)),
                               actor(memnew(godot::Node3D)),
                               blackboard(memnew(Blackboard))

    {
        task->set_complain_enabled(false);
        task->set_animation_player_name("AnimationPlayer");
        task->set_play_animation("first");

        task->initialize(actor, blackboard);
        blackboard->set_var("AnimationPlayer", animation_player);

        animation_player->add_animation_library("first", memnew(godot::AnimationLibrary()));
        animation_player->add_animation_library("second", memnew(godot::AnimationLibrary()));
    }
    ~BTPlayAnimationFixture()
    {
        memdelete(actor);
        memdelete(animation_player);
    }
};

TEST_SUITE("BTNode3DInRange")
{
    TEST_CASE_FIXTURE(BTPlayAnimationFixture, "Set and get basic")
    {
        CHECK_EQ(task->get_animation_player_name(), "AnimationPlayer.");
        CHECK_EQ(task->get_play_animation(), "first");
    }

    TEST_CASE_FIXTURE(BTPlayAnimationFixture, "Succeeds when playing right animation.")
    {
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(animation_player->get_current_animation(), "first");
    }

    TEST_CASE_FIXTURE(BTPlayAnimationFixture, "Fails when playing no animation.")
    {
        task->set_play_animation("Non-existent");
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::FAILURE);
    }

    TEST_CASE_FIXTURE(BTPlayAnimationFixture, "Can play different animation after first one.")
    {
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(animation_player->get_current_animation(), "first");

        task->set_play_animation("second");
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(animation_player->get_current_animation(), "second");
    }
}