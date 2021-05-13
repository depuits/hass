import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, ICON_EMPTY

aButt_sensor_ns = cg.esphome_ns.namespace('aButt_sensor')

AButtSensor = aButt_sensor_ns.class_('AButtSensor', cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema('clicks', ICON_EMPTY, 1).extend({
    cv.GenerateID(): cv.declare_id(AButtSensor)
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
    