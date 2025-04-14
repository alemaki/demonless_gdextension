#ifndef COMBO_ATTACK_STAGE_HPP
#define COMBO_ATTACK_STAGE_HPP

#include "skills/skill_action.hpp"

class ComboAttackStage : public SkillAction
{
    GDCLASS(ComboAttackStage, SkillAction)

protected:
    /* TODO: store animation here? Maybe no - shouldn't be owner of the animation */
    double until_cancel_window = 0.2;
    double after_cancel_window = 0.8;
    bool change_direction_allowed = false;
    godot::Vector3 direction = {1, 0, 0};

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, until_cancel_window);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, after_cancel_window);
    CREATE_GETTER_SETTER_BOOL_DEFAULT(change_direction_allowed);

    /* Can change direction only if allowed or before starting the attack */
    void set_direction(const godot::Vector3 direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->direction;
    }

private:
    void _reset() override;
    void _step(double delta) override;
    void _end() override;

protected:
    static void _bind_methods();
};

#endif /* COMBO_ATTACK_STAGE_HPP */