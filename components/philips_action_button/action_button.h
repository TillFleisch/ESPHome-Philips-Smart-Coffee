#pragma once

#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include "esphome/components/uart/uart.h"

#define MESSAGE_REPETITIONS 5
#define BUTTON_SEQUENCE_DELAY 100
#define LONG_PRESS_REPETITION_DELAY 50
#define LONG_PRESS_DURATION 3500

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
                void loop() override;

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

                /**
                 * @brief Sets the long press parameter on this button component.
                 *
                 * @param long_press True if a long press should be executed, false otherwise
                 */
                void set_long_press(bool long_press)
                {
                    should_long_press_ = long_press;
                }

                /**
                 * @brief Determines if the button is currently performing a long press
                 *
                 * @return True if the button is currently performing a long press
                 */
                bool is_long_pressing()
                {
                    return is_long_pressing_;
                }

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

                /**
                 * @brief Writes the button to uart or initializes loop based message sending
                 *
                 */
                void perform_action();

                /// @brief Action used by this Button
                Action action_;
                /// @brief reference to uart connected to mainboard
                uart::UARTDevice *mainboard_uart_;
                /// @brief time in ms for how long the button should be pressed.
                bool should_long_press_ = false;
                /// @brief true if the component is currently performing a long press
                bool is_long_pressing_ = false;
                /// @brief time at which the button press was started
                long press_start_ = -(LONG_PRESS_DURATION + 1);
                /// @brief time at which the last message was sent
                long last_message_sent_ = 0;
            };
        } // namespace philips_action_button
    }     // namespace philips_series_2200
} // namespace esphome