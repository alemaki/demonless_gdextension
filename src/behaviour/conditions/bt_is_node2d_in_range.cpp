#include "bt_is_node2d_in_range.hpp"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

BTTask::Status BTIsNode2DInRange::_tick(double delta)
{
    godot::Node2D* actor = godot::Object::cast_to<godot::Node2D>(this->get_actor());
    /* TODO: make it blackboard reference. */
    godot::Node2D* node = godot::Object::cast_to<godot::Node2D>(actor->get_tree()->get_root()->find_child(this->node_name, true));
    
    TASK_FAIL_COND(node == nullptr);

    if (actor->get_position().distance_to(node->get_position()) < range)
    {
        return BTTask::Status::SUCCESS;
    }

    return BTTask::Status::FAILURE;
}

void BTIsNode2DInRange::set_range(double range)
{
    this->range = range;
}

void BTIsNode2DInRange::set_node_name(const godot::String& node_name)
{
    this->node_name = node_name;
}

void BTIsNode2DInRange::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_range", "range"), &BTIsNode2DInRange::set_range);
    ClassDB::bind_method(D_METHOD("get_range"), &BTIsNode2DInRange::get_range);
    ClassDB::bind_method(D_METHOD("set_node_name", "node_name"), &BTIsNode2DInRange::set_node_name);
    ClassDB::bind_method(D_METHOD("_tick"), &BTIsNode2DInRange::_tick);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "range"), "set_range", "get_range");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "node_name"), "set_node_name", "get_node_name");

}