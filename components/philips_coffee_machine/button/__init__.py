import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID

from .. import CONTROLLER_ID, PhilipsCoffeeMachine, philips_coffee_machine_ns

DEPENDENCIES = ["philips_coffee_machine"]

CONF_ACTION = "action"
CONF_LONG_PRESS = "long_press"

philips_action_button_ns = philips_coffee_machine_ns.namespace("philips_action_button")
ActionButton = philips_action_button_ns.class_(
    "ActionButton", button.Button, cg.Component
)

Action = philips_action_button_ns.enum("ActionButton")
ACTIONS = {
    "SELECT_COFFEE": Action.SELECT_COFFEE,
    "MAKE_COFFEE": Action.MAKE_COFFEE,
    "SELECT_ESPRESSO": Action.SELECT_ESPRESSO,
    "MAKE_ESPRESSO": Action.MAKE_ESPRESSO,
    "SELECT_HOT_WATER": Action.SELECT_HOT_WATER,
    "MAKE_HOT_WATER": Action.MAKE_HOT_WATER,
    "SELECT_STEAM": Action.SELECT_STEAM,
    "MAKE_STEAM": Action.MAKE_STEAM,
    "SELECT_CAPPUCCINO": Action.SELECT_CAPPUCCINO,
    "MAKE_CAPPUCCINO": Action.MAKE_CAPPUCCINO,
    "SELECT_LATTE": Action.SELECT_LATTE,
    "MAKE_LATTE": Action.MAKE_LATTE,
    "SELECT_AMERICANO": Action.SELECT_AMERICANO,
    "MAKE_AMERICANO": Action.MAKE_AMERICANO,
    "BEAN": Action.SELECT_BEAN,
    "SIZE": Action.SELECT_SIZE,
    "MILK": Action.MILK,
    "AQUA_CLEAN": Action.SELECT_AQUA_CLEAN,
    "CALC_CLEAN": Action.SELECT_CALC_CLEAN,
    "PLAY_PAUSE": Action.PLAY_PAUSE,
}


def validate_long_press(config):
    """Validate that long press only applies to select options."""
    if config[CONF_LONG_PRESS] and "MAKE" in config[CONF_ACTION]:
        raise cv.Invalid(f"Action {config[CONF_ACTION]} does not support long press.")

    return config


CONFIG_SCHEMA = cv.All(
    button.BUTTON_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(ActionButton),
            cv.Required(CONTROLLER_ID): cv.use_id(PhilipsCoffeeMachine),
            cv.Required(CONF_ACTION): cv.enum(ACTIONS, upper=True, space="_"),
            cv.Optional(CONF_LONG_PRESS, default=False): cv.boolean,
        },
    ).extend(cv.COMPONENT_SCHEMA),
    validate_long_press,
)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await button.register_button(var, config)

    cg.add(var.set_action(config[CONF_ACTION]))
    cg.add(var.set_long_press(config[CONF_LONG_PRESS]))
    cg.add(parent.add_action_button(var))
