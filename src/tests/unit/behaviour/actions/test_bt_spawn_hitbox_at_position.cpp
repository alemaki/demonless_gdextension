#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "actor_behaviour/actions/bt_spawn_hitbox_at_position.hpp"

TEST_SUITE("BTSpawnHitboxAtPosition")
{
#define MOCK_HITBOX_PATH "res://gdextension/src/tests/mock_objects/area3d/mock_hitbox.tscn"

    TEST_CASE("Set and get hitbox path expected behavior")
    {
        godot::Ref<BTSpawnHitboxAtPosition> task = memnew(BTSpawnHitboxAtPosition);
        task->set_path_to_attack_hitbox(MOCK_HITBOX_PATH);
        task->set_complain_enabled(false);

        CHECK_EQ(task->get_path_to_attack_hitbox(), MOCK_HITBOX_PATH);
    }

    TEST_CASE("Spawns hitbox at given position")
    {
        godot::Ref<BTSpawnHitboxAtPosition> task = memnew(BTSpawnHitboxAtPosition);
        task->set_complain_enabled(false);
        task->set_path_to_attack_hitbox(MOCK_HITBOX_PATH);
        task->set_hitbox_var("hitbox_var");

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        godot::Vector3 global_position(1, 2, 3);
        blackboard->set_var("global_position", global_position);
        task->set_var_global_position("global_position");

        task->initialize(mock_actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_NE(mock_actor->get_child_count(), 0);
        Hitbox* hitbox = godot::Object::cast_to<Hitbox>(mock_actor->get_child(0));
        REQUIRE(hitbox != nullptr);
        CHECK_EQ(hitbox->get_global_position(), global_position);

        memdelete(mock_actor);
    }

    TEST_CASE("Fails when hitbox_var is not set")
    {
        godot::Ref<BTSpawnHitboxAtPosition> task = memnew(BTSpawnHitboxAtPosition);
        task->set_complain_enabled(false);
        task->set_path_to_attack_hitbox(MOCK_HITBOX_PATH);

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        task->initialize(mock_actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);

        memdelete(mock_actor);
    }

    TEST_CASE("Hitbox looks at target position if provided")
    {
        godot::Ref<BTSpawnHitboxAtPosition> task = memnew(BTSpawnHitboxAtPosition);
        task->set_complain_enabled(false);
        task->set_path_to_attack_hitbox(MOCK_HITBOX_PATH);
        task->set_hitbox_var("hitbox_var");

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        godot::Vector3 look_at_position(5, 0, 0);
        blackboard->set_var("look_at_position", look_at_position);
        task->set_var_look_at_pos("look_at_position");

        task->initialize(mock_actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
        Hitbox* hitbox = godot::Object::cast_to<Hitbox>(mock_actor->get_child(0));
        REQUIRE(hitbox != nullptr);

        CHECK_EQ(hitbox->get_rotation(), mock_actor->get_rotation());

        memdelete(mock_actor);
    }
}