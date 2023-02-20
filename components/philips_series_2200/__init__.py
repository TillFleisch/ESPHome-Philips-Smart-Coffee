from typing_extensions import Required
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.components.uart import UARTComponent

from esphome.const import CONF_ID

AUTO_LOAD = ['light']

DEPENDENCIES = ['uart']

DISPLAY_UART_ID = 'display_uart'
MAINBOARD_UART_ID = 'mainboard_uart'


philips_series_2200_ns = cg.esphome_ns.namespace('philips_series_2200')
PhilipsSeries2200 = philips_series_2200_ns.class_(
    'PhilipsSeries2200', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PhilipsSeries2200),
    cv.Required(DISPLAY_UART_ID): cv.use_id(UARTComponent),
    cv.Required(MAINBOARD_UART_ID): cv.use_id(UARTComponent)
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
