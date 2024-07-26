#pragma once

#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace maidsite_desk_controller
  {
    class MaidsiteDeskController;

    class MaidsiteDeskSelect : public select::Select, public Component
    {
    private:
      int type;
      MaidsiteDeskController *parent;

    public:
      void setup() override;
      void dump_config() override;

      void set_parent(MaidsiteDeskController *parent) { this->parent = parent; }
      void set_type(int type) { this->type = type; }

      void set_options(std::vector<std::string> options);

    protected:
      void control(const std::string &value) override;
    };
  } // namespace maidsite_desk_controller
} // namespace esphome
