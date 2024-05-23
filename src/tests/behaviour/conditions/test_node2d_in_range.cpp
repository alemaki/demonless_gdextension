#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "tests/test_utils/test_runner.hpp"

#include "behaviour/conditions/bt_node2d_in_range.hpp"

TEST_SUITE("BTNode2DInRange")
{
    TEST_CASE("BTNode2DInRange")
    {
        godot::Ref<BTNode2DInRange> task = memnew(BTNode2DInRange);
        task->set_range(100.0);
        /* TODO: Node's names snake or pascal case?*/
        task->set_node_name("TargetNode");

        SUBCASE("Set and get range and node name expected behavior")
        {
            CHECK(task->get_range() == doctest::Approx(100.0));
            CHECK(task->get_node_name() == "TargetNode");
        }

        SUBCASE("Node in range")
        {
            godot::Node2D* actor = memnew(godot::Node2D);
            actor->set_position(godot::Vector2(0, 0));

            godot::Node2D* target_node = memnew(godot::Node2D);
            target_node->set_position(godot::Vector2(50, 50));
            target_node->set_name("TargetNode");

            godot::Node* scene_root = get_scene_root();
            godot::UtilityFunctions::print(scene_root);
            scene_root->add_child(actor);
            scene_root->add_child(target_node);


            task->initialize(actor);
            auto status = task->execute(0.1);


            CHECK(status == BTTask::Status::SUCCESS);

            memfree(actor);
            memfree(target_node);
        }
        /*
        SUBCASE("Node out of range")
        {
            godot::Node2D* actor = memnew(godot::Node2D);
            actor->set_position(godot::Vector2(0, 0));

            godot::Node2D* target_node = memnew(godot::Node2D);
            target_node->set_position(godot::Vector2(200, 200));
            target_node->set_name("TargetNode");

            godot::Node* scene_root = get_scene_root();
            scene_root->add_child(actor);
            scene_root->add_child(target_node);

            task->initialize(actor);
            auto status = task->execute(0.1);

            CHECK(status == BTTask::Status::FAILURE);

            memfree(actor);
            memfree(target_node);
        }

        SUBCASE("Node not found")
        {
            godot::Node2D* actor = memnew(godot::Node2D);
            actor->set_position(godot::Vector2(0, 0));

            godot::Node* scene_root = get_scene_root();
            scene_root->add_child(actor);

            task->initialize(actor);
            auto status = task->execute(0.1);

            CHECK(status == BTTask::Status::FAILURE);

            memfree(actor);
        }*/
    }
}