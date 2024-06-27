#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>

#include "components/input/character_input_component.hpp"

#include "tests/test_utils/test_utils.hpp"

TEST_SUITE("TestCaracterInputComponent")
{
    TEST_CASE("Test actions exist")
    {
        CharacterInputComponent* character_input_component = memnew(CharacterInputComponent);
        godot::InputMap* input_map = godot::InputMap::get_singleton();

        REQUIRE_NE(input_map, nullptr);
        REQUIRE(input_map->has_action("ui_attack"));
        REQUIRE(input_map->has_action("ui_block"));
        REQUIRE(input_map->has_action("ui_right"));
        REQUIRE(input_map->has_action("ui_left"));
        REQUIRE(input_map->has_action("ui_down"));
        REQUIRE(input_map->has_action("ui_up"));

        memdelete(character_input_component);
    }

    TEST_CASE("Test directional input")
    {
        CharacterInputComponent* character_input_component = memnew(CharacterInputComponent);

        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 0));
        simulate(character_input_component);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 0));

        godot::Input::get_singleton()->action_press("ui_right");
        simulate(character_input_component);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(1, 0));

        godot::Input::get_singleton()->action_press("ui_left");
        simulate(character_input_component);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 0));

        godot::Input::get_singleton()->action_release("ui_right");
        godot::Input::get_singleton()->action_release("ui_left");

        godot::Input::get_singleton()->action_press("ui_up");
        simulate(character_input_component);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, -1));
        godot::Input::get_singleton()->action_release("ui_up");

        godot::Input::get_singleton()->action_press("ui_down");
        simulate(character_input_component);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 1));

        godot::Input::get_singleton()->action_press("ui_right");
        simulate(character_input_component);
        CHECK_EQ(character_input_component->get_direction_input(), (godot::Vector2(0, 1) + godot::Vector2(1, 0)).normalized());

        godot::Input::get_singleton()->action_release("ui_right");
        godot::Input::get_singleton()->action_release("ui_down");

        memdelete(character_input_component);
    }

    TEST_CASE("Test Character input component")
    {
        CharacterInputComponent* character_input_component = memnew(CharacterInputComponent);

        CHECK_FALSE(character_input_component->is_attack_pressed());
        CHECK_FALSE(character_input_component->is_block_pressed());

        simulate(character_input_component);
        CHECK_FALSE(character_input_component->is_attack_pressed());
        CHECK_FALSE(character_input_component->is_block_pressed());

        godot::Input::get_singleton()->action_press("ui_attack");
        simulate(character_input_component);
        CHECK(character_input_component->is_attack_pressed());

        godot::Input::get_singleton()->action_press("ui_block");
        simulate(character_input_component);
        CHECK(character_input_component->is_block_pressed());

        godot::Input::get_singleton()->action_release("ui_attack");
        simulate(character_input_component);
        CHECK_FALSE(character_input_component->is_attack_pressed());

        godot::Input::get_singleton()->action_release("ui_block");
        simulate(character_input_component);
        CHECK_FALSE(character_input_component->is_block_pressed());
        

        memdelete(character_input_component);
    }
}