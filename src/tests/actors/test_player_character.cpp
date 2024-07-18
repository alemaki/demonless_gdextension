#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/input.hpp>

#include "tests/test_utils/test_runner.hpp"
#include "tests/test_utils/test_utils.hpp"

#include "behaviour/player/actions/bt_evaluate_movement_input.hpp"
#include "actors/player/player_character.hpp"

struct PlayerCharacterFixture
{
    godot::Ref<BTEvaluateMovementInput> task;
    PlayerCharacter* player;
    CharacterInputComponent* input_component;
    godot::Ref<Blackboard> blackboard;
    BehaviourTree* behaviour_tree;

    PlayerCharacterFixture() : task(memnew(BTEvaluateMovementInput)),
                               player(memnew(PlayerCharacter)),
                               input_component(memnew(CharacterInputComponent)),
                               blackboard(memnew(Blackboard)),
                               behaviour_tree(memnew(BehaviourTree))
    {
        BehaviourTree* behaviour_tree = memnew(BehaviourTree);
        task->set_complain(false);
        behaviour_tree->add_task_by_ref(task);
        player->add_child(behaviour_tree);
        player->set_behaviour_tree(behaviour_tree);
        player->add_child(input_component);
        player->set_input_component(input_component);
        player->set_name("Player");
        player->set_movement_speed(100.0);
        
        ::get_scene_root()->add_child(player);
    }

    ~PlayerCharacterFixture()
    {
        memdelete(player);
    }
};

TEST_SUITE("PlayerCharacter")
{
    TEST_CASE_FIXTURE(PlayerCharacterFixture, "Player moves based on input")
    {
        godot::Input::get_singleton()->action_press("ui_right");

        double delta = ::get_current_engine_delta();
        ::simulate(::get_scene_root());

        godot::Vector2 expected_position = godot::Vector2(100, 0)*delta;
        godot::Vector2 expected_velocity = godot::Vector2(100, 0);
        CHECK(::vectors_almost_equal(player->get_velocity(), expected_velocity));
        CHECK(::vectors_almost_equal(player->get_position(), expected_position));

        godot::Input::get_singleton()->action_release("ui_right");
    }
}