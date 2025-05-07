#ifndef MELEE_ATTACK_HPP
#define MELEE_ATTACK_HPP

#include "skills/skill_action.hpp"
#include "components/area3d/hitbox.hpp"
#include "utils/utils.hpp"

class MeleeAttack : public SkillAction
{
    GDCLASS(MeleeAttack, SkillAction)

private:
    bool is_hitbox_active = false;

protected:
    double early_cancel_endpoint = 0.2;
    double late_cancel_startpoint = 0.8;
    double hit_startpoint = 0.2;
    double hit_endpoint = 0.8;
    Hitbox* hitbox = nullptr;
    
public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, early_cancel_endpoint);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, late_cancel_startpoint);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, hit_startpoint);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, hit_endpoint);
    CREATE_GETTER_SETTER_DEFAULT(Hitbox*, hitbox);

private:
    void _reset() override;
    void _step(double delta) override;
    void _end() override;

public:
    void _ready() override;

protected:
    static void _bind_methods();
};

#endif /* MELEE_ATTACK_HPP */