#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/camera3d.hpp>

#include "actor_behaviour/player/actions/bt_save_mouse_position.hpp"
#include "actors/player/player_character.hpp"
#include "components/input/character_input_component.hpp"
#include "tests/test_utils/test_runner.hpp"

struct BTSaveMousePositionFixture
{
    godot::Ref<BTSaveMousePosition> task;
    godot::Camera3D* camera;
    PlayerCharacter* player;
    CharacterInputComponent* input_component;
    godot::Ref<Blackboard> blackboard;

    BTSaveMousePositionFixture() :
        task(memnew(BTSaveMousePosition)),
        player(memnew(PlayerCharacter)),
        camera(memnew(godot::Camera3D)),
        input_component(memnew(CharacterInputComponent)),
        blackboard(memnew(Blackboard))
    {
        task->set_complain_enabled(false);
        task->set_mouse_position_var("mouse_pos");

        ::get_scene_root()->add_child(player);

        player->add_child(input_component);
        player->set_input_component(input_component);

        player->add_child(camera);
        camera->set_position(godot::Vector3(0, 10, 0));
        camera->look_at(player->get_position(), godot::Vector3(0, 0, -1));
        task->initialize(player, blackboard);
    }

    ~BTSaveMousePositionFixture()
    {
        memdelete(player);
    }
};

TEST_SUITE("BTSaveMousePosition")
{
    TEST_CASE_FIXTURE(BTSaveMousePositionFixture, "Set and get mouse_position_var")
    {
        CHECK_EQ(task->get_mouse_position_var(), godot::StringName("mouse_pos"));
    }

    TEST_CASE_FIXTURE(BTSaveMousePositionFixture, "Fails if actor is not PlayerCharacter")
    {
        godot::Node* invalid_actor = memnew(godot::Node);

        task->initialize(invalid_actor, blackboard);
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
        memdelete(invalid_actor);
    }

    TEST_CASE_FIXTURE(BTSaveMousePositionFixture, "Fails if PlayerCharacter lacks input component")
    {
        player->remove_child(input_component);
        player->set_input_component(nullptr);

        auto status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::FAILURE);
    }

    TEST_CASE_FIXTURE(BTSaveMousePositionFixture, "Saves mouse position successfully")
    {
        godot::Vector3 mock_mouse_position = input_component->get_mouse_casted_position();

        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(blackboard->get_var("mouse_pos"), mock_mouse_position);
    }

    TEST_CASE_FIXTURE(BTSaveMousePositionFixture, "Does nothing if mouse_position_var is empty")
    {
        task->set_mouse_position_var("");
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::FAILURE);
        CHECK_FALSE(blackboard->has_var("mouse_pos"));
    }
}