#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/uart/uart.h"

#define MESSAGE_REPETITIONS 5
#define POWER_TRIP_DELAY 500

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_power_switch
        {

            /**
             * @brief Power Switch wich reflects the power state of the coffee machine.
             * On/Off will change the hardware state of the machine using uart and the power tripping mechanism.
             *
             */
            class Power : public switch_::Switch, public Component
            {
            public:
                void setup() override;
                void loop() override;

                /**
                 * @brief Write a boolean state to this entity which should be propagated to hardware
                 *
                 * @param state new State the entity should write to hardware
                 */
                void write_state(bool state);
                void dump_config() override;

                /**
                 * @brief Sets the mainboard uart reference used by this power switch. The uart is used to fake power on and power off messages.
                 *
                 * @param uart uart reference
                 */
                void set_mainboard_uart(uart::UARTDevice *uart)
                {
                    mainboard_uart_ = uart;
                }

                /**
                 * @brief Sets the power pin reference which is used to trip the display power
                 *
                 * @param ping pin reference
                 */
                void set_power_pin(GPIOPin *pin)
                {
                    power_pin_ = pin;
                }

                /**
                 * @brief Sets the cleaning status of this power switch.
                 * If true the machine will clean during startup
                 */
                void set_cleaning(bool cleaning)
                {
                    cleaning_ = cleaning;
                }

            private:
                /// @brief Reference to uart which is connected to the mainboard
                uart::UARTDevice *mainboard_uart_;
                /// @brief power pin which is used for display power
                GPIOPin *power_pin_;
                /// @brief True if the coffee machine is supposed to clean
                bool cleaning_ = true;
            };

        } // namespace power_switch
    }     // namespace philips_series_2200
} // namespace esphome