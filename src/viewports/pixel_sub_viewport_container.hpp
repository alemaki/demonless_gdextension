#ifndef PIXEL_VIEWPORT_HPP
#define PIXEL_VIEWPORT_HPP

#include <godot_cpp/classes/sub_viewport_container.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "utils/utils.hpp"

class PixelSubViewportContainer : public godot::SubViewportContainer 
{
    GDCLASS(PixelSubViewportContainer, godot::SubViewportContainer);

private:
    godot::SubViewport* sub_viewport = nullptr;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::SubViewport*, sub_viewport);

    void _ready() override;

protected:
    static void _bind_methods();

};

#endif /* PIXEL_VIEWPORT_HPP */