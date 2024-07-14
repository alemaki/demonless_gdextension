#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/input.hpp>

#include "tests/test_utils/test_runner2d.hpp"
#include "tests/test_utils/test_utils.hpp"

#include "behaviour/player/actions/bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

struct BTEvaluateMovementInputFixture
{
    godot::Ref<BTEvaluateMovementInput> task;
    PlayerCharacter* player;
    CharacterInputComponent* input_component;
    godot::Ref<Blackboard> blackboard;
    BehaviourTree* behaviour_tree;

    BTEvaluateMovementInputFixture() : task(memnew(BTEvaluateMovementInput)),
                                       player(memnew(PlayerCharacter)),
                                       input_component(memnew(CharacterInputComponent)),
                                       blackboard(memnew(Blackboard)),
                                       behaviour_tree(memnew(BehaviourTree))
    {
        BehaviourTree* behaviour_tree = memnew(BehaviourTree);
        task->set_complain(false);
        task->initialize(player, blackboard);
        behaviour_tree->add_task_by_ref(task);
        player->add_child(behaviour_tree);
        player->set_behaviour_tree(behaviour_tree);
        player->add_child(input_component);
        player->set_input_component(input_component);
        player->set_name("Player");
        player->set_movement_speed(100.0);
        

        ::get_scene_root()->add_child(player);
        
        godot::Input::get_singleton()->action_press("ui_right");
    }

    ~BTEvaluateMovementInputFixture()
    {
        memdelete(player);
        godot::Input::get_singleton()->action_release("ui_right");
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
        double delta = ::get_current_engine_delta();
        ::simulate(input_component, delta);
        
        BTTask::Status status = task->execute(0.1);

        godot::Vector2 expected_velocity = godot::Vector2(100, 0);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(player->get_velocity().x, doctest::Approx(expected_velocity.x));
        CHECK_EQ(player->get_velocity().y, doctest::Approx(expected_velocity.y));
    }

    TEST_CASE_FIXTURE(BTEvaluateMovementInputFixture, "Player moves based on input")
    {
        double delta = ::get_current_engine_delta();
        ::simulate(::get_scene_root(), delta);

        godot::Vector2 expected_position = godot::Vector2(100, 0)*delta;
        godot::Vector2 expected_velocity = godot::Vector2(100, 0);
        CHECK_EQ(player->get_velocity().x, doctest::Approx(expected_velocity.x));
        CHECK_EQ(player->get_velocity().y, doctest::Approx(expected_velocity.y));
        CHECK_EQ(player->get_position().x, doctest::Approx(expected_position.x));
        CHECK_EQ(player->get_position().y, doctest::Approx(expected_position.y));
    }

    TEST_CASE_FIXTURE(BTEvaluateMovementInputFixture, "Actor without input component")
    {
        player->set_input_component(nullptr);
        BTTask::Status status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
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