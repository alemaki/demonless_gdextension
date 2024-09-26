#include "bt_spawn_node3d_at_position.hpp"
#include "utils/utils.hpp"
#include <godot_cpp/classes/scene_tree.hpp>

void BTSpawnNode3DAtPosition::_setup()
{
    this->node3d = utils::load_scene_node<Node3D>(this->path_to_node3d);
}

BTTask::Status BTSpawnNode3DAtPosition::_tick(double delta)
{
    TASK_FAIL_COND(this->node3d == nullptr);
    TASK_COMPLAIN_COND(this->node3d_var == godot::String(""), "node_var not set.");
    Node3D* node3d = godot::Object::cast_to<Node3D>(this->node3d->duplicate());
    ERR_FAIL_COND_V(node3d == nullptr, BTTask::Status::FAILURE);

    if (this->child_of_actor)
    {
        this->get_actor()->add_child(node3d);
    }
    else
    {
       ::get_scene_root()->add_child(node3d);
    }

    if (this->get_blackboard()->has_var(this->var_global_position))
    {
        node3d->set_global_position(this->get_blackboard()->get_var(this->var_global_position));
    }

    if (!(this->node3d_var.is_empty()))
    {
        this->get_blackboard()->set_var(this->node3d_var, this->node3d);
    }

    TASK_SUCCEED();
}

void BTSpawnNode3DAtPosition::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnNode3DAtPosition, STRING, path_to_node3d);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnNode3DAtPosition, STRING_NAME, var_global_position);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnNode3DAtPosition, STRING_NAME, node3d_var);
    BIND_GETTER_SETTER_PROPERTY_BOOL_DEFAULT(BTSpawnNode3DAtPosition, child_of_actor);
}