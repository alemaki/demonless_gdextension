#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include "tests/test_utils/test_utils.hpp"
#include "actor_behaviour/actions/bt_look_at_position.hpp"


TEST_SUITE("BTLookAtPosition")
{
    TEST_CASE("Set and get look_at_position_var and up_vector")
    {
        godot::Ref<BTLookAtPosition> task = memnew(BTLookAtPosition);
        task->set_look_at_position_var("target_pos");
        task->set_up_vector(godot::Vector3(0, 1, 0));

        CHECK_EQ(task->get_look_at_position_var(), godot::StringName("target_pos"));
        CHECK_EQ(task->get_up_vector(), godot::Vector3(0, 1, 0));
    }

    TEST_CASE("Actor successfully looks at position")
    {
        godot::Ref<BTLookAtPosition> task = memnew(BTLookAtPosition);
        task->set_look_at_position_var("target_pos");
        task->set_up_vector(godot::Vector3(0, 1, 0));

        godot::Node3D* actor = memnew(godot::Node3D);
        godot::Vector3 initial_position(0, 0, 0);
        ::get_scene_root()->add_child(actor);

        actor->set_global_position(initial_position);

        godot::Ref<Blackboard> blackboard = memnew(Blackboard);
        blackboard->set_var("target_pos", godot::Vector3(1, 0, 1));

        task->initialize(actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);

        /* actor->get_transform().basis.get_column(2) returns negative vector. */
        CHECK_VECTORS_EQ(-actor->get_transform().basis.get_column(2), (godot::Vector3(blackboard->get_var("target_pos")) - actor->get_global_position()).normalized());

        memdelete(actor);
    }

    TEST_CASE("Fails if actor is not a Node3D")
    {
        godot::Ref<BTLookAtPosition> task = memnew(BTLookAtPosition);
        task->set_look_at_position_var("target_pos");
        task->set_complain_enabled(false);

        godot::Node* actor = memnew(godot::Node);

        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        task->initialize(actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);

        memdelete(actor);
    }

    TEST_CASE("Fails if target position is not found in blackboard")
    {
        godot::Ref<BTLookAtPosition> task = memnew(BTLookAtPosition);
        task->set_look_at_position_var("target_pos");
        task->set_complain_enabled(false);

        godot::Node3D* actor = memnew(godot::Node3D);
        ::get_scene_root()->add_child(actor);

        actor->set_global_position(godot::Vector3(0, 0, 0));


        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        task->initialize(actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);

        memdelete(actor);
    }
}