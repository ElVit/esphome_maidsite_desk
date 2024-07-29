#include "maidsite_desk_select.h"
#include "maidsite_desk_controller.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace maidsite_desk_controller
  {
    static const char *const TAG = "maidsite_desk.select";

    void MaidsiteDeskSelect::setup() {}

    void MaidsiteDeskSelect::dump_config()
    {
      LOG_SELECT("", "MaidsiteDeskSelect", this);
      ESP_LOGCONFIG(TAG, " type %i", type);
    }

    void MaidsiteDeskSelect::control(const std::string &value)
    {
      this->publish_state(value);
      parent->select_control(this->type, value);
      //auto index = this->index_of(value);
      //if (index.has_value())
      //{
      //  this->parent->select_control(this->type, index.value());
      //}
    }

    void MaidsiteDeskSelect::set_options(std::vector<std::string> options)
    {
      traits.set_options(options);
    }
  } // namespace maidsite_desk_controller
} // namespace esphome
