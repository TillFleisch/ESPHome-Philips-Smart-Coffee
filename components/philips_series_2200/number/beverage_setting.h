#pragma once

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "esphome/components/uart/uart.h"
#include "../text_sensor/status_sensor.h"
#include "../commands.h"

#define MESSAGE_REPETITIONS 5
#define SETTINGS_BUTTON_SEQUENCE_DELAY 500

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_beverage_setting
        {
            /**
             * @brief Source/target of the value
             *
             */
            enum Source
            {
                ANY = 0,
                COFFEE,
                ESPRESSO,
                CAPPUCCINO,
                HOT_WATER
            };

            /**
             * @brief Reports the currently selected size/bean count of the coffee machine from the selected source.
             */
            class BeverageSetting : public number::Number, public Component
            {
            public:
                void setup() override;
                void dump_config() override;

                /**
                 * @brief Pass user intput to mainboard.
                 */
                void control(float value);

                /**
                 * Sets the type of this beverage setting.
                 * @param is_bean True for bean, False for size
                 */
                void set_type(bool is_bean)
                {
                    is_bean_ = is_bean;
                }

                /**
                 * @brief Set the source used by this size/bean settings entity.
                 *
                 * @param source Source of the value
                 */
                void set_source(Source source)
                {
                    source_ = source;
                };

                /**
                 * @brief Sets the status sensor reference
                 * @param status_sensor text sensor reference
                 */
                void set_status_sensor(philips_status_sensor::StatusSensor *status_sensor)
                {
                    status_sensor_ = status_sensor;
                }

                /**
                 * @brief Reference to uart which is connected to the mainboard
                 *
                 * @param uart uart connected to mainboard
                 */
                void set_uart_device(uart::UARTDevice *uart)
                {
                    mainboard_uart_ = uart;
                };

                /**
                 * @brief Published the state if it's different form the currently published state.
                 *
                 */
                void update_state(float state)
                {
                    if (std::isnan(this->state) && std::isnan(state))
                        return;

                    if (this->state != state)
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
                /// @brief Indicated if this setting component applies to beans, size otherwise
                bool is_bean_ = false;

                /// @brief Indicator for the sensors source value
                Source source_;

                /// @brief reference to uart connected to mainboard
                uart::UARTDevice *mainboard_uart_;

                /// @brief User selected target amount
                int8_t target_amount_ = -1;

                /// @brief timestamp of the last transmission
                uint32_t last_transmission_ = 0;

                /// @brief reference to a status sensor
                philips_status_sensor::StatusSensor *status_sensor_;
            };

        } // namespace philips_beverage_setting
    }     // namespace philips_series_2200
} // namespace esphome