#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "tests/test_utils/test_macros.hpp"

#include "actors/player/player_character.hpp"

struct PlayerCharacterFixture
{
    PlayerCharacter* player;
    CharacterInputComponent* input_component;
    CharacterMovementComponent* movement_component;
    godot::MeshInstance3D* mesh;
    godot::AnimationPlayer* animation_player;

    PlayerCharacterFixture() :
        player(memnew(PlayerCharacter)),
        input_component(memnew(CharacterInputComponent)),
        movement_component(memnew(CharacterMovementComponent)),
        mesh(memnew(MeshInstance3D)),
        animation_player(memnew(AnimationPlayer))
    {
        player->add_child(input_component);
        player->set_input_component(input_component);
        player->set_name("Player");
        player->add_child(movement_component);
        player->set_movement_component(movement_component);
        mesh->add_child(animation_player);
        animation_player->set_name("AnimationPlayer");
        animation_player->set_owner(mesh);
        player->add_child(mesh);
        player->set_mesh_instance(mesh);
        movement_component->set_speed(100.0);
        
        ::get_scene_root()->add_child(player);
    }

    ~PlayerCharacterFixture()
    {
        memdelete(player);
    }
};

TEST_SUITE("PlayerCharacter")
{
    TEST_CASE_FIXTURE(PlayerCharacterFixture, "Player looks in the right direction when moving")
    {
        godot::Input::get_singleton()->action_press("move_right");

        double delta = ::get_current_engine_delta();
        ::simulate(::get_scene_root());

        godot::Vector3 expected_direction = godot::Vector3(1, 0, 0);
        // TODO 

        godot::Input::get_singleton()->action_release("move_right");
    }

    TEST_CASE_FIXTURE(PlayerCharacterFixture, "Player moves in the right direction based on input")
    {
        godot::Input::get_singleton()->action_press("move_down");

        double delta = ::get_current_engine_delta();
        ::simulate(::get_scene_root());

        godot::Vector3 expected_position = godot::Vector3(0, 0, 100) * delta;
        godot::Vector3 expected_velocity = godot::Vector3(0, 0, 100);
        CHECK_VECTORS_EQ(player->get_velocity(), expected_velocity);
        CHECK_VECTORS_EQ(player->get_position(), expected_position);

        godot::Input::get_singleton()->action_release("move_down");
    }

    TEST_CASE_FIXTURE(PlayerCharacterFixture, "Player stops moving after input is released")
    {
        godot::Input::get_singleton()->action_press("move_left");

        double delta = ::get_current_engine_delta();
        ::simulate(::get_scene_root());

        godot::Vector3 initial_position = player->get_position();
        godot::Vector3 initial_velocity = player->get_velocity();

        godot::Input::get_singleton()->action_release("move_left");
        ::simulate(::get_scene_root());

        CHECK_VECTORS_EQ(player->get_position(), initial_position);
        CHECK_VECTORS_EQ(player->get_velocity(), godot::Vector3(0, 0, 0));
    }
}