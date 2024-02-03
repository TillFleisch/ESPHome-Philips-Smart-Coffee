import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_MODE, CONF_TYPE

from .. import CONTROLLER_ID, PhilipsSeries2200, philips_series_2200_ns
from ..text_sensor import STATUS_SENSOR_ID, StatusSensor

AUTO_LOAD = ["number"]
DEPENDENCIES = ["philips_series_2200"]

CONF_TYPE_SIZE = "size"
CONF_TYPE_BEAN = "bean"
CONF_SOURCE = "source"

philips_beverage_settings_ns = philips_series_2200_ns.namespace(
    "philips_beverage_setting"
)
BeverageSettings = philips_beverage_settings_ns.class_(
    "BeverageSetting", number.Number, cg.Component
)

Source = philips_beverage_settings_ns.enum("Source")
SOURCES = {
    "ANY": Source.ANY,
    "COFFEE": Source.COFFEE,
    "ESPRESSO": Source.ESPRESSO,
    "CAPPUCCINO": Source.CAPPUCCINO,
    "HOT_WATER": Source.HOT_WATER,
}


SUB_SCHEMA = number.NUMBER_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(BeverageSettings),
        cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries2200),
        cv.Required(STATUS_SENSOR_ID): cv.use_id(StatusSensor),
        cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(
            number.NUMBER_MODES, upper=True
        ),
        cv.Optional(CONF_SOURCE, default="ANY"): cv.enum(
            SOURCES, upper=True, space="_"
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


def validate_enum(config):
    """Validate that sources are only used on valid types."""
    if config[CONF_TYPE] == CONF_TYPE_BEAN and config[CONF_SOURCE] == "HOT_WATER":
        raise cv.Invalid("Beverage bean setting does not support HOT_WATER source!")

    return config


CONFIG_SCHEMA = cv.All(
    cv.typed_schema(
        {
            CONF_TYPE_SIZE: SUB_SCHEMA,
            CONF_TYPE_BEAN: SUB_SCHEMA,
        }
    ),
    validate_enum,
)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    status_sensor = await cg.get_variable(config[STATUS_SENSOR_ID])
    var = await number.new_number(config, min_value=1, max_value=3, step=1)
    await cg.register_component(var, config)

    cg.add(var.set_type(config[CONF_TYPE] == CONF_TYPE_BEAN))
    cg.add(var.set_source(config[CONF_SOURCE]))
    cg.add(var.set_status_sensor(status_sensor))
    cg.add(parent.add_beverage_setting(var))
