# sn74hc4094
Outputs Expander for ESPHome on 74HC4094

The SN74HC4094 component allows you to use 74HC4094 shift registers as output expanders in ESPHome. It uses 3 wires (optionally 4) for communication.

Once configured, you can use any of the 8 pins for your projects. Up-to 256 shift registers can be daisy-chained to provide more pins, without using more GPIO pins on the controller.

Use of the OE pin is optional. If used, the pin should be pulled up externally.

# Example configuration entry
sn74hc595:<br>
  - id: 'sn74hc4094_hub'<br>
    data_pin: D5<br>
    clock_pin: D8<br>
    stb_pin: D7<br>
    oe_pin: D6<br>

# Individual outputs
switch:
  - platform: gpio<br>
    name: "SN74HC4094 Pin #0"<br>
    pin:<br>
      sn74hc595: sn74hc4094_hub<br>
      #Use pin number 0<br>
      number: 0<br>
      inverted: false<br>

# Configuration variables:
id (Required, ID): The id to use for this SN74HC4094 component<br>
data_pin (Required, Pin Schema): Pin connected to SN74HC4094 SER input<br>
clock_pin (Required, Pin Schema): Pin connected to SN74HC4094 CLK pin<br>
stb_pin (Required, Pin Schema): Pin connected to SN74HC4094 STROBE pin<br>
oe_pin (Optional, Pin Schema): Pin connected to SN74HC4094 OE pin<br>

# Pin configuration variables:<br>
sn74hc4094 (Required, ID): The id of the SN74HC595 component of the pin.<br>
number (Required, int): The pin number.<br>
inverted (Optional, boolean): If all written values should be treated as inverted. Defaults to false.<br>

