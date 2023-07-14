import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_MODE
from ..philips_series_2200 import CONTROLLER_ID, PhilipsSeries2200
from ..philips_status_sensor.text_sensor import STATUS_SENSOR_ID, StatusSensor

AUTO_LOAD = ['number']
DEPENDENCIES = ['philips_series_2200', 'philips_status_sensor']

CONF_SOURCE = 'source'

philips_size_settings_ns = cg.esphome_ns.namespace(
    'philips_series_2200').namespace('philips_size_settings')
SizeSettings = philips_size_settings_ns.class_(
    'SizeSettings', number.Number, cg.Component)

Source = philips_size_settings_ns.enum("Source")
SOURCES = {
    "COFFEE": Source.COFFEE,
    "ESPRESSO": Source.ESPRESSO,
    "CAPPUCCINO": Source.CAPPUCCINO,
    "HOT_WATER": Source.HOT_WATER,
}

CONFIG_SCHEMA = number.NUMBER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(SizeSettings),
    cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries2200),
    cv.Required(STATUS_SENSOR_ID): cv.use_id(StatusSensor),
    cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(number.NUMBER_MODES, upper=True),
    cv.Required(CONF_SOURCE): cv.enum(
            SOURCES, upper=True, space="_"
        )
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    status_sensor = await cg.get_variable(config[STATUS_SENSOR_ID])
    var = await number.new_number(config, min_value=1, max_value=3, step=1)
    await cg.register_component(var, config)

    cg.add(var.set_source(config[CONF_SOURCE]))
    cg.add(var.set_status_sensor(status_sensor))
    cg.add(parent.add_size_settings(var))
