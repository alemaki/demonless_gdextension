#include "sticky_camera.hpp"

void StickyCamera::_ready()
{
    DISABLE_EDITOR_PROCESSING();
    /* move on its own*/
    this->set_as_top_level(true);
    this->vector_stick = this->get_position();
}

void StickyCamera::_physics_process(double delta)
{
    ERR_FAIL_NULL(this->get_parent());
    godot::Node3D* parent = godot::Object::cast_to<Node3D>(this->get_parent());
    ERR_FAIL_NULL_MSG(parent, vformat("%s: Must be attached to Node3D", this->get_name()));

    godot::Vector3 position = parent->get_position();
    position.x += this->vector_stick.x;
    position.z += this->vector_stick.z;
    position.y = this->vector_stick.y;

    this->set_position(position);
}

void StickyCamera::_bind_methods()
{
    using namespace godot;
}
