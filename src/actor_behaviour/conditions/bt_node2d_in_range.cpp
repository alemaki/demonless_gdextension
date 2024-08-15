#include "bt_node2d_in_range.hpp"

#include <godot_cpp/classes/node2d.hpp>

BTTask::Status BTNode2DInRange::_tick(double delta)
{
    godot::Node2D* actor = godot::Object::cast_to<godot::Node2D>(this->get_actor());
    godot::Node2D* node = godot::Object::cast_to<Node2D>(this->get_blackboard()->get_var(this->node_name, godot::Variant(), false));

    TASK_FAIL_COND(node == nullptr);

    TASK_SUCCEED_COND(actor->get_position().distance_to(node->get_position()) <= range);

    return BTTask::Status::FAILURE;
}

void BTNode2DInRange::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTNode2DInRange, FLOAT, range);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTNode2DInRange, STRING, node_name);

}