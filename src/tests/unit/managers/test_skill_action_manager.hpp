#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"

#include "skill_action_manager.hpp"
#include "skill_action.hpp"

using namespace godot;

struct SkillActionManagerFixture
{
    SkillActionManager* manager = nullptr;
    SkillAction* skill1 = nullptr;
    SkillAction* skill2 = nullptr;

    SkillActionManagerFixture()
    {
        manager = memnew(SkillActionManager);
        skill1 = memnew(SkillAction);
        skill2 = memnew(SkillAction);

        skill1->set_name("Skill1");
        skill2->set_name("Skill2");

        manager->add_child(skill1);
        manager->add_child(skill2);

        ::get_scene_root()->add_child(manager);
        manager->_ready();
    }

    ~SkillActionManagerFixture()
    {
        manager->remove_child(skill1);
        manager->remove_child(skill2);
        ::get_scene_root()->remove_child(manager);

        memdelete(skill1);
        memdelete(skill2);
        memdelete(manager);
    }
};

TEST_SUITE("SkillActionManager")
{
    TEST_CASE_FIXTURE(SkillActionManagerFixture, "collects all SkillAction children into skill_map")
    {
        TypedArray<SkillAction> skills = manager->get_all_skills();

        REQUIRE_EQ(skills.size(), 2);
        CHECK_EQ(skills[0], godot::Variant(skill1));
        CHECK_EQ(skills[1], godot::Variant(skill2));
    }

    TEST_CASE_FIXTURE(SkillActionManagerFixture, "can get SkillAction by name")
    {
        TypedArray<SkillAction> skills = manager->get_all_skills();
        CHECK(manager->get_skill("Skill1") == skill1);
        CHECK(manager->get_skill("Skill2") == skill2);
    }
}

TEST_SUITE("[errors]" "SkillActionManager")
{
    TEST_CASE("prints error for non-SkillAction child")
    {
        SkillActionManager* mgr = memnew(SkillActionManager);
        Node3D* invalid_child = memnew(Node3D);

        invalid_child->set_name("FakeSkill");
        mgr->add_child(invalid_child);
        ::get_scene_root()->add_child(mgr);

        CHECK_GODOT_ERROR(mgr->_ready());

        memdelete(invalid_child);
        memdelete(mgr);
    }

    TEST_CASE("handles duplicate skill names with ERR_FAIL_COND")
    {
        SkillActionManager* mgr = memnew(SkillActionManager);
        SkillAction* skillA = memnew(SkillAction);
        SkillAction* skillB = memnew(SkillAction);

        skillA->set_name("DuplicateSkill");
        skillB->set_name("DuplicateSkill");

        mgr->add_child(skillA);
        mgr->add_child(skillB);

        ::get_scene_root()->add_child(mgr);

        CHECK_GODOT_ERROR(mgr->_ready());

        memdelete(skillA);
        memdelete(skillB);
        memdelete(mgr);
    }

    TEST_CASE_FIXTURE(SkillActionManagerFixture, "get_skill returns nullptr and logs error when skill missing")
    {
        SkillAction* result;
        CHECK_GODOT_ERROR(result = manager->get_skill("NonexistentSkill"));
        CHECK_EQ(result, nullptr);
    }
}