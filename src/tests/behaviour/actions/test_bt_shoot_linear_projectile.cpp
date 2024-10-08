#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "actor_behaviour/actions/bt_shoot_linear_projectile.hpp"

TEST_SUITE("BTShootLinearProjectile")
{

TEST_CASE("BTShootLinearProjectile")
{
    godot::Ref<BTShootLinearProjectile> task = memnew(BTShootLinearProjectile);
    task->set_linear_projectile_scene_path("res://gdextension/src/tests/mock_objects/projectiles/MockLinearProjectile.tscn");
    task->set_complain_enabled(false);

    SUBCASE("Set and get scene path expected behavior")
    {
        CHECK_EQ(task->get_linear_projectile_scene_path(), "res://gdextension/src/tests/mock_objects/projectiles/MockLinearProjectile.tscn");
    }

    SUBCASE("Creates projectile")
    {
        godot::Node3D* mock_actor = memnew(godot::Node3D);

        task->initialize(mock_actor, memnew(Blackboard));
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::SUCCESS);
        CHECK_EQ(mock_actor->get_child(0)->get_class(), LinearProjectile::get_class_static());

        memdelete(mock_actor);
    }
}

}