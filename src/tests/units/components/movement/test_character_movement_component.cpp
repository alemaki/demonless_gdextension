#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/movement/character_movement_component.hpp"
#include "tests/test_utils/test_utils.hpp"

struct CharacterMovementComponentFixture
{
    CharacterMovementComponent* movement_component;
    godot::CharacterBody3D* character;

    CharacterMovementComponentFixture() : movement_component(memnew(CharacterMovementComponent)),
                                          character(memnew(godot::CharacterBody3D)) {}
    ~CharacterMovementComponentFixture()
    {
        memdelete(movement_component);
        memdelete(character);
    }
};

TEST_SUITE("CharacterMovementComponentTests")
{
    TEST_CASE_FIXTURE(CharacterMovementComponentFixture, "Test component basics")
    {
        CHECK_EQ(movement_component->get_speed(), 1);
        CHECK_EQ(movement_component->get_friction(), 1);

        movement_component->set_speed(100);
        CHECK_EQ(movement_component->get_speed(), 100);

        movement_component->set_speed(-10);
        CHECK_EQ(movement_component->get_speed(), 0);

        movement_component->set_friction(0.5);
        CHECK_EQ(movement_component->get_friction(), 0.5);

        movement_component->set_friction(-1);
        CHECK_EQ(movement_component->get_friction(), 0);

        movement_component->set_friction(1.5);
        CHECK_EQ(movement_component->get_friction(), 1);

        godot::Vector3 target_velocity(50, 0, 0);
        movement_component->set_target_velocity(target_velocity);
        CHECK_VECTORS_EQ(movement_component->get_target_velocity(), target_velocity);

        godot::Vector3 negative_velocity(-50, -50, -50);
        movement_component->set_target_velocity(negative_velocity);
        CHECK_VECTORS_EQ(movement_component->get_target_velocity(), negative_velocity);

        movement_component->set_character(character);
        CHECK_EQ(movement_component->get_character(), character);

        movement_component->set_character(nullptr);
        CHECK_EQ(movement_component->get_character(), nullptr);
    }

    TEST_CASE_FIXTURE(CharacterMovementComponentFixture, "Test friction")
    {
        ::get_scene_root()->add_child(character);
        godot::Vector3 initial_velocity(0, 0, 0);
        character->set_velocity(initial_velocity);
        movement_component->set_character(character);
        movement_component->set_target_velocity(godot::Vector3(100, 0, 0));
        movement_component->set_friction(0.1);

        ::simulate(movement_component, 1);
        double delta = ::get_current_engine_delta();
        CHECK_VECTORS_EQ(character->get_velocity(), godot::Vector3(100, 0, 0)*movement_component->get_friction());
        CHECK_VECTORS_EQ(character->get_position(), godot::Vector3(100, 0, 0)*movement_component->get_friction()*delta);
    }
}