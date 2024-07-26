#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace maidsite_desk_controller
  {
    class MaidsiteDeskButton : public Component, public button::Button
    {
    protected:
      void press_action() override;
    };
  } // namespace maidsite_desk_controller
} // namespace esphome
