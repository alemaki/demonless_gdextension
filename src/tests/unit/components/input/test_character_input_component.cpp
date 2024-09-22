#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include "components/input/character_input_component.hpp"

#include "tests/test_utils/test_utils.hpp"

struct CharacterInputComponentFixture
{
    CharacterInputComponent* character_input_component;

    CharacterInputComponentFixture() : character_input_component(memnew(CharacterInputComponent)){}
    ~CharacterInputComponentFixture()
    {
        memdelete(character_input_component);
    }
};

TEST_SUITE("TestCaracterInputComponent")
{
    TEST_CASE_FIXTURE(CharacterInputComponentFixture, "Test actions exist")
    {
        godot::InputMap* input_map = godot::InputMap::get_singleton();

        REQUIRE_NE(input_map, nullptr);
        REQUIRE(input_map->has_action("attack"));
        REQUIRE(input_map->has_action("block"));
        REQUIRE(input_map->has_action("move_right"));
        REQUIRE(input_map->has_action("move_left"));
        REQUIRE(input_map->has_action("move_down"));
        REQUIRE(input_map->has_action("move_up"));
    }

    TEST_CASE_FIXTURE(CharacterInputComponentFixture, "Test directional input")
    {
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), godot::Vector3(0, 0, 0));
        simulate(character_input_component);
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), godot::Vector3(0, 0, 0));

        godot::Input::get_singleton()->action_press("move_right");
        simulate(character_input_component);
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), godot::Vector3(1, 0, 0));

        godot::Input::get_singleton()->action_press("move_left");
        simulate(character_input_component);
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), godot::Vector3(0, 0, 0));

        godot::Input::get_singleton()->action_release("move_right");
        godot::Input::get_singleton()->action_release("move_left");

        godot::Input::get_singleton()->action_press("move_up");
        simulate(character_input_component);
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), godot::Vector3(0, 0, -1));
        godot::Input::get_singleton()->action_release("move_up");

        godot::Input::get_singleton()->action_press("move_down");
        simulate(character_input_component);
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), godot::Vector3(0, 0, 1));

        godot::Input::get_singleton()->action_press("move_right");
        simulate(character_input_component);
        CHECK_VECTORS_EQ(character_input_component->get_direction_input(), (godot::Vector3(0, 0, 1) + godot::Vector3(1, 0, 0)).normalized());

        godot::Input::get_singleton()->action_release("move_right");
        godot::Input::get_singleton()->action_release("move_down");
    }

    TEST_CASE_FIXTURE(CharacterInputComponentFixture, "Test Character input component")
    {
        CHECK_FALSE(character_input_component->is_attack_pressed());
        CHECK_FALSE(character_input_component->is_block_pressed());

        simulate(character_input_component);
        CHECK_FALSE(character_input_component->is_attack_pressed());
        CHECK_FALSE(character_input_component->is_block_pressed());

        godot::Input::get_singleton()->action_press("attack");
        simulate(character_input_component);
        CHECK(character_input_component->is_attack_pressed());

        godot::Input::get_singleton()->action_press("block");
        simulate(character_input_component);
        CHECK(character_input_component->is_block_pressed());

        godot::Input::get_singleton()->action_release("attack");
        simulate(character_input_component);
        CHECK_FALSE(character_input_component->is_attack_pressed());

        godot::Input::get_singleton()->action_release("block");
        simulate(character_input_component);
        CHECK_FALSE(character_input_component->is_block_pressed());
    }

    TEST_CASE_FIXTURE(CharacterInputComponentFixture, "Test mouse casted position")
    {
        godot::Camera3D* camera = memnew(godot::Camera3D);

        godot::Viewport* viewport = ::get_scene_root()->get_viewport();
        REQUIRE(viewport != nullptr);
        viewport->add_child(camera);

        get_scene_root()->add_child(character_input_component);
        
        camera->set_position(godot::Vector3(0, 10, 0));
        camera->look_at(godot::Vector3(0, 0, 0), godot::Vector3(0, 0, -1)); // up from camera should be -1 by Z axis

        godot::Vector2 viewport_size = viewport->get_visible_rect().get_size();
        godot::Input::get_singleton()->warp_mouse(viewport_size / 2);
        
        ::simulate(get_scene_root());
        
        godot::Vector3 result = character_input_component->get_mouse_casted_position();
        
        CHECK_VECTORS_EQ(result, godot::Vector3(0, 0, 0));

        memdelete(camera);
    }
}