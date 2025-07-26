#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/classes/node3d.hpp>

#include "components/area3d/hitbox.hpp"
#include "actor_behaviour/actions/bt_execute_attack.hpp"
#include "attacks/attack.hpp"

struct BTExecuteAttackFixture
{
    Attack* attack = memnew(Attack);
    godot::Ref<BTExecuteAttack> task = memnew(BTExecuteAttack);
    godot::Ref<Blackboard> blackboard = memnew(Blackboard);
    godot::Node3D* mock_actor = memnew(godot::Node3D);
    BTExecuteAttackFixture()
    {
        blackboard->set_var("attack_name", attack);
        task->set_attack_to_execute("attack_name");
        task->set_blackboard(blackboard);
        task->initialize(mock_actor, blackboard);
    }

    ~BTExecuteAttackFixture()
    {
        memdelete(mock_actor);
        memdelete(attack);
    }
};


TEST_SUITE("BTExecuteAttack")
{
    TEST_CASE_FIXTURE(BTExecuteAttackFixture, "BTExecuteAttack basic")
    {
        task->set_attack_to_execute("attack_name");

        CHECK_EQ(task->get_attack_to_execute(), godot::StringName("attack_name"));
    }

    TEST_CASE_FIXTURE(BTExecuteAttackFixture, "Steps in attack and returns running")
    {
        auto status = task->execute(0.1);

        CHECK_EQ(status, BTTask::Status::RUNNING);
        CHECK_EQ(attack->get_time_accumulated(), doctest::Approx(0.1));
    }

    TEST_CASE_FIXTURE(BTExecuteAttackFixture, "Returns success when attack is done")
    {
        auto status = task->execute(1);

        CHECK_EQ(status, BTTask::Status::RUNNING);
        CHECK_EQ(attack->get_time_accumulated(), doctest::Approx(1));

        status = task->execute(0.1);
        CHECK_EQ(status, BTTask::Status::SUCCESS);
    }

    TEST_CASE_FIXTURE(BTExecuteAttackFixture, "Resets attack when reentered")
    {
        auto status = task->execute(1);

        CHECK_EQ(status, BTTask::Status::RUNNING);
        CHECK_EQ(attack->get_time_accumulated(), doctest::Approx(1));

        task->abort();
        status = task->execute(0.1);
        CHECK_EQ(attack->get_time_accumulated(), doctest::Approx(0.1));
        CHECK_EQ(status, BTTask::Status::RUNNING);
    }
}


TEST_SUITE("[Error] BTExecuteAttack")
{
    TEST_CASE_FIXTURE(BTExecuteAttackFixture, "Test basic errors")
    {
        blackboard->erase_var("attack_name");
        BTTask::Status status;
        CHECK_GODOT_ERROR(task->abort());
        CHECK_GODOT_ERROR(status = task->execute(1));

        blackboard->set_var("attack_name", nullptr);
        CHECK_GODOT_ERROR(task->abort());
        CHECK_GODOT_ERROR(status = task->execute(1));
    }
}