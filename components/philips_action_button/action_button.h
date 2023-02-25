#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "esphome/components/uart/uart.h"

#define MESSAGE_REPETITIONS 5
#define BUTTON_SEQUENCE_DELAY 100

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_action_button
        {
            /**
             * @brief Executable actions. Select actions only select the type.
             * Make actions select the type and press play.
             *
             */
            enum Action
            {
                SELECT_COFFEE = 0,
                MAKE_COFFEE,
                SELECT_ESPRESSO,
                MAKE_ESPRESSO,
                SELECT_HOT_WATER,
                MAKE_HOT_WATER,
                SELECT_STEAM,
                MAKE_STEAM,
                SELECT_BEAN,
                SELECT_SIZE,
                SELECT_AQUA_CLEAN,
                SELECT_CALC_CLEAN,
                PLAY_PAUSE,
            };

            /**
             * @brief Emulates (a) button press(es) using the mainboard uart.
             *
             */
            class ActionButton : public button::Button, public Component
            {
            public:
                void dump_config() override;

                /**
                 * @brief Set the action used by this ActionButton.
                 *
                 * @param action Action to use
                 */
                void set_action(Action action) { action_ = action; };

                /**
                 * @brief Reference to uart which is connected to the mainboard
                 *
                 * @param uart uart connected to mainboard
                 */
                void set_uart_device(uart::UARTDevice *uart) { mainboard_uart_ = uart; };

            private:
                /**
                 * @brief Writes data MESSAGE_REPETITIONS times to the mainboard uart
                 *
                 * @param data Data to send
                 */
                void write_array(const std::vector<uint8_t> &data);

                /**
                 * @brief Executes button press
                 *
                 */
                void press_action() override;

                /// @brief Action used by this Button
                Action action_;
                /// @brief reference to uart connected to mainboard
                uart::UARTDevice *mainboard_uart_;
            };
        } // namespace philips_action_button
    }     // namespace philips_series_2200
} // namespace esphome