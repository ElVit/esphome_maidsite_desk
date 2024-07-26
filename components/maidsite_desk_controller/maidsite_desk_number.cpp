#include "maidsite_desk_number.h"
#include "maidsite_desk_controller.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace maidsite_desk_controller
  {
    static const char *const TAG = "maidsite_desk.number";

    void MaidsiteDeskNumber::setup() {}

    void MaidsiteDeskNumber::dump_config()
    {
      LOG_NUMBER("", "MaidsiteDeskNumber", this);
      ESP_LOGCONFIG(TAG, " type %i", type);
    }

    void MaidsiteDeskNumber::control(float value)
    {
      this->publish_state(value);
      parent->number_control(this->type, value);
    }

    void MaidsiteDeskNumber::set_min_value(float value)
    {
      traits.set_min_value(value);
    }

    void MaidsiteDeskNumber::set_max_value(float value)
    {
      traits.set_max_value(value);
    }
  } // namespace maidsite_desk_controller
} // namespace esphome
