#include "esphome/core/log.h"
#include "action_button.h"

namespace esphome
{
    namespace philips_series_2200
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

            void ActionButton::perform_action()
            {
                auto action = action_;
                // Coffee
                if (action == SELECT_COFFEE || action == MAKE_COFFEE)
                {
                    write_array(command_press_coffee);
                    if (action == SELECT_COFFEE)
                        return;

                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // Espresso
                if (action == SELECT_ESPRESSO || action == MAKE_ESPRESSO)
                {
                    write_array(command_press_espresso);
                    if (action == SELECT_ESPRESSO)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // Hot water
                if (action == SELECT_HOT_WATER || action == MAKE_HOT_WATER)
                {
                    write_array(command_press_hot_water);
                    if (action == SELECT_HOT_WATER)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // Steam
                if (action == SELECT_STEAM || action == MAKE_STEAM)
                {
                    write_array(command_press_steam);
                    if (action == SELECT_STEAM)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action = PLAY_PAUSE;
                }

                // press/play or subsequent press/play
                if (action == PLAY_PAUSE)
                    write_array(command_press_play_pause);
                else if (action == SELECT_BEAN)
                    // bean button
                    write_array(command_press_bean);
                else if (action == SELECT_SIZE)
                    // size button
                    write_array(command_press_size);
                else if (action == SELECT_AQUA_CLEAN)
                    // aqua clean button
                    write_array(command_press_aqua_clean);
                else if (action == SELECT_CALC_CLEAN)
                    // calc clean button
                    write_array(command_press_calc_clean);
                else
                    ESP_LOGE(TAG, "Invalid Action provided!");
            }
        } // namespace philips_action_button
    }     // namespace philips_series_2200
} // namespace esphome