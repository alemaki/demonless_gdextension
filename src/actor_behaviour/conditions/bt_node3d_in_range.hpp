#ifndef BT_IS_PLAYER_IN_RANGE_HPP
#define BT_IS_PLAYER_IN_RANGE_HPP

#include "behaviour_tree/tasks/bt_condition.hpp"

class BTNode3DInRange : public BTCondition
{
    GDCLASS(BTNode3DInRange, BTCondition);

private: 
    double range;
    godot::StringName node_name;

protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, range)
    CREATE_GETTER_SETTER_STRING_DEFAULT(node_name);

protected:
    static void _bind_methods();
};

#endif /* BT_IS_PLAYER_IN_RANGE_HPP */