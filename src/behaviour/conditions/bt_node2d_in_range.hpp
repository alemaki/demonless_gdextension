#ifndef BT_IS_PLAYER_IN_RANGE_HPP
#define BT_IS_PLAYER_IN_RANGE_HPP

#include "behaviour_tree/tasks/bt_condition.hpp"

class BTNode2DInRange : public BTCondition
{
    GDCLASS(BTNode2DInRange, BTCondition);

private: 
    double range;
    godot::StringName node_name;

protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    void set_range(double range);
    _FORCE_INLINE_ double get_range()
    {
        return this->range;
    }
    void set_node_name(const godot::String& node_name);
    _FORCE_INLINE_ godot::String get_node_name()
    {
        return this->node_name;
    }

protected:
    static void _bind_methods();
};

#endif /* BT_IS_PLAYER_IN_RANGE_HPP */