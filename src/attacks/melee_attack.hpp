#ifndef MELEE_ATTACK_HPP
#define MELEE_ATTACK_HPP

#include "skills/skill_action.hpp"

class MeleeAttack : public SkillAction
{
    GDCLASS(MeleeAttack, SkillAction)

protected:
    double early_cancel_endpoint = 0.2;
    double late_cancel_startpoint = 0.8;
    /* TODO: Hitbox */
public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, early_cancel_endpoint);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, late_cancel_startpoint);

private:
    void _reset() override;
    void _step(double delta) override;
    void _end() override;

protected:
    static void _bind_methods();
};

#endif /* MELEE_ATTACK_HPP */