#include "combo_skill_action.hpp"

void ComboSkillAction::_ready()
{
    this->actions = ::get_all_children_of_type<SkillAction>(this);
    this->duration = 1;
    ERR_FAIL_COND_MSG(this->actions.is_empty(), vformat("[%s] %s: Has no children SkillActions", ComboSkillAction::get_class_static(), this->get_name()));
    for (int i = 0, size = this->actions.size(); i < size; i++)
    {
        this->duration += godot::Object::cast_to<SkillAction>(this->actions[i])->get_duration();
    }
    SkillAction* action = this->get_current_skill_action();
    action->reset();
}

bool ComboSkillAction::try_buffer_next_action()
{
    SkillAction* action = this->get_current_skill_action();
    if (!action)
    {
        return false;
    }
    if (action->get_time_remaining() <= this->buffer_timespan_before_action_end)
    {
        this->buffered_next_action = true;
        this->buffered_cancel = false;
        return true;
    }
    return false;
}

bool ComboSkillAction::try_buffer_cancel()
{
    SkillAction* action = this->get_current_skill_action();
    if (!action)
    {
        return false;
    }
    if (action->get_time_remaining() <= this->buffer_timespan_before_action_end)
    {
        this->buffered_next_action = false;
        this->buffered_cancel = true;
        return true;
    }
    return false;
}

void ComboSkillAction::_reset()
{
    SkillAction* action = this->get_current_skill_action();
    if (action && action->is_active())
    {
        action->end();
    }
    this->current_action_index = 0;
    this->buffered_next_action = false;
    this->buffered_cancel = false;

    action = this->get_current_skill_action();
    if (action)
    {
        action->reset();
    }
}

void ComboSkillAction::_step(double delta)
{
    ERR_FAIL_COND_MSG(this->actions.size() == 0, vformat("[%s] %s: Has no children SkillActions", ComboSkillAction::get_class_static(), this->get_name()));

    /* action should be valid always if code reaches here */
    SkillAction* action = this->get_current_skill_action();
    /* TODO: direction */
    action->step(delta);
    if (action->is_cancellable() || action->is_finished())
    {
        if (this->buffered_next_action)
        {
            action->end();
            this->current_action_index++;
            if (this->actions.size() == this->current_action_index)
            {
                this->end();
                return;
            }
            action = this->get_current_skill_action();
            action->reset();
            this->buffered_next_action = false;
        }
        else if (action->is_finished() || this->buffered_cancel)
        {
            this->end();
            this->buffered_cancel = false;
        }
    }
}

void ComboSkillAction::_end()
{
    SkillAction* action = this->get_current_skill_action();
    if (action && action->is_active())
    {
        action->end();
    }
}

SkillAction *ComboSkillAction::get_current_skill_action() const
{
    if (this->actions.size() > this->current_action_index)
    {
        return godot::Object::cast_to<SkillAction>(this->actions[this->current_action_index]);
    }
    return nullptr;
}

void ComboSkillAction::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(ComboSkillAction, FLOAT, buffer_timespan_before_action_end);
}
