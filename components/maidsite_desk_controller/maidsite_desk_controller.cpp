#include "maidsite_desk_controller.h"
#include "esphome/core/log.h"

namespace esphome
{
  namespace maidsite_desk_controller
  {
    static const char *const TAG = "maidsite_desk_controller";

    float MaidsiteDeskController::byte2float(int high, int low)
    {
      return static_cast<float>((high << 8) + low) / 10;
    }

    bool MaidsiteDeskController::bufferMessage(int data, unsigned int *buffer, int len)
    {
      // This is a very simple method of receiving messages from the desk.
      // It will fail on messages that contain the value 0x7E in their payload.
      // But it is super simple and works for the messages we care about.

      // Read message:
      // format: 0xF2 0xF2 [command] [param_count] [[param] ...] [checksum] 0x7E
      // checksum: sum of [command], [param_count] and all [param]s

      static int cmd_incoming = 0; // 0: wait for F2, 1: wait for 2nd F2, 2: buffer data
      static int pos = 0;

      if (cmd_incoming < 2 && data == 0xF2)
      { // start of message, must appear twice
        cmd_incoming++;
        pos = 0;
      }
      else if (cmd_incoming == 1)
      { // no second F2 received
        cmd_incoming = 0;
      }
      else if (cmd_incoming == 2)
      {
        if (data == 0x7E)
        { // end of message
          cmd_incoming = 0;
          for (; pos < len - 1; pos++)
          { // fill rest of buffer with zeros
            buffer[pos] = 0;
          }
          return true;
        }
        else if (pos >= len)
        { // message too long, drop it
          cmd_incoming = 0;
        }
        else
        {
          buffer[pos++] = data; // buffer data
        }
      } // else: received garbage

      return false;
    }

    void MaidsiteDeskController::handleMessage(unsigned int *message)
    {
      // ESP_LOGV("maidsite_desk_controller", "message %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", message[0], message[1], message[2], message[3], message[4], message[5], message[6], message[7], message[8], message[9]);

      switch (message[0])
      {
      case 0x01:
        ESP_LOGV("maidsite_desk_controller", "sensor_height 0x%0X%0X", message[2], message[3]);
        float new_height;
        new_height = byte2float(message[2], message[3]);
        if (new_height == current_height)
          return;
        current_height = new_height;
        if (sensor_height != nullptr)
          sensor_height->publish_state(current_height);
        if (number_height_abs != nullptr)
          number_height_abs->publish_state(current_height);

        if (sensor_height_pct != nullptr && limit_max != 0)
          sensor_height_pct->publish_state((current_height - limit_min) / (limit_max - limit_min) * 100);
        if (number_height_pct != nullptr && limit_max != 0)
          number_height_pct->publish_state(roundf((current_height - limit_min) / (limit_max - limit_min) * 1000) / 10);
        break;

      case 0x20:
        ESP_LOGV("maidsite_desk_controller", "sensor_limits 0x%0X  max %i min %i", message[2], (message[2] & 1), (message[2] >> 4));

        if ((message[2] & 1) == 0)
        { // low nibble 0 -> no max limit, use physical_max
          limit_max = physical_max;
          if (sensor_height_max != nullptr)
            sensor_height_max->publish_state(limit_max);
          if (number_height_abs != nullptr)
            number_height_abs->set_max_value(limit_max);
        }
        if ((message[2] >> 4) == 0)
        { // high nibble 0 -> no min limit, use physical_min
          limit_min = physical_min;
          if (sensor_height_min != nullptr)
            sensor_height_min->publish_state(limit_min);
          if (number_height_abs != nullptr)
            number_height_abs->set_min_value(limit_min);
        }
        break;

      case 0x07:
        ESP_LOGV("maidsite_desk_controller", "sensor_physical_limits 0x%02X%02X 0x%02X%02X", message[2], message[3], message[4], message[5]);
        physical_max = byte2float(message[2], message[3]);
        physical_min = byte2float(message[4], message[5]);
        break;

      case 0x21:
        ESP_LOGV("maidsite_desk_controller", "sensor_height_max 0x%02X%02X", message[2], message[3]);
        limit_max = byte2float(message[2], message[3]);
        if (sensor_height_max != nullptr)
          sensor_height_max->publish_state(limit_max);
        if (number_height_abs != nullptr)
          number_height_abs->set_max_value(limit_max);
        break;

      case 0x22:
        ESP_LOGV("maidsite_desk_controller", "sensor_height_min 0x%02X%02X", message[2], message[3]);
        limit_min = byte2float(message[2], message[3]);
        if (sensor_height_min != nullptr)
          sensor_height_min->publish_state(limit_min);
        if (number_height_abs != nullptr)
          number_height_abs->set_min_value(limit_min);
        break;

      case 0x25:
        ESP_LOGV("maidsite_desk_controller", "sensor_position_m1 0x%02X%02X", message[2], message[3]);
        if (sensor_position_m1 != nullptr)
          sensor_position_m1->publish_state(byte2float(message[2], message[3]));
        break;

      case 0x26:
        ESP_LOGV("maidsite_desk_controller", "sensor_position_m2 0x%02X%02X", message[2], message[3]);
        if (sensor_position_m2 != nullptr)
          sensor_position_m2->publish_state(byte2float(message[2], message[3]));
        break;

      case 0x27:
        ESP_LOGV("maidsite_desk_controller", "sensor_position_m3 0x%02X%02X", message[2], message[3]);
        if (sensor_position_m3 != nullptr)
          sensor_position_m3->publish_state(byte2float(message[2], message[3]));
        break;

      case 0x28:
        ESP_LOGV("maidsite_desk_controller", "sensor_position_m4 0x%02X%02X", message[2], message[3]);
        if (sensor_position_m4 != nullptr)
          sensor_position_m4->publish_state(byte2float(message[2], message[3]));
        break;

      case 0x0E:
        // ToDo: Make this to TextSensor
        ESP_LOGV("maidsite_desk_controller", "sensor_units 0x%02X", message[2]);
        if (sensor_units != nullptr)
          sensor_units->publish_state(message[2]);
        break;

      case 0x19:
        // ToDo: Make this to TextSensor
        ESP_LOGV("maidsite_desk_controller", "sensor_touch_mode 0x%02X", message[2]);
        if (sensor_touch_mode != nullptr)
          sensor_touch_mode->publish_state(message[2]);
        break;

      default:
        ESP_LOGV("maidsite_desk_controller", "unknown message %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", message[0], message[1], message[2], message[3], message[4], message[5], message[6], message[7], message[8], message[9]);
      }
    }

    void MaidsiteDeskController::update()
    {
      const int max_length = 10;
      static unsigned int buffer[max_length];
      while (available())
      {
        if (bufferMessage(read(), buffer, max_length))
        {
          handleMessage(buffer);
        }
      }
    }

    // Write message:
    // format: 0xF1 0xF1 [command] [param_count] [[param] ...] [checksum] 0x7E
    // checksum: sum of [command], [param_count] and all [param]s
    void MaidsiteDeskController::send_simple_command(unsigned char cmd)
    {
      write_array({0xF1, 0xF1, cmd, 0x00, cmd, 0x7E});
    }

    void MaidsiteDeskController::send_1byte_command(unsigned char cmd, unsigned char byte0)
    {
      unsigned char checksum = cmd + 1 + byte0;
      write_array({0xF1, 0xF1, cmd, 0x01, byte0, checksum, 0x7E});
    }

    void MaidsiteDeskController::send_2byte_command(unsigned char cmd, unsigned char byte0, unsigned char byte1)
    {
      unsigned char checksum = cmd + 2 + byte0 + byte1;
      write_array({ 0xF1, 0xF1, cmd, 0x02, byte0, byte1, checksum, 0x7E });
    }

    void MaidsiteDeskController::step_up()
    {
      send_simple_command(0x01);
    }

    void MaidsiteDeskController::step_down()
    {
      send_simple_command(0x02);
    }

    void MaidsiteDeskController::stop()
    {
      send_simple_command(0x2B);
    }

    void MaidsiteDeskController::goto_mem_position(int pos)
    {
      switch (pos)
      {
      case 1:
        send_simple_command(0x05);
        break;
      case 2:
        send_simple_command(0x06);
        break;
      case 3:
        send_simple_command(0x27);
        break;
      case 4:
        send_simple_command(0x28);
        break;
      }
    }

    void MaidsiteDeskController::save_mem_position(int pos)
    {
      switch (pos)
      {
      case 1:
        send_simple_command(0x03);
        break;
      case 2:
        send_simple_command(0x04);
        break;
      case 3:
        send_simple_command(0x25);
        break;
      case 4:
        send_simple_command(0x26);
        break;
      }
    }

    void MaidsiteDeskController::request_physical_limits()
    {
      send_simple_command(0x0C);
    }

    void MaidsiteDeskController::request_limits()
    {
      send_simple_command(0x20);
    }

    void MaidsiteDeskController::request_settings()
    {
      send_simple_command(0x07);
    }

    void MaidsiteDeskController::request_move_to()
    {
      send_simple_command(0x1B);
    }

    void MaidsiteDeskController::goto_height(float height)
    {
      unsigned char high_byte = ((int)height * 10) >> 8;
      unsigned char low_byte = ((int)height * 10) & 0xFF;

      send_2byte_command(0x80, high_byte, low_byte);
      request_move_to();
    }

    void MaidsiteDeskController::goto_max_position()
    {
      goto_height(limit_max);
    }

    void MaidsiteDeskController::goto_min_position()
    {
      goto_height(limit_min);
    }

    void MaidsiteDeskController::set_units(const std::string &value)
    {
      unsigned char payload = 0;
      if (value == "mm")
        payload = 0;
      else if (value == "1/10 inch")
        payload = 1;
      else
        return;
      send_1byte_command(0x0E, payload);
    }

    void MaidsiteDeskController::set_touch_mode(const std::string &value)
    {
      unsigned char payload = 0;
      if (value == "single")
        payload = 0;
      else if (value == "continuous")
        payload = 1;
      else
        return;
      send_1byte_command(0x19, payload);
    }

    void MaidsiteDeskController::add_button(button::Button *btn, int action)
    {
      btn->add_on_press_callback([this, action]() { this->button_press_action(action); });
    }

    void MaidsiteDeskController::button_press_action(int action)
    {
      ESP_LOGV("MaidsiteDeskController", "button_press_action %i", action);
      switch (action)
      {
      case BUTTON_STEP_UP:
        step_up();
        break;
      case BUTTON_STEP_DOWN:
        step_down();
        break;
      case BUTTON_STOP:
        stop();
        break;
      case BUTTON_GOTO_MAX:
        goto_max_position();
        break;
      case BUTTON_GOTO_MIN:
        goto_min_position();
        break;
      case BUTTON_GOTO_M1:
        goto_mem_position(1);
        break;
      case BUTTON_GOTO_M2:
        goto_mem_position(2);
        break;
      case BUTTON_GOTO_M3:
        goto_mem_position(3);
        break;
      case BUTTON_GOTO_M4:
        goto_mem_position(4);
        break;
      case BUTTON_SAVE_M1:
        save_mem_position(1);
        delay(100);
        request_settings();
        break;
      case BUTTON_SAVE_M2:
        save_mem_position(2);
        delay(100);
        request_settings();
        break;
      case BUTTON_SAVE_M3:
        save_mem_position(3);
        delay(100);
        request_settings();
        break;
      case BUTTON_SAVE_M4:
        save_mem_position(4);
        delay(100);
        request_settings();
        break;
      }
    }

    void MaidsiteDeskController::add_number(MaidsiteDeskNumber *number, int type)
    {
      switch (type)
      {
      case NUMBER_HEIGHT_ABS:
        number_height_abs = number;
        break;
      case NUMBER_HEIGHT_PCT:
        number_height_pct = number;
        number_height_pct->set_min_value(0);
        number_height_pct->set_max_value(100);
        break;
      default:
        return;
      }
      number->set_type(type);
      number->set_parent(this);
    }

    void MaidsiteDeskController::number_control(int type, float value)
    {
      ESP_LOGV("MaidsiteDeskController", "number_control %i", type);
      switch (type)
      {
      case NUMBER_HEIGHT_ABS:
        goto_height(value);
        break;
      case NUMBER_HEIGHT_PCT:
        if (limit_max > 0 && limit_max > limit_min)
          goto_height((limit_max - limit_min) * value / 100 + limit_min);
        break;
      }
    }

    void MaidsiteDeskController::add_select(MaidsiteDeskSelect *select, int type)
    {
      switch (type)
      {
      case NUMBER_SET_UNITS:
        select_set_units = select;
        select_set_units->set_options({"mm", "1/10 inch"});
        break;
      case NUMBER_SET_TOUCH_MODE:
        select_set_touch_mode = select;
        select_set_touch_mode->set_options({"single", "continuous"});
        break;
      default:
        return;
      }
      number->set_type(type);
      number->set_parent(this);
    }

    void MaidsiteDeskController::select_control(int type, const std::string &value)
    {
      ESP_LOGV("MaidsiteDeskController", "select_control %i", type);
      switch (type)
      {
      case SELECT_SET_UNITS:
        set_units(value);
        break;
      case SELECT_SET_TOUCH_MODE:
        set_touch_mode(value);
        break;
      }
    }
  } // namespace maidsite_desk_controller
} // namespace esphome
