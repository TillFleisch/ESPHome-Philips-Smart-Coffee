import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from ..philips_series_2200 import CONTROLLER_ID, PhilipsSeries2200

DEPENDENCIES = ['philips_series_2200']

CLEAN_DURING_START = 'clean'

power_switch_namespace = cg.esphome_ns.namespace(
    'philips_series_2200').namespace("philips_power_switch")
PowerSwitch = power_switch_namespace.class_(
    'Power', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PowerSwitch),
    cv.Required(CONTROLLER_ID): cv.use_id(PhilipsSeries2200),
    cv.Optional(CLEAN_DURING_START, default=True): cv.boolean
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    controller = yield cg.get_variable(config[CONTROLLER_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield switch.register_switch(var, config)

    cg.add(var.set_cleaning(config[CLEAN_DURING_START]))
    cg.add(controller.register_power_switch(var))
