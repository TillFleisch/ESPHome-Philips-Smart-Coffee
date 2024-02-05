#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/uart/uart.h"
#include "../commands.h"

#define MESSAGE_REPETITIONS 5
#define POWER_TRIP_RETRY_DELAY 100
#define MAX_POWER_TRIP_COUNT 5

namespace esphome
{
    namespace philips_coffee_machine
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
                 * @brief Sets the time period which is used for removing power to the display.
                 */
                void set_power_trip_delay(uint32_t time)
                {
                    power_trip_delay_ = time;
                }

                /**
                 * @brief Sets the cleaning status of this power switch.
                 * If true the machine will clean during startup
                 */
                void set_cleaning(bool cleaning)
                {
                    cleaning_ = cleaning;
                }

                /**
                 * @brief Sets the initial state reference on this power switch
                 *
                 * @param initial_state hub components initial state reference
                 */
                void set_initial_state(bool *initial_state)
                {
                    initial_state_ = initial_state;
                }

                /**
                 * @brief Sets the number of message repetitions to use while turning on the machine
                 *
                 * @param count number of repetitions
                 */
                void set_power_message_repetitions(uint count)
                {
                    power_message_repetitions_ = count;
                }

                /**
                 * @brief Processes and publish the new switch state.
                 */
                void update_state(bool state);

            private:
                /// @brief Reference to uart which is connected to the mainboard
                uart::UARTDevice *mainboard_uart_;
                /// @brief power pin which is used for display power
                GPIOPin *power_pin_;
                /// @brief True if the coffee machine is supposed to clean
                bool cleaning_ = true;
                /// @brief length of power outage applied to the display
                uint32_t power_trip_delay_ = 500;
                /// @brief Determines wether a power trip should be performed
                bool should_power_trip_ = false;
                /// @brief Time of last power trip
                uint32_t last_power_trip_ = 0;
                /// @brief nr of power performed power trips
                uint power_trip_count_ = 0;
                /// @brief determines how often the power on message is repeated
                uint power_message_repetitions_ = 5;
                /// @brief initial power state reference
                bool *initial_state_;
            };

        } // namespace philips_power_switch
    }     // namespace philips_coffee_machine
} // namespace esphome