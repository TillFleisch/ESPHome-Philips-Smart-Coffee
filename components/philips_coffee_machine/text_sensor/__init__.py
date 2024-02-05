import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID

from .. import CONTROLLER_ID, PhilipsCoffeeMachine, philips_coffee_machine_ns

STATUS_SENSOR_ID = "status_sensor_id"

philips_status_sensor_ns = philips_coffee_machine_ns.namespace("philips_status_sensor")
StatusSensor = philips_status_sensor_ns.class_(
    "StatusSensor", text_sensor.TextSensor, cg.Component
)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(StatusSensor),
        cv.Required(CONTROLLER_ID): cv.use_id(PhilipsCoffeeMachine),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONTROLLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)

    cg.add(parent.add_status_sensor(var))
