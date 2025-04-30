#ifndef COMBO_SKILL_ACTION_HPP
#define COMBO_SKILL_ACTION_HPP

#include "skills/skill_action.hpp"

class ComboSkillAction : public SkillAction
{
    GDCLASS(ComboSkillAction, SkillAction)
private:
    int current_action_index = 0;
    godot::TypedArray<SkillAction> actions;

protected:
    /* Will buffer a cancel or next action */
    double buffer_timespan_before_action_end = 0.5;
    bool buffered_next_action = false;
    bool buffered_cancel = false;

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, buffer_timespan_before_action_end);

    void _ready();
    bool try_buffer_next_action();
    bool try_buffer_cancel();

    godot::StringName get_animation() const override;

private:
    void _reset() override;
    void _step(double delta) override;
    void _end() override;

    SkillAction* get_current_skill_action() const;

protected:
    static void _bind_methods();
};

#endif /* COMBO_SKILL_ACTION_HPP */