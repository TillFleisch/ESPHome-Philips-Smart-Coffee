import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from ..philips_series_2200 import CONTROLLER_ID, PhilipsSeries2200
from ..philips_status_sensor.text_sensor import STATUS_SENSOR_ID, StatusSensor

DEPENDENCIES = ['philips_series_2200', 'philips_status_sensor']

CONF_SOURCE = 'source'

philips_water_sensor_ns = cg.esphome_ns.namespace(
    'philips_series_2200').namespace('philips_water_sensor')
WaterSensor = philips_water_sensor_ns.class_(
    'WaterSensor', sensor.Sensor, cg.Component)

Source = philips_water_sensor_ns.enum("Source")
SOURCES = {
    "COFFEE": Source.COFFEE,
    "ESPRESSO": Source.ESPRESSO,
    "HOT_WATER": Source.HOT_WATER
}

CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(WaterSensor),
    cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries2200),
    cv.Required(STATUS_SENSOR_ID): cv.use_id(StatusSensor),
    cv.Required(CONF_SOURCE): cv.enum(
            SOURCES, upper=True, space="_"
        )
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    status_sensor = await cg.get_variable(config[STATUS_SENSOR_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    cg.add(var.set_source(config[CONF_SOURCE]))
    cg.add(var.set_status_sensor(status_sensor))
    cg.add(parent.add_water_sensor(var))
