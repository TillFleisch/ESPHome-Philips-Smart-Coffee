#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "../philips_power_switch/power.h"
#include "../philips_action_button/action_button.h"
#include "../philips_status_sensor/status_sensor.h"
#include "../philips_bean_settings/bean_settings.h"
#include "../philips_size_settings/size_settings.h"

#define POWER_STATE_TIMEOUT 500

namespace esphome
{
    namespace philips_series_2200
    {

        class PhilipsSeries2200 : public Component
        {
        public:
            void setup() override;
            void loop() override;
            void dump_config() override;

            /**
             * @brief Set the reference to the uart port connected to the display
             *
             * @param uart display uart reference
             */
            void register_display_uart(uart::UARTComponent *uart)
            {
                display_uart_ = uart::UARTDevice(uart);
            };

            /**
             * @brief Set the reference to the uart port connected to the Mainboard
             *
             * @param uart Mainboard uart reference
             */
            void register_mainboard_uart(uart::UARTComponent *uart)
            {
                mainboard_uart_ = uart::UARTDevice(uart);
            };

            /**
             * @brief Sets the pin used for power tripping the display unit
             *
             * @param pin GPIO pin
             */
            void set_power_pin(GPIOPin *pin)
            {
                power_pin_ = pin;
            };

            /**
             * @brief Sets the invert status of the power pin
             */
            void set_invert_power_pin(bool invert)
            {
                initial_pin_state_ = !invert;
            }

            void set_power_trip_delay(uint32_t time)
            {
                power_trip_delay_ = time;
            }

            /**
             * @brief Reference to a power switch object.
             * The switch state will be updated based on the presence/absence of display update messages.
             *
             * @param power_switch a power_switch reference
             */
            void register_power_switch(philips_power_switch::Power *power_switch)
            {
                power_switch->set_mainboard_uart(&mainboard_uart_);
                power_switch->set_power_pin(power_pin_);
                power_switch->set_power_trip_delay(power_trip_delay_);
                power_switch->set_initial_state(&initial_pin_state_);
                power_switches_.push_back(power_switch);
            };

            /**
             * @brief Adds an action button to this controller.
             * No reference is stored, but the correct uart references is passed along.
             *
             * @param action_button Action button which will be added to this controller
             */
            void add_action_button(philips_action_button::ActionButton *action_button)
            {
                action_button->set_uart_device(&mainboard_uart_);
                action_buttons_.push_back(action_button);
            }

            /**
             * @brief Adds a status sensor to this controller
             * @param status_sensor reference to a status sensor
             */
            void add_status_sensor(philips_status_sensor::StatusSensor *status_sensor)
            {
                status_sensors_.push_back(status_sensor);
            }

            /**
             * @brief Adds a bean settings entity to this controller
             * @param bean_settings reference to a bean settings entity
             */
            void add_bean_settings(philips_bean_settings::BeanSettings *bean_settings)
            {
                bean_settings->set_uart_device(&mainboard_uart_);
                bean_settings_.push_back(bean_settings);
            }

            /**
             * @brief Adds a size settings entity to this controller
             * @param water_sensor reference to a size setting
             */
            void add_size_settings(philips_size_settings::SizeSettings *size_setting)
            {
                size_setting->set_uart_device(&mainboard_uart_);
                size_setting_.push_back(size_setting);
            }

        private:
            long last_message_from_mainboard_time_ = 0;
            long last_message_from_display_time_ = 0;

            /// @brief reference to uart connected to the display unit
            uart::UARTDevice display_uart_;

            /// @brief reference to uart connected to the mainboard
            uart::UARTDevice mainboard_uart_;

            /// @brief pin connect to display panel power transistor/mosfet
            GPIOPin *power_pin_;

            /// @brief the initial power pin state (may be inverted through user configuration)
            bool initial_pin_state_ = true;

            /// @brief length of power outage applied to the display
            uint32_t power_trip_delay_ = 500;

            /// @brief power switch reference
            std::vector<philips_power_switch::Power *> power_switches_;

            /// @brief list of status sensors to update with messages
            std::vector<philips_status_sensor::StatusSensor *> status_sensors_;

            /// @brief list of registered bean settings
            std::vector<philips_bean_settings::BeanSettings *> bean_settings_;

            /// @brief list of registered water sensors
            std::vector<philips_size_settings::SizeSettings *> size_setting_;

            /// @brief list of registered action buttons
            std::vector<philips_action_button::ActionButton *> action_buttons_;
        };

    } // namespace philips_series_2200
} // namespace esphome
