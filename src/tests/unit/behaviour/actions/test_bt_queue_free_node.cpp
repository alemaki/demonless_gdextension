#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/node3d.hpp>

#include "components/area3d/hitbox.hpp"
#include "actor_behaviour/actions/bt_queue_free_node.hpp"

TEST_SUITE("BTQueueFreeNode")
{
    TEST_CASE("Set and get hitbox variable name")
    {
        godot::Ref<BTQueueFreeNode> task = memnew(BTQueueFreeNode);
        task->set_var_node("hitbox_var");

        CHECK_EQ(task->get_var_node(), godot::StringName("hitbox_var"));
    }

    TEST_CASE("Despawn node successfully")
    {
        godot::Ref<BTQueueFreeNode> task = memnew(BTQueueFreeNode);
        task->set_var_node("hitbox_var");
        task->set_complain_enabled(false);

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        Hitbox* mock_hitbox = memnew(Hitbox);
        mock_actor->add_child(mock_hitbox);

        blackboard->set_var("hitbox_var", mock_hitbox);

        task->initialize(mock_actor, blackboard);

        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);

        CHECK_EQ(mock_actor->get_child_count(), 0);
        REQUIRE_NE(mock_hitbox, nullptr);
        CHECK(mock_hitbox->is_queued_for_deletion());

        memdelete(mock_actor);
    }

    TEST_CASE("Fails if node is not found")
    {
        godot::Ref<BTQueueFreeNode> task = memnew(BTQueueFreeNode);
        task->set_var_node("hitbox_var");
        task->set_complain_enabled(false);

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        task->initialize(mock_actor, blackboard);

        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);

        memdelete(mock_actor);
    }

    TEST_CASE("Fails if node is not castable")
    {
        godot::Ref<BTQueueFreeNode> task = memnew(BTQueueFreeNode);
        task->set_var_node("hitbox_var");
        task->set_complain_enabled(false);

        godot::Node3D* mock_actor = memnew(godot::Node3D);
        godot::Ref<Blackboard> blackboard = memnew(Blackboard);

        blackboard->set_var("hitbox_var", memnew(godot::Resource));

        task->initialize(mock_actor, blackboard);

        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);

        memdelete(mock_actor);
    }
}
