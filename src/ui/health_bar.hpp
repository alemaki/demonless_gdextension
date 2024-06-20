#ifndef HEALTH_BAR_HPP
#define HEALTH_BAR_HPP

#include <godot_cpp/classes/progress_bar.hpp>

#include "components/health/health_component.hpp"

class HealthBar : public godot::ProgressBar 
{
    GDCLASS(HealthBar, godot::ProgressBar);

private:
    HealthComponent* health_component;

protected:
    void _on_change();

public:
    HealthBar();

protected:
    static void _bind_methods();

};

#endif /* HEALTH_BAR_HPP */