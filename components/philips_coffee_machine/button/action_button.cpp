#include "esphome/core/log.h"
#include "action_button.h"

namespace esphome
{
    namespace philips_coffee_machine
    {
        namespace philips_action_button
        {

            static const char *const TAG = "philips-action-button";

            void ActionButton::dump_config()
            {
                LOG_BUTTON("", "Philips Action Button", this);
            }

            void ActionButton::loop()
            {
                // Repeated message sending for long presses
                if (should_long_press_ && millis() - press_start_ <= LONG_PRESS_DURATION)
                {
                    if (millis() - last_message_sent_ > LONG_PRESS_REPETITION_DELAY)
                    {
                        last_message_sent_ = millis();
                        perform_action();
                    }
                    is_long_pressing_ = true;
                }
                else
                {
                    is_long_pressing_ = false;
                }
            }

            void ActionButton::write_array(const std::vector<uint8_t> &data)
            {
                for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                    mainboard_uart_->write_array(data);
                mainboard_uart_->flush();
            }

            void ActionButton::press_action()
            {
                if (should_long_press_)
                {
                    // Reset button press start time
                    press_start_ = millis();
                    last_message_sent_ = 0;
                }
                else
                {
                    // Perform a single button press
                    perform_action();
                }
            }

            void ActionButton::execute_command(const std::vector<uint8_t> &command)
            {
                auto action = action_;

                write_array(command);
                
                if (
                    action == SELECT_COFFEE 
                    || action == SELECT_ESPRESSO
                    || action == SELECT_ESPRESSO_LUNGO
                    || action == SELECT_HOT_WATER
                    || action == SELECT_STEAM
                    || action == SELECT_CAPPUCCINO
                    || action == SELECT_LATTE
                    || action == SELECT_AMERICANO
                ) return;

                delay(BUTTON_SEQUENCE_DELAY);
                write_array(command_press_play_pause);

            }

            void ActionButton::perform_action()
            {
                auto action = action_;

                // These are what the commands are mapped to.
                // command[0] - Coffee
                // command[1] - Espresso
                // command[2] - Hot Water
                // command[3] - Steam
                // command[4] - Cappuccino
                // command[5] - Latte
                // command[6] - Americano
                // command[7] - Espresso Lungo

                // Default for most models
                std::vector<uint8_t> command[8] {
                    command_press_1,
                    command_press_2,
                    command_press_3
                };

                // command additions and overrides for specific models
                #ifdef PHILIPS_EP2220
                    command[3] = command_press_4;
                #endif

                #ifdef PHILIPS_EP2235
                    command[4] = command_press_4;
                #endif

                #ifdef PHILIPS_EP3221
                    command[0] = command_press_5;
                    command[2] = command_press_4;
                    command[3] = command_press_3;
                    command[6] = command_press_6;
                    command[7] = command_press_1;
                #endif

                #ifdef PHILIPS_EP3243
                    command[4] = command_press_6;
                    command[5] = command_press_4;
                    command[6] = command_press_5;
                #endif
                
                switch (action) {
                    case SELECT_COFFEE:
                    case MAKE_COFFEE:
                        execute_command(command[0]);
                        break;
                    case SELECT_ESPRESSO:
                    case MAKE_ESPRESSO:
                        execute_command(command[1]);
                        break;
                    case SELECT_HOT_WATER:
                    case MAKE_HOT_WATER:
                        execute_command(command[2]);
                        break;
                    case SELECT_STEAM:
                    case MAKE_STEAM:
                        execute_command(command[3]);
                        break;
                    case SELECT_CAPPUCCINO:
                    case MAKE_CAPPUCCINO:
                        execute_command(command[4]);
                        break;
                    case SELECT_LATTE:
                    case MAKE_LATTE:
                        execute_command(command[5]);
                        break;
                    case SELECT_AMERICANO:
                    case MAKE_AMERICANO:
                        execute_command(command[6]);
                        break;
                    case SELECT_ESPRESSO_LUNGO:
                    case MAKE_ESPRESSO_LUNGO:
                        execute_command(command[7]);
                        break;
                    case PLAY_PAUSE:
                        write_array(command_press_play_pause);
                        break;
                    case SELECT_BEAN:
                        write_array(command_press_bean);
                        break;
                    case SELECT_SIZE:
                        write_array(command_press_size);
                        break;
#if defined(PHILIPS_EP3243)
                    case SELECT_MILK:
                        write_array(command_press_milk);
                        break;
#endif
                    case SELECT_AQUA_CLEAN:
                        write_array(command_press_aqua_clean);
                        break;
                    case SELECT_CALC_CLEAN:
                        write_array(command_press_calc_clean);
                        break;
                    default:
                        ESP_LOGE(TAG, "Invalid Action provided!");
                }

            }


        } // namespace philips_action_button
    }     // namespace philips_coffee_machine
} // namespace esphome