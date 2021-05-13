#include "esphome/core/log.h"
#include "AButt_Sensor.h"
#include "AButt.h"

namespace esphome {
namespace aButt_sensor {

static const char *TAG = "AButt.sensor";


AButtSensor* globSensorVar = nullptr;
void clicked(unsigned short clicks) {
	globSensorVar->publish_state(clicks);
}

void holdStart() {
	globSensorVar->publish_state(-1);
}
void holdEnd() {
	globSensorVar->publish_state(0);
}

void AButtSensor::setup() {
	button = new AButt(_pin, _inverted);

	if (_inverted) {
		pinMode(_pin, INPUT_PULLUP);
	} else {
		pinMode(_pin, INPUT);
	}

	button->onClick(clicked);
	button->onHold(holdStart, holdEnd);

	ESP_LOGCONFIG(TAG, "Setup completed");
}

void AButtSensor::loop() {
	button->update();
}

void AButtSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "AButt sensor");
}

} //namespace empty_sensor
} //namespace esphome
