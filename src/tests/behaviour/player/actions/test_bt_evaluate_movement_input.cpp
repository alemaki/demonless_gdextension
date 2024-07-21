#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/input.hpp>

#include "tests/test_utils/test_runner.hpp"
#include "tests/test_utils/test_utils.hpp"

#include "behaviour/player/actions/bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

struct BTEvaluateMovementInputFixture
{
    godot::Ref<BTEvaluateMovementInput> task;
    PlayerCharacter* player;
    CharacterInputComponent* input_component;
    CharacterMovementComponent* movement_component;
    godot::Ref<Blackboard> blackboard;

    BTEvaluateMovementInputFixture() : task(memnew(BTEvaluateMovementInput)),
                                       player(memnew(PlayerCharacter)),
                                       input_component(memnew(CharacterInputComponent)),
                                       movement_component(memnew(CharacterMovementComponent)),
                                       blackboard(memnew(Blackboard))
    {
        BehaviourTree* behaviour_tree = memnew(BehaviourTree);
        task->set_complain(false);
        task->initialize(player, blackboard);
        player->add_child(input_component);
        player->set_input_component(input_component);
        player->add_child(movement_component);
        player->set_movement_component(movement_component);
        movement_component->set_speed(100.0);

        ::get_scene_root()->add_child(player);
    }

    ~BTEvaluateMovementInputFixture()
    {
        memdelete(player);
    }
};

TEST_SUITE("BTEvaluateMovementInput")
{
    TEST_CASE_FIXTURE(BTEvaluateMovementInputFixture, "Actor is a valid player character")
    {
        CHECK_NE(task->get_actor(), nullptr);
        CHECK_EQ(godot::Object::cast_to<PlayerCharacter>(task->get_actor()), player);
    }

    TEST_CASE_FIXTURE(BTEvaluateMovementInputFixture, "Player velocity changes on input")
    {
        godot::Input::get_singleton()->action_press("ui_right");
        double delta = ::get_current_engine_delta();
        ::simulate(input_component);
        
        BTTask::Status status = task->execute(0.1);

        ::simulate(player);

        godot::Vector2 expected_velocity = godot::Vector2(100, 0);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK(::vectors_almost_eq(player->get_velocity(), expected_velocity));

        godot::Input::get_singleton()->action_release("ui_right");
    }

    TEST_CASE_FIXTURE(BTEvaluateMovementInputFixture, "Actor without input component")
    {
        player->set_input_component(nullptr);
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
        CHECK(::vectors_almost_eq(player->get_velocity(), godot::Vector2(0, 0)));
    }

    TEST_CASE_FIXTURE(BTEvaluateMovementInputFixture, "Actor is not a player character")
    {
        godot::Node* non_player_actor = memnew(godot::Node2D);

        task->initialize(non_player_actor, blackboard);
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);

        memdelete(non_player_actor);
    }
}