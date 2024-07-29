#pragma once

#include <future>
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "maidsite_desk_button.h"
#include "maidsite_desk_number.h"
#include "maidsite_desk_select.h"

#define BUTTON_STEP_UP 0
#define BUTTON_STEP_DOWN 1
#define BUTTON_STOP 2
#define BUTTON_GOTO_MAX 3
#define BUTTON_GOTO_MIN 4
#define BUTTON_GOTO_M1 5
#define BUTTON_GOTO_M2 6
#define BUTTON_GOTO_M3 7
#define BUTTON_GOTO_M4 8
#define BUTTON_SAVE_M1 9
#define BUTTON_SAVE_M2 10
#define BUTTON_SAVE_M3 11
#define BUTTON_SAVE_M4 12

#define NUMBER_HEIGHT_ABS 0
#define NUMBER_HEIGHT_PCT 1

#define SELECT_SET_UNITS 0
#define SELECT_SET_TOUCH_MODE 1

namespace esphome
{
  namespace maidsite_desk_controller
  {
    class MaidsiteDeskController : public PollingComponent, public sensor::Sensor, public uart::UARTDevice
    {
    private:
      float current_height = 0;
      float limit_min = 0;
      float limit_max = 0;
      float physical_min = 0;
      float physical_max = 0;

      float byte2float(int high, int low);
      bool bufferMessage(int data, unsigned int *buffer, int len);
      void handleMessage(unsigned int *message);

    public:
      void update() override;

      void add_button(button::Button *btn, int action);
      void add_number(MaidsiteDeskNumber *number, int type);
      void add_select(MaidsiteDeskSelect *select, int type);
      void number_control(int type, float value);
      void select_control(int type, const std::string &value);

      void set_sensor_height(sensor::Sensor *sensor) { this->sensor_height = sensor; }
      void set_sensor_height_min(sensor::Sensor *sensor) { this->sensor_height_min = sensor; }
      void set_sensor_height_max(sensor::Sensor *sensor) { this->sensor_height_max = sensor; }
      void set_sensor_height_pct(sensor::Sensor *sensor) { this->sensor_height_pct = sensor; }
      void set_sensor_position_m1(sensor::Sensor *sensor) { this->sensor_position_m1 = sensor; }
      void set_sensor_position_m2(sensor::Sensor *sensor) { this->sensor_position_m2 = sensor; }
      void set_sensor_position_m3(sensor::Sensor *sensor) { this->sensor_position_m3 = sensor; }
      void set_sensor_position_m4(sensor::Sensor *sensor) { this->sensor_position_m4 = sensor; }
      void set_sensor_units(sensor::Sensor *sensor) { this->sensor_units = sensor; }
      void set_sensor_touch_mode(sensor::Sensor *sensor) { this->sensor_touch_mode = sensor; }

      void send_simple_command(unsigned char cmd);
      void send_1byte_command(unsigned char cmd, unsigned char byte0);
      void send_2byte_command(unsigned char cmd, unsigned char byte0, unsigned char byte1);

      void step_up();
      void step_down();
      void stop();
      void goto_mem_position(int pos);
      void save_mem_position(int pos);
      void goto_height(float height);
      void goto_max_position();
      void goto_min_position();
      void request_physical_limits();
      void request_limits();
      void request_settings();
      void request_move_to();

      void set_units(const std::string &value);
      void set_touch_mode(const std::string &value);

    protected:
      Sensor *sensor_height{nullptr};
      Sensor *sensor_height_min{nullptr};
      Sensor *sensor_height_max{nullptr};
      Sensor *sensor_height_pct{nullptr};
      Sensor *sensor_position_m1{nullptr};
      Sensor *sensor_position_m2{nullptr};
      Sensor *sensor_position_m3{nullptr};
      Sensor *sensor_position_m4{nullptr};
      Sensor *sensor_units{nullptr};
      Sensor *sensor_touch_mode{nullptr};

      MaidsiteDeskNumber *number_height_abs{nullptr};
      MaidsiteDeskNumber *number_height_pct{nullptr};

      MaidsiteDeskSelect *select_set_units{nullptr};
      MaidsiteDeskSelect *select_set_touch_mode{nullptr};

      void button_press_action(int type);
    };
  } // namespace maidsite_desk_controller
} // namespace esphome
