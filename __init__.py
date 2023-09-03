import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_MODE,
    CONF_NUMBER,
    CONF_INVERTED,
    CONF_DATA_PIN,
    CONF_CLOCK_PIN,
    CONF_OUTPUT,
)

DEPENDENCIES = []
MULTI_CONF = True

sn74hc4094_ns = cg.esphome_ns.namespace("sn74hc4094")

SN74HC4094Component = sn74hc4094_ns.class_("SN74HC4094Component", cg.Component)
SN74HC4094GPIOPin = sn74hc4094_ns.class_(
    "SN74HC4094GPIOPin", cg.GPIOPin, cg.Parented.template(SN74HC4094Component)
)

CONF_SN74HC4094 = "sn74hc4094"
CONF_STB_PIN = "stb_pin"
CONF_OE_PIN = "oe_pin"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.declare_id(SN74HC4094Component),
        cv.Required(CONF_DATA_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_CLOCK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_STB_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_OE_PIN): pins.gpio_output_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    data_pin = await cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data_pin))
    clock_pin = await cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(clock_pin))
    stb_pin = await cg.gpio_pin_expression(config[CONF_STB_PIN])
    cg.add(var.set_stb_pin(stb_pin))
    if CONF_OE_PIN in config:
        oe_pin = await cg.gpio_pin_expression(config[CONF_OE_PIN])
        cg.add(var.set_oe_pin(oe_pin))


def _validate_output_mode(value):
    if value is not True:
        raise cv.Invalid("Only output mode is supported")
    return value


SN74HC4094_PIN_SCHEMA = cv.All(
    {
        cv.GenerateID(): cv.declare_id(SN74HC4094GPIOPin),
        cv.Required(CONF_SN74HC4094): cv.use_id(SN74HC4094Component),
        cv.Required(CONF_NUMBER): cv.int_range(min=0, max=2048, max_included=False),
        cv.Optional(CONF_MODE, default={}): cv.All(
            {
                cv.Optional(CONF_OUTPUT, default=True): cv.All(
                    cv.boolean, _validate_output_mode
                ),
            },
        ),
        cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    }
)


@pins.PIN_SCHEMA_REGISTRY.register(CONF_SN74HC4094, SN74HC4094_PIN_SCHEMA)
async def sn74hc4094_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_SN74HC4094])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    return var