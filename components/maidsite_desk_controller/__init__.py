import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart, sensor, button, number, select
from esphome.const import (
  CONF_ID,
  CONF_UNIT_OF_MEASUREMENT,
  CONF_STEP,
  CONF_ACCURACY_DECIMALS,
  CONF_ENTITY_CATEGORY,
  UNIT_CENTIMETER,
  UNIT_PERCENT,
  ENTITY_CATEGORY_CONFIG,
  CONF_INITIAL_OPTION,
  CONF_OPTIONS,
)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor', 'button', 'number', 'select']

maidsite_desk_controller_ns = cg.esphome_ns.namespace('maidsite_desk_controller')

MaidsiteDeskController = maidsite_desk_controller_ns.class_('MaidsiteDeskController', cg.Component, uart.UARTDevice)
MaidsiteDeskButton = maidsite_desk_controller_ns.class_('MaidsiteDeskButton', button.Button, cg.Component)
MaidsiteDeskNumber = maidsite_desk_controller_ns.class_('MaidsiteDeskNumber', number.Number, cg.Component)
MaidsiteDeskSelect = maidsite_desk_controller_ns.class_('MaidsiteDeskSelect', select.Select, cg.Component)


CONF_SENSORS = "sensors"
CONF_BUTTONS = "buttons"
CONF_NUMBERS = "numbers"
CONF_SELECTS = "selects"

CONF_HEIGHT_MIN = "height_min"
CONF_HEIGHT_MAX = "height_max"
CONF_HEIGHT_ABS = "height_abs"
CONF_HEIGHT_PCT = "height_pct"
CONF_POSITION_M1 = "position_m1"
CONF_POSITION_M2 = "position_m2"
CONF_POSITION_M3 = "position_m3"
CONF_POSITION_M4 = "position_m4"
CONF_UNITS = "units"
CONF_TOUCH_MODE = "touch_mode"

CONF_STEP_UP = "step_up"
CONF_STEP_DOWN = "step_down"
CONF_STOP = "stop"
CONF_GOTO_MAX = "goto_max"
CONF_GOTO_MIN = "goto_min"
CONF_GOTO_M1 = "goto_m1"
CONF_GOTO_M2 = "goto_m2"
CONF_GOTO_M3 = "goto_m3"
CONF_GOTO_M4 = "goto_m4"
CONF_SAVE_M1 = "save_m1"
CONF_SAVE_M2 = "save_m2"
CONF_SAVE_M3 = "save_m3"
CONF_SAVE_M4 = "save_m4"

CONF_SET_UNITS = "set_units"
CONF_SET_TOUCH_MODE = "set_touch_mode"

button_constants = {}
button_constants[CONF_STEP_UP] = 0
button_constants[CONF_STEP_DOWN] = 1
button_constants[CONF_STOP] = 2
button_constants[CONF_GOTO_MAX] = 3
button_constants[CONF_GOTO_MIN] = 4
button_constants[CONF_GOTO_M1] = 5
button_constants[CONF_GOTO_M2] = 6
button_constants[CONF_GOTO_M3] = 7
button_constants[CONF_GOTO_M4] = 8
button_constants[CONF_SAVE_M1] = 9
button_constants[CONF_SAVE_M2] = 10
button_constants[CONF_SAVE_M3] = 11
button_constants[CONF_SAVE_M4] = 12

number_constants = {}
number_constants[CONF_HEIGHT_ABS] = 0
number_constants[CONF_HEIGHT_PCT] = 1

select_constants = {}
select_constants[CONF_SET_UNITS] = 0
select_constants[CONF_SET_TOUCH_MODE] = 1

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
  cv.GenerateID(): cv.declare_id(MaidsiteDeskController),

  cv.Optional(CONF_SENSORS): cv.Schema({
    cv.Optional(CONF_HEIGHT_ABS): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_HEIGHT_PCT): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_PERCENT),
    cv.Optional(CONF_HEIGHT_MIN): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_HEIGHT_MAX): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_POSITION_M1): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_POSITION_M2): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_POSITION_M3): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_POSITION_M4): sensor.sensor_schema(accuracy_decimals = 1, unit_of_measurement = UNIT_CENTIMETER),
    cv.Optional(CONF_UNITS): sensor.sensor_schema(accuracy_decimals = 0),
    cv.Optional(CONF_TOUCH_MODE): sensor.sensor_schema(accuracy_decimals = 0),
  }),
  cv.Optional(CONF_NUMBERS): cv.Schema({
    cv.Optional(CONF_HEIGHT_ABS): number.NUMBER_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskNumber),
      cv.Optional(CONF_ACCURACY_DECIMALS, default=1): cv.int_,
      cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_CENTIMETER): cv.string,
      cv.Optional(CONF_STEP, default=1): cv.float_,
    }),
    cv.Optional(CONF_HEIGHT_PCT): number.NUMBER_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskNumber),
      cv.Optional(CONF_ACCURACY_DECIMALS, default=1): cv.int_,
      cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_PERCENT): cv.string,
      cv.Optional(CONF_STEP, default=.1): cv.float_,
    }),
  }),
  cv.Optional(CONF_BUTTONS): cv.Schema({
    cv.Optional(CONF_STEP_UP): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_STEP_DOWN): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_STOP): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_GOTO_MAX): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_GOTO_MIN): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_GOTO_M1): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_GOTO_M2): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_GOTO_M3): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_GOTO_M4): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
    }),
    cv.Optional(CONF_SAVE_M1): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
      cv.Optional(CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG): cv.entity_category,
    }),
    cv.Optional(CONF_SAVE_M2): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
      cv.Optional(CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG): cv.entity_category,
    }),
    cv.Optional(CONF_SAVE_M3): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
      cv.Optional(CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG): cv.entity_category,
    }),
    cv.Optional(CONF_SAVE_M4): button.BUTTON_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskButton),
      cv.Optional(CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG): cv.entity_category,
    }),
  }),
  cv.Optional(CONF_SELECTS): cv.Schema({
    cv.Optional(CONF_SET_UNITS): select.SELECT_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskSelect),
      cv.Required(CONF_OPTIONS): cv.All(
        cv.ensure_list(cv.string_strict), cv.Length(min=1)
      ),
      cv.Optional(CONF_INITIAL_OPTION): cv.string_strict,
    }),
    cv.Optional(CONF_SET_TOUCH_MODE): select.SELECT_SCHEMA.extend({
      cv.GenerateID(): cv.declare_id(MaidsiteDeskSelect),
      cv.Required(CONF_OPTIONS): cv.All(
        cv.ensure_list(cv.string_strict), cv.Length(min=1)
      ),
      cv.Optional(CONF_INITIAL_OPTION): cv.string_strict,
    }),
  }),
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])
  await cg.register_component(var, config)
  await uart.register_uart_device(var, config)

  if CONF_SENSORS in config:
    sensors = config[CONF_SENSORS]
    if CONF_HEIGHT_MIN in sensors:
      sens = await sensor.new_sensor(sensors[CONF_HEIGHT_MIN])
      cg.add(var.set_sensor_height_min(sens))
    if CONF_HEIGHT_MAX in sensors:
      sens = await sensor.new_sensor(sensors[CONF_HEIGHT_MAX])
      cg.add(var.set_sensor_height_max(sens))
    if CONF_HEIGHT_ABS in sensors:
      sens = await sensor.new_sensor(sensors[CONF_HEIGHT_ABS])
      cg.add(var.set_sensor_height(sens))
    if CONF_HEIGHT_PCT in sensors:
      sens = await sensor.new_sensor(sensors[CONF_HEIGHT_PCT])
      cg.add(var.set_sensor_height_pct(sens))
    if CONF_POSITION_M1 in sensors:
      sens = await sensor.new_sensor(sensors[CONF_POSITION_M1])
      cg.add(var.set_sensor_position_m1(sens))
    if CONF_POSITION_M2 in sensors:
      sens = await sensor.new_sensor(sensors[CONF_POSITION_M2])
      cg.add(var.set_sensor_position_m2(sens))
    if CONF_POSITION_M3 in sensors:
      sens = await sensor.new_sensor(sensors[CONF_POSITION_M3])
      cg.add(var.set_sensor_position_m3(sens))
    if CONF_POSITION_M4 in sensors:
      sens = await sensor.new_sensor(sensors[CONF_POSITION_M4])
      cg.add(var.set_sensor_position_m4(sens))
    if CONF_UNITS in sensors:
      sens = await sensor.new_sensor(sensors[CONF_UNITS])
      cg.add(var.set_sensor_units(sens))
    if CONF_TOUCH_MODE in sensors:
      sens = await sensor.new_sensor(sensors[CONF_TOUCH_MODE])
      cg.add(var.set_sensor_touch_mode(sens))

  if CONF_BUTTONS in config:
    buttons = config[CONF_BUTTONS]
    for button_type in buttons.keys():
      btn = await button.new_button(buttons[button_type])
      cg.add(var.add_button(btn, button_constants[button_type]))

  if CONF_NUMBERS in config:
    numbers = config[CONF_NUMBERS]
    for number_type in numbers.keys():
      num = await number.new_number(numbers[number_type], min_value=0, max_value=100, step=.1)
      cg.add(var.add_number(num, number_constants[number_type]))

  if CONF_SELECTS in config:
    selects = config[CONF_SELECTS]
    for select_type in selects.keys():
      sel = await select.new_select(selects[select_type])
      cg.add(var.add_select(sel, select_constants[select_type]))
    # if CONF_SET_UNITS in selects:
    #   sel = await select.new_select(selects[CONF_SET_UNITS], options=["0.2m", "0.75m"])
    #   cg.add(var.add_select(sel, select_constants[CONF_SET_UNITS]))
    # if CONF_SET_TOUCH_MODE in selects:
    #   sel = await select.new_select(selects[CONF_SET_TOUCH_MODE], options=["0.2m", "0.75m"])
    #   cg.add(var.add_select(sel, select_constants[CONF_SET_TOUCH_MODE]))
