#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "tests/test_utils/test_runner.hpp"

#include "behaviour/conditions/bt_node2d_in_range.hpp"

struct BTNode2DInRangeFixture
{
    godot::Ref<BTNode2DInRange> task;
    godot::Node2D* actor;
    godot::Node2D* target_node;
    godot::Node* scene_root;
    godot::Ref<Blackboard> blackboard;

    BTNode2DInRangeFixture() : task(memnew(BTNode2DInRange)),
                               actor(memnew(godot::Node2D)),
                               target_node(memnew(godot::Node2D)),
                               scene_root(get_scene_root()),
                               blackboard(memnew(Blackboard))

    {
        task->set_range(100.0);
        task->set_node_name("TargetNode");

        actor->set_position(godot::Vector2(0, 0));

        scene_root->add_child(actor);
        scene_root->add_child(target_node);

        task->initialize(actor, blackboard);
        blackboard->set_var("TargetNode", target_node);
    }
    ~BTNode2DInRangeFixture()
    {
        memdelete(actor);
        memdelete(target_node);
    }
};

TEST_SUITE("BTNode2DInRange")
{
    TEST_CASE_FIXTURE(BTNode2DInRangeFixture, "Set and get range and node name expected behavior")
    {
        CHECK_EQ(task->get_range(), doctest::Approx(100.0));
        CHECK_EQ(task->get_node_name(), "TargetNode");
    }

    TEST_CASE_FIXTURE(BTNode2DInRangeFixture, "Node in range")
    {
        target_node->set_position(godot::Vector2(50, 50));
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }

    TEST_CASE_FIXTURE(BTNode2DInRangeFixture, "Node out of range")
    {
        target_node->set_position(godot::Vector2(200, 200));
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
    }

    TEST_CASE_FIXTURE(BTNode2DInRangeFixture, "Non-existent node")
    {
        blackboard->erase_var("TargetNode");
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
    }
}