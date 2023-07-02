#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "../philips_status_sensor/status_sensor.h"

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_water_sensor
        {
            /**
             * @brief Source of the sensor value
             *
             */
            enum Source
            {
                COFFEE = 0,
                ESPRESSO,
                HOT_WATER
            };

            /**
             * @brief Reports the currently selected water amount of the coffee machine from the selected source.
             */
            class WaterSensor : public sensor::Sensor, public Component
            {
            public:
                void setup() override;
                void dump_config() override;

                /**
                 * @brief Set the source used by this WaterSensor.
                 *
                 * @param source Source of the value
                 */
                void set_source(Source source) { source_ = source; };

                /**
                 * @brief Sets the status sensor reference
                 * @param status_sensor text sensor reference
                 */
                void set_status_sensor(philips_status_sensor::StatusSensor *status_sensor)
                {
                    status_sensor_ = status_sensor;
                }

                /**
                 * @brief Published the state if it's different form the currently published state.
                 *
                 */
                void update_state(float state)
                {
                    if (std::isnan(this->raw_state) && std::isnan(state))
                        return;

                    if (this->raw_state != state)
                    {
                        publish_state(state);
                        return;
                    }
                }

                /**
                 * @brief Updates the sensor value based on the incoming messages.
                 * @param data incoming data from the motherboard
                 * @param len length of the incoming data
                 */
                void update_status(uint8_t *data, size_t len);

            private:
                /// @brief Indicator for the sensors source value
                Source source_;

                /// @brief reference to a status sensor
                philips_status_sensor::StatusSensor *status_sensor_;
            };

        } // namespace philips_water_sensor
    }     // namespace philips_series_2200
} // namespace esphome