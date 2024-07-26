# ESPHome Maidsite Desk Controller

[ESPHome](https://esphome.io/) component for controlling Maidsite desk controllers via their serial protocol.

## Usage

### What you need

* ESPHome compatible microcontroller
* cable with RJ12 connector (phone cable, RJ11 may also work)

### Wiring

#### RJ12

Please double check this for your specific model!

pin | function
----|---------
 1  | NC (pulled up)
 2  | GND
 3  | TX
 4  | VCC
 5  | RX
 6  | NC (pulled up)

#### microcontroller

ESP    | desk
-------|-----
GND    | GND
5V     | VCC
GPIO3  | TX
GPIO4  | RX

![IMG_1256](https://github.com/user-attachments/assets/229f52bb-5cdd-454c-b72b-e84979ee5976)

Sometime the Desk Controller does not supply enough current, then simply disconnet the VCC pin and plug in a micro-usb/usb-c cable into your esp-board.  

## Usage

```yaml
substitutions:
  pin_tx: GPIO3
  pin_rx: GPIO4

esphome:
  ...
  on_boot:
    # This script is required to initialize the following sensors:
    #    height_abs, height_pct, height_min, height_max, position_m1 - position_m4
    # You can skip this if you don't use those.
    priority: 0 # when mostly everything else is done
    then:
      - lambda: "id(my_desk).request_physical_limits();"
      - delay: 0.1s
      - lambda: "id(my_desk).request_limits();"
      - delay: 0.1s
      - lambda: "id(my_desk).request_settings();"

external_components:
  - source:
      type: git
      url: https://github.com/ElVit/esphome_maidsite_desk/
    components: [ maidsite_desk_controller ]

uart:
  - id: uart_bus
    tx_pin: $pin_tx
    rx_pin: $pin_rx
    baud_rate: 9600

maidsite_desk_controller:
  id: my_desk

  sensors:
    height_abs:
      name: "Height"

  numbers:
    height_abs:
      name: "Height"
      mode: SLIDER

  buttons:
    stop:
      name: "Stop"
    step_up:
      name: "Step up"
    step_down:
      name: "Step down"
    goto_m1:
      name: "Move to M1"
    goto_m2:
      name: "Move to M2"
    goto_m3:
      name: "Move to M3"
    goto_m4:
      name: "Move to M4"
```

### Examples

* basic, sufficient for most people: [example_basic.yaml](example_basic.yaml)
* full with all features in use: [example_full.yaml](example_full.yaml)

### Features

#### sensors

sensor       | description
-------------|----------------------------
height_abs   | current height of the desk
height_pct   | height in percent
height_min   | minimal height
height_max   | maximal height
position_m1  | 1st stored height
position_m2  | 2nd stored height
position_m3  | 3rd stored height
position_m4  | 4th stored height

####  number entities

Hold current values read from the desk and set values to desk when changed.

sensor      | description
------------|----------------------------
height_abs  | current height of the desk
height_pct  | height in percent

####  buttons

button     | description
-----------|---------------------------
step_up    | move up desk by one step (~14mm)
step_down  | move down desk by one step (~14mm)
goto_max   | move up until stopped
goto_min   | move down until stopped
stop       | stop movement of desk
goto_m1    | move to 1st stored height
goto_m2    | move to 2nd stored height
goto_m3    | move to 3rd stored height
goto_m4    | move to 4th stored height
save_m1    | save current height to 1th position
save_m2    | save current height to 2th position
save_m3    | save current height to 3th position
save_m4    | save current height to 4th position

#### lambda methods

lambda method                         | description
--------------------------------------|---------------------------
`id(my_desk).step_up()`               | step_up desk by one step (~14mm)
`id(my_desk).step_down()`             | step_down desk by one step (~14mm)
`id(my_desk).goto_max_position()`     | move up
`id(my_desk).goto_min_position()`     | move down
`id(my_desk).stop()`                  | stop movement of desk
`id(my_desk).goto_mem_position(pos)`  | move to stored height `pos`
`id(my_desk).save_mem_position(pos)`  | save current height to position `pos`
`id(my_desk).goto_height(height)`     | move to specified height

## Dev Notes

[Jarvis reverse engineering notes](https://docs.google.com/spreadsheets/d/1GKZfDFljVX4eQBMawq0-Rc8t0x8V6gjQ5BgAYngPYTo/edit?pli=1&gid=1438530487#gid=1438530487)

## Sources

Thanks to [shades66](https://github.com/shades66/Maidesite-standing-desk/tree/main) for the minimal and easy to setup esphome config.  
Thanks to [Rocka84](https://github.com/Rocka84/esphome_components) for doing the main work, I've just forked this repo and modified some parts.  
Thanks to [phord/Jarvis](https://github.com/phord/Jarvis) for reverse engineering the UART interface and most control messages!  
Thanks to [OkhammahkO](https://github.com/OkhammahkO) for collecting the scattered information at [pimp-my-desk/desk-control](https://gitlab.com/pimp-my-desk/desk-control) and for his work in the home assistant community!

