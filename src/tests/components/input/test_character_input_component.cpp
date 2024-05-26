#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/input.hpp>

#include "components/input/character_input_component.hpp"


void simulate(CharacterInputComponent* component, float delta, int frames)
{
	for (int i = 0; i < frames; ++i)
    {
		component->_process(delta);
	}
}
TEST_SUITE("TestCaracterInputComponent")
{
    TEST_CASE("Test Character input component")
    {
        CharacterInputComponent* character_input_component = memnew(CharacterInputComponent);

        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 0));

        godot::Input::get_singleton()->action_press("ui_right");
        simulate(character_input_component, 1.0f, 1);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(1, 0));

        godot::Input::get_singleton()->action_press("ui_left");
        simulate(character_input_component, 1.0f, 1);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 0));

        godot::Input::get_singleton()->action_release("ui_right");
        godot::Input::get_singleton()->action_release("ui_left");

        godot::Input::get_singleton()->action_press("ui_up");
        simulate(character_input_component, 1.0f, 1);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, -1));
        godot::Input::get_singleton()->action_release("ui_up");

        godot::Input::get_singleton()->action_press("ui_down");
        simulate(character_input_component, 1.0f, 1);
        CHECK_EQ(character_input_component->get_direction_input(), godot::Vector2(0, 1));

        godot::Input::get_singleton()->action_press("ui_right");
        simulate(character_input_component, 1.0f, 1);
        CHECK_EQ(character_input_component->get_direction_input(), (godot::Vector2(0, 1) + godot::Vector2(1, 0)).normalized());

        godot::Input::get_singleton()->action_release("ui_right");
        godot::Input::get_singleton()->action_release("ui_down");

        memdelete(character_input_component);
    }
}