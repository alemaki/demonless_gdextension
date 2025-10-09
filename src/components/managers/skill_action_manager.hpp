#ifndef SKILL_ACTION_MANAGER_HPP
#define SKILL_ACTION_MANAGER_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/templates/hash_map.hpp>

#include "skills/skill_action.hpp"

using namespace godot;

/* TODO: make it generic manager */
class SkillActionManager : public Node
{
    GDCLASS(SkillActionManager, Node);

private:
    HashMap<StringName, SkillAction*> skill_map;

public:
    void _ready() override;

    SkillAction *get_skill(const StringName &name) const;
    TypedArray<SkillAction> get_all_skills() const;

protected:
    static void _bind_methods();
};

#endif // SKILL_ACTION_MANAGER_HPP