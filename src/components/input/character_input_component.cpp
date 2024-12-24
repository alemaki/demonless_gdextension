#include "character_input_component.hpp"

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

godot::Vector3 CharacterInputComponent::get_mouse_casted_position()
{
    godot::Ref<godot::World3D> world = this->get_world_3d();
    godot::Vector2 mouse_pos = this->get_viewport()->get_mouse_position();
    godot::Camera3D* camera = this->get_tree()->get_root()->get_camera_3d();
    ERR_FAIL_COND_V(camera == nullptr, godot::Vector3(0, 0, 0));

    godot::Vector3 ray_origin = camera->project_ray_origin(mouse_pos);
    godot::Vector3 ray_direction  = camera->project_ray_normal(mouse_pos);
    ERR_FAIL_COND_V(godot::Math::abs(ray_direction.y) <= 1e-6, godot::Vector3(0, 0, 0));

    //TODO: float for better performance?
    double t = - ray_origin.y / ray_direction.y;
    godot::Vector3 ray_end = ray_origin + ray_direction * t;
    
    return ray_end;
}

void CharacterInputComponent::_bind_methods()
{
    using namespace godot;
    
    ClassDB::bind_method(D_METHOD("get_direction_input"),&CharacterInputComponent::get_direction_input);
    ClassDB::bind_method(D_METHOD("is_attack_pressed"),&CharacterInputComponent::is_attack_pressed);
    ClassDB::bind_method(D_METHOD("is_block_pressed"),&CharacterInputComponent::is_block_pressed);
}