#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "tests/test_utils/test_utils.hpp"
#include "actor_behaviour/player/actions/bt_set_target_direction.hpp"
#include "actors/player/player_character.hpp"
#include "blackboard/blackboard.hpp"

struct SetTargetDirectionFixture
{
    godot::Ref<BTSetTargetDirection> task;
    PlayerCharacter* player;
    CharacterMovementComponent* movement_cp;
    godot::Ref<Blackboard> blackboard;

    SetTargetDirectionFixture() : task(memnew(BTSetTargetDirection)),
                                  player(memnew(PlayerCharacter)),
                                  movement_cp(memnew(CharacterMovementComponent)),
                                  blackboard(memnew(Blackboard))
    {
        player->set_movement_component(movement_cp);
        task->set_complain_enabled(false);
        task->set_target_direction(godot::Vector2(1, 1));
        task->initialize(player, blackboard);
    }

    ~SetTargetDirectionFixture()
    {
        memdelete(player);
    }
};

TEST_SUITE("BTSetTargetDirection")
{
    TEST_CASE_FIXTURE(SetTargetDirectionFixture, "Set target direction success")
    {
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_VECTORS_EQ(movement_cp->get_target_velocity().normalized(), godot::Vector3(1, 0, 1).normalized());
    }

    TEST_CASE_FIXTURE(SetTargetDirectionFixture, "Fail when actor is not a PlayerCharacter")
    {
        // Initialize with a non-player actor
        godot::Node* non_player = memnew(godot::Node);
        task->initialize(non_player, blackboard);
        
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::FAILURE);
        
        memdelete(non_player);
    }

    TEST_CASE_FIXTURE(SetTargetDirectionFixture, "Fail when PlayerCharacter has no movement component")
    {
        player->set_movement_component(nullptr);
        memdelete(movement_cp); // Clean up component manually

        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::FAILURE);
    }
}