#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "actor_behaviour/actions/bt_spawn_node3d_at_position.hpp"

#include "tests/test_utils/test_utils.hpp"

TEST_SUITE("BTSpawnNode3DAtPosition")
{
#define MOCK_HITBOX_PATH "res://gdextension/src/tests/mock_objects/area3d/mock_hitbox.tscn"

    TEST_CASE("Set and get node3d path expected behavior")
    {
        godot::Ref<BTSpawnNode3DAtPosition> task = memnew(BTSpawnNode3DAtPosition);
        task->set_path_to_node3d(MOCK_HITBOX_PATH);
        task->set_complain_enabled(false);

        CHECK_EQ(task->get_path_to_node3d(), MOCK_HITBOX_PATH);
    }

    TEST_CASE("Spawns node3d at given position and child of actor")
    {
        godot::Ref<BTSpawnNode3DAtPosition> task = memnew(BTSpawnNode3DAtPosition);
        task->set_complain_enabled(false);
        task->set_path_to_node3d(MOCK_HITBOX_PATH);
        task->set_node3d_var("hitbox_var");

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        ::get_scene_root()->add_child(mock_actor);

        godot::Vector3 global_position(1, 2, 3);
        blackboard->set_var("global_position", global_position);
        task->set_var_global_position("global_position");
        task->set_child_of_actor(true);

        task->initialize(mock_actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
        REQUIRE_NE(mock_actor->get_child_count(), 0);
        Node3D* node3d = godot::Object::cast_to<Node3D>(mock_actor->get_child(0));
        REQUIRE(node3d != nullptr);
        CHECK_EQ(node3d->get_global_position(), global_position);

        memdelete(mock_actor);
    }

    TEST_CASE("Spawns node3d at given position and not child of actor")
    {
        godot::Ref<BTSpawnNode3DAtPosition> task = memnew(BTSpawnNode3DAtPosition);
        task->set_complain_enabled(false);
        task->set_path_to_node3d(MOCK_HITBOX_PATH);
        task->set_node3d_var("hitbox_var");

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        ::get_scene_root()->add_child(mock_actor);

        godot::Vector3 global_position(1, 2, 3);
        blackboard->set_var("global_position", global_position);
        task->set_var_global_position("global_position");
        task->set_child_of_actor(false);

        task->initialize(mock_actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(mock_actor->get_child_count(), 0);
        REQUIRE_NE(::get_scene_root()->get_child_count(), 1);
        Node3D* node3d = godot::Object::cast_to<Node3D>(::get_scene_root()->get_child(1));
        REQUIRE(node3d != nullptr);
        CHECK_EQ(node3d->get_global_position(), global_position);

        memdelete(mock_actor);
    }

    TEST_CASE("Succeeds when node3d_var is not set")
    {
        godot::Ref<BTSpawnNode3DAtPosition> task = memnew(BTSpawnNode3DAtPosition);
        task->set_complain_enabled(false);
        task->set_path_to_node3d(MOCK_HITBOX_PATH);

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        task->initialize(mock_actor, blackboard);

        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);

        memdelete(mock_actor);
    }
}