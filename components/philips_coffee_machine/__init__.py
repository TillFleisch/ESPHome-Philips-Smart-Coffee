import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components.uart import UARTComponent
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

DISPLAY_UART_ID = "display_uart"
MAINBOARD_UART_ID = "mainboard_uart"
POWER_PIN = "power_pin"
CONTROLLER_ID = "controller_id"
INVERT_POWER_PIN = "invert_power_pin"
POWER_TRIP_DELAY = "power_trip_delay"
CONF_POWER_MESSAGE_REPETITIONS = "power_message_repetitions"

CONF_COMMAND_SET = "model"
COMMAND_SETS = {
    "EP_2220": "PHILIPS_EP2220",
    "EP_2235": "PHILIPS_EP2235",
    "EP_3243": "PHILIPS_EP3243",
    # Note that the EP3243 and EP3246 are identical except for cosmetic differences
    "EP_3246": "PHILIPS_EP3243",
}

CONF_LANGUAGE = "language"
# Using IETF BCP 47 language tags (RFC 5646)
LANGUAGES = {
    "en-US": "PHILIPS_COFFEE_LANG_en_US",
    "de-DE": "PHILIPS_COFFEE_LANG_de_DE",
    "it-IT": "PHILIPS_COFFEE_LANG_it_IT",
}

philips_coffee_machine_ns = cg.esphome_ns.namespace("philips_coffee_machine")
PhilipsCoffeeMachine = philips_coffee_machine_ns.class_(
    "PhilipsCoffeeMachine", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PhilipsCoffeeMachine),
        cv.Required(DISPLAY_UART_ID): cv.use_id(UARTComponent),
        cv.Required(MAINBOARD_UART_ID): cv.use_id(UARTComponent),
        cv.Required(POWER_PIN): pins.gpio_output_pin_schema,
        cv.Optional(INVERT_POWER_PIN, default=False): cv.boolean,
        cv.Optional(POWER_TRIP_DELAY, default="500ms"): cv.All(
            cv.positive_time_period_milliseconds,
            cv.Range(
                min=cv.TimePeriod(milliseconds=0),
                max_included=cv.TimePeriod(milliseconds=10000),
            ),
        ),
        cv.Optional(CONF_POWER_MESSAGE_REPETITIONS, default=5): cv.positive_int,
        cv.Optional(CONF_COMMAND_SET, default="EP_2220"): cv.enum(
            COMMAND_SETS, upper=True, space="_"
        ),
        cv.Optional(CONF_LANGUAGE, default="en-US"): cv.enum(LANGUAGES, space="-"),
    }
).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    # Use user-specified command set, default to EP_2200
    cg.add_define(COMMAND_SETS[config[CONF_COMMAND_SET]])
    cg.add_define(LANGUAGES[config[CONF_LANGUAGE]])

    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    display = yield cg.get_variable(config[DISPLAY_UART_ID])
    mainboard = yield cg.get_variable(config[MAINBOARD_UART_ID])
    pin = yield cg.gpio_pin_expression(config[POWER_PIN])

    cg.add(var.register_display_uart(display))
    cg.add(var.register_mainboard_uart(mainboard))
    cg.add(var.set_power_pin(pin))
    cg.add(var.set_power_message_repetitions(config[CONF_POWER_MESSAGE_REPETITIONS]))
    cg.add(var.set_invert_power_pin(config[INVERT_POWER_PIN]))
    cg.add(var.set_power_trip_delay(config[POWER_TRIP_DELAY]))
