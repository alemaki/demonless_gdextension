#include "pixel_sub_viewport_container.hpp"

void PixelSubViewportContainer::_ready()
{
    if (!this->get_sub_viewport())
    {
        godot::SubViewport* viewport = memnew(godot::SubViewport);
        this->add_child(viewport);
        this->set_sub_viewport(viewport);
    }
    godot::SubViewport* viewport = this->get_sub_viewport();
    this->set_anchor(godot::Side::SIDE_LEFT, 0);
    this->set_anchor(godot::Side::SIDE_RIGHT, 1);
    this->set_anchor(godot::Side::SIDE_TOP, 0);
    this->set_anchor(godot::Side::SIDE_BOTTOM, 1);
    this->set_stretch(true);
    this->set_stretch_shrink(3); // magic, TODO: test some other values
    this->set_texture_filter(godot::CanvasItem::TextureFilter::TEXTURE_FILTER_NEAREST);
}

static void _bind_methods();