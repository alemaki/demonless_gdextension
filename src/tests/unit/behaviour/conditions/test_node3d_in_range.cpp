#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "tests/test_utils/test_macros.hpp"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "actor_behaviour/conditions/bt_node3d_in_range.hpp"

struct BTNode3DInRangeFixture
{
    godot::Ref<BTNode3DInRange> task;
    godot::Node3D* actor;
    godot::Node3D* target_node;
    godot::Node* scene_root;
    godot::Ref<Blackboard> blackboard;

    BTNode3DInRangeFixture() : task(memnew(BTNode3DInRange)),
                               actor(memnew(godot::Node3D)),
                               target_node(memnew(godot::Node3D)),
                               scene_root(get_scene_root()),
                               blackboard(memnew(Blackboard))

    {
        task->set_range(100.0);
        task->set_node_name("TargetNode");

        actor->set_position(godot::Vector3(0, 0, 0));

        scene_root->add_child(actor);
        scene_root->add_child(target_node);

        task->initialize(actor, blackboard);
        blackboard->set_var("TargetNode", target_node);
    }
    ~BTNode3DInRangeFixture()
    {
        memdelete(actor);
        memdelete(target_node);
    }
};

TEST_SUITE("BTNode3DInRange")
{
    TEST_CASE_FIXTURE(BTNode3DInRangeFixture, "Set and get range and node name expected behavior")
    {
        CHECK_EQ(task->get_range(), doctest::Approx(100.0));
        CHECK_EQ(task->get_node_name(), "TargetNode");
    }

    TEST_CASE_FIXTURE(BTNode3DInRangeFixture, "Node in range")
    {
        target_node->set_position(godot::Vector3(50, 50, 0));
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }

    TEST_CASE_FIXTURE(BTNode3DInRangeFixture, "Node out of range")
    {
        target_node->set_position(godot::Vector3(200, 200, 0));
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
    }

    TEST_CASE_FIXTURE(BTNode3DInRangeFixture, "Non-existent node")
    {
        blackboard->erase_var("TargetNode");
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
    }
}