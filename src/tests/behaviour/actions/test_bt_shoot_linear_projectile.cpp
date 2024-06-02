#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "behaviour/actions/bt_shoot_linear_projectile.hpp"

TEST_SUITE("BTShootLinearProjectile")
{

TEST_CASE("BTShootLinearProjectile")
{
    godot::Ref<BTShootLinearProjectile> task = memnew(BTShootLinearProjectile);
    task->set_linear_projectile_scene_path("res://gdextension/src/tests/mock_objects/projectiles/MockLinearProjectile.tscn");

    SUBCASE("Set and get scene path expected behavior")
    {
        CHECK(task->get_linear_projectile_scene_path() == "res://gdextension/src/tests/mock_objects/projectiles/MockLinearProjectile.tscn");
    }

    SUBCASE("Creates projectile")
    {
        godot::Node2D* mock_actor = memnew(godot::Node2D);

        task->initialize(mock_actor, memnew(Blackboard));
        auto status = task->execute(0.1);

        CHECK(status == BTTask::Status::SUCCESS);
        CHECK(mock_actor->get_child(0)->get_class() == LinearProjectile::get_class_static());

        memdelete(mock_actor);
    }
}

}