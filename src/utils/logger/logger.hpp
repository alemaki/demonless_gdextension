#ifndef SRC_LOGGER_HPP
#define SRC_LOGGER_HPP

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/templates/hash_set.hpp>

#include "behaviour_tree/utils/macros.hpp"
#include "behaviour_tree/utils/utils.hpp"

namespace utils
{
    class Logger
    {
    private:
        Logger(){};
        Logger(const Logger&) = delete;

    public:
        static Logger* instance;

        enum Level
        {
            CRIT_ERR = 0, /* CRITICALS and ERRORS are handled in place with godot's CRASH_ERR and ERR_PTINT- always enabled */
            INFO = 1,
            DEBG = 2,
        };

        int enabled  = CRIT_ERR | INFO | DEBG;
        bool every_class_enabled = true;

        godot::HashSet<godot::StringName> enabled_classes = {};
        godot::HashSet<godot::StringName> disabled_classes = {};
        godot::HashSet<godot::StringName> enabled_base_classes = {};
        godot::HashSet<godot::StringName> disabled_base_classes = {};


        CREATE_GETTER_SETTER_BOOL_DEFAULT(every_class_enabled);

        bool is_class_enabled(const godot::StringName& name) const;

        void disable_class(const godot::StringName& name);
        void enable_class(const godot::StringName& name);
        void disable_base_class(const godot::StringName& name);
        void enable_base_class(const godot::StringName& name);

        void enable_level(Level level);
        void disable_level(Level level);
        void enable_all_levels();

        inline void log_info(godot::StringName class_name, godot::String message)
        {
            if (((this->enabled & Level::INFO) == 0) || (!this->is_class_enabled(class_name))) 
            {
                return;
            }
            godot::UtilityFunctions::print("[INFO]", "[", class_name, "]", message);
        }

        inline void log_debug(godot::StringName class_name, godot::String message)
        {
            if ((this->enabled & Level::DEBG) == 0 || (!this->is_class_enabled(class_name))) 
            {
                return;
            }
            godot::UtilityFunctions::print("[DEBG]", "[", class_name, "]", message);
        }
    };

    inline Logger* Logger::instance = new Logger();

} // namespace utils

#define LOG_DEBUG_AS(tag, message) utils::Logger::instance->log_debug(tag, message)
#define LOG_DEBUG(message) utils::Logger::instance->log_debug(this->get_class(), message)
#define LOG_INFO_AS(tag, message) utils::Logger::instance->log_info(tag, message)
#define LOG_INFO(message) utils::Logger::instance->log_info(this->get_class(), message)

#endif /* SRC_LOGGER_HPP */
