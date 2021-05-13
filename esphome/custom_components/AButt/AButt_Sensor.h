#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

class AButt;

namespace esphome {
namespace aButt_sensor {

class AButtSensor : public sensor::Sensor, public PollingComponent {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;

protected:
	AButt* button;
};

} //namespace empty_sensor
} //namespace esphome
