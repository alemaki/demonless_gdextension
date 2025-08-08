#include "skill_action.hpp"

void SkillAction::reset()
{
    this->time_accumulated = 0;
    this->phase = SkillPhase::None;
    this->_reset();
}

void SkillAction::step(double delta)
{
    if (is_finished())
    {
        _end();
        phase = SkillPhase::Ended;
        return;
    }
    
    phase = SkillPhase::Active;
    time_accumulated += delta;
    _step(delta);
}

void SkillAction::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(SkillAction, FLOAT, duration);
}