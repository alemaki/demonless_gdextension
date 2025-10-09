#include "skill_action_manager.hpp"

void SkillActionManager::_ready()
{
    skill_map.clear();

    const int child_count = get_child_count();

    for (int i = 0; i < child_count; i++) {
        Node *child = get_child(i);

        SkillAction *skill = Object::cast_to<SkillAction>(child);
        ERR_CONTINUE_MSG(!skill, vformat("[SkillActionManager]: Child '%s' is not of type SkillAction!",
                                          child->get_name()));

        StringName skill_name = skill->get_name();

        ERR_CONTINUE_MSG(skill_map.has(skill_name), vformat("[SkillActionManager]: Duplicate skill name '%s' detected!",
                                                             skill_name));

        skill_map[skill_name] = skill;
    }
}

SkillAction *SkillActionManager::get_skill(const StringName &name) const
{
    ERR_FAIL_COND_V_MSG(!skill_map.has(name), nullptr,"[SkillActionManager]: No skill found with name '" + name + "'.");
    return skill_map[name];
}

TypedArray<SkillAction> SkillActionManager::get_all_skills() const
{
    TypedArray<SkillAction> skills;
    for (const godot::KeyValue<godot::StringName, SkillAction *> key_value : skill_map)
    {
        skills.push_back(key_value.value);
    }
    return skills;
}

void SkillActionManager::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_skill", "name"), &SkillActionManager::get_skill);
    ClassDB::bind_method(D_METHOD("get_all_skills"), &SkillActionManager::get_all_skills);
}