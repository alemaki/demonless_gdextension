#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "actor_behaviour/player/conditions/bt_character_input_pressed.hpp"
#include "actors/player/player_character.hpp"


struct BTCharacterInputPressedFixture
{
    godot::Ref<BTCharacterInputPressed> task;
    godot::Node* actor;
    godot::Ref<Blackboard> blackboard;

    BTCharacterInputPressedFixture() : task(memnew(BTCharacterInputPressed)),
                                       actor(memnew(godot::Node)),
                                       blackboard(memnew(Blackboard))
    {
        task->set_complain(false);
        task->set_check_attack_pressed(false);
        task->set_check_block_pressed(false);
        task->set_check_direction_pressed(false);

        blackboard->set_var("is_attack_pressed", false);
        blackboard->set_var("is_block_pressed", false);
        blackboard->set_var("direction_input", godot::Vector2(0, 0));

        task->initialize(actor, blackboard);
    }

    ~BTCharacterInputPressedFixture()
    {
        memdelete(actor);
    }
};

TEST_SUITE("BTCharacterInputPressed")
{
    TEST_CASE_FIXTURE(BTCharacterInputPressedFixture, "Attack input pressed")
    {
        task->set_check_attack_pressed(true);
        blackboard->set_var("is_attack_pressed", true);

        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }

    TEST_CASE_FIXTURE(BTCharacterInputPressedFixture, "Block input pressed")
    {
        task->set_check_block_pressed(true);
        blackboard->set_var("is_block_pressed", true);

        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }

    TEST_CASE_FIXTURE(BTCharacterInputPressedFixture, "Direction input pressed")
    {
        task->set_check_direction_pressed(true);
        blackboard->set_var("direction_input", godot::Vector2(1, 0));

        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }

    TEST_CASE_FIXTURE(BTCharacterInputPressedFixture, "No input pressed")
    {
        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::FAILURE);
    }

    TEST_CASE_FIXTURE(BTCharacterInputPressedFixture, "Multiple inputs pressed")
    {
        task->set_check_attack_pressed(true);
        task->set_check_block_pressed(true);
        blackboard->set_var("is_attack_pressed", true);
        blackboard->set_var("is_block_pressed", true);

        BTTask::Status status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }
};