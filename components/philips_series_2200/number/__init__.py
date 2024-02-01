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

philips_size_settings_ns = philips_series_2200_ns.namespace("philips_size_settings")
SizeSettings = philips_size_settings_ns.class_(
    "SizeSettings", number.Number, cg.Component
)

philips_bean_settings_ns = philips_series_2200_ns.namespace("philips_bean_settings")
BeanSettings = philips_bean_settings_ns.class_(
    "BeanSettings", number.Number, cg.Component
)

SizeSource = philips_size_settings_ns.enum("Source")
SIZE_SOURCES = {
    "COFFEE": SizeSource.COFFEE,
    "ESPRESSO": SizeSource.ESPRESSO,
    "CAPPUCCINO": SizeSource.CAPPUCCINO,
    "HOT_WATER": SizeSource.HOT_WATER,
}

BeanSource = philips_bean_settings_ns.enum("Source")
BEAN_SOURCES = {
    "COFFEE": BeanSource.COFFEE,
    "ESPRESSO": BeanSource.ESPRESSO,
    "CAPPUCCINO": BeanSource.CAPPUCCINO,
}


def parameterized_schema(id, source_enum):
    return number.NUMBER_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(id),
            cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries2200),
            cv.Required(STATUS_SENSOR_ID): cv.use_id(StatusSensor),
            cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(
                number.NUMBER_MODES, upper=True
            ),
            cv.Required(CONF_SOURCE): cv.enum(source_enum, upper=True, space="_"),
        }
    ).extend(cv.COMPONENT_SCHEMA)


CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_TYPE_SIZE: parameterized_schema(SizeSettings, SIZE_SOURCES),
        CONF_TYPE_BEAN: parameterized_schema(BeanSettings, BEAN_SOURCES),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    status_sensor = await cg.get_variable(config[STATUS_SENSOR_ID])
    var = await number.new_number(config, min_value=1, max_value=3, step=1)
    await cg.register_component(var, config)

    cg.add(var.set_source(config[CONF_SOURCE]))
    cg.add(var.set_status_sensor(status_sensor))
    if config[CONF_TYPE] == CONF_TYPE_SIZE:
        cg.add(parent.add_size_settings(var))
    else:
        cg.add(parent.add_bean_settings(var))
