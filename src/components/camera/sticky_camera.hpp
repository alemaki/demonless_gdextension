#ifndef STICKY_CAMERA_HPP
#define STICKY_CAMERA_HPP


#include <godot_cpp/classes/camera3d.hpp>

class StickyCamera : public godot::Camera3D
{
    GDCLASS(StickyCamera, godot::Camera3D);

private:
    godot::Vector3 vector_stick;

public:
    void _ready() override;
    void _physics_process(double delta) override;

protected:
    static void _bind_methods();
};

#endif // STICKY_CAMERA_HPP