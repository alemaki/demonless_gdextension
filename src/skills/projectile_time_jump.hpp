#ifndef PROJECTILE_TIME_ACTION_HPP
#define PROJECTILE_TIME_ACTION_HPP

#include "skills/skill_action.hpp"

class ProjectileTimeJump : public SkillAction
{
    GDCLASS(ProjectileTimeJump, SkillAction)

    double time_skip = 1;

protected:
    virtual void _step(double delta) override;

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, time_skip);

protected:
    static void _bind_methods();
};

#endif /* PROJECTILE_TIME_ACTION_HPP */