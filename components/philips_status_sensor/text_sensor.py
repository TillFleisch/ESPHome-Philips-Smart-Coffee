import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID
from ..philips_series_2200 import CONTROLLER_ID, PhilipsSeries2200


philips_status_sensor_ns = cg.esphome_ns.namespace(
    'philips_series_2200').namespace('philips_status_sensor')
StatusSensor = philips_status_sensor_ns.class_(
    'StatusSensor', text_sensor.TextSensor, cg.Component)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(StatusSensor),
    cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries2200)
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)

    cg.add(parent.add_status_sensor(var))
