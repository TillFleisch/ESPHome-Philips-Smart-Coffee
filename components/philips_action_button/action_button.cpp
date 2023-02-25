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

            void ActionButton::write_array(const std::vector<uint8_t> &data)
            {
                for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                    mainboard_uart_->write_array(data);
                mainboard_uart_->flush();
            }

            void ActionButton::press_action()
            {
                // Coffee
                if (action_ == SELECT_COFFEE || action_ == MAKE_COFFEE)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x08, 0x00, 0x00, 0x39, 0x1C});
                    if (action_ == SELECT_COFFEE)
                        return;

                    delay(BUTTON_SEQUENCE_DELAY);
                    action_ = PLAY_PAUSE;
                }

                // Espresso
                if (action_ == SELECT_ESPRESSO || action_ == MAKE_ESPRESSO)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x09, 0x2D});
                    if (action_ == SELECT_ESPRESSO)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action_ = PLAY_PAUSE;
                }

                // Hot water
                if (action_ == SELECT_HOT_WATER || action_ == MAKE_HOW_WATER)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x04, 0x00, 0x00, 0x21, 0x01});
                    if (action_ == SELECT_HOT_WATER)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action_ = PLAY_PAUSE;
                }

                // Steam
                if (action_ == SELECT_STEAM || action_ == MAKE_STEAM)
                {
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x10, 0x00, 0x00, 0x09, 0x26});
                    if (action_ == SELECT_STEAM)
                        return;
                    delay(BUTTON_SEQUENCE_DELAY);
                    action_ = PLAY_PAUSE;
                }

                // press/play or subsequent press/play
                if (action_ == PLAY_PAUSE)
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x00, 0x01, 0x19, 0x32});
                else if (action_ == SELECT_BEAN)
                    // bean button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x09, 0x2F});
                else if (action_ == SELECT_SIZE)
                    // size button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x20, 0x05});
                else if (action_ == SELECT_AQUA_CLEAN)
                    // aqua clean button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x10, 0x00, 0x0D, 0x36});
                else if (action_ == SELECT_CALC_CLEAN)
                    // calc clean button
                    write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x20, 0x00, 0x28, 0x37});
                else
                    ESP_LOGE(TAG, "Invalid Action provided!");
            }
        } // namespace philips_action_button
    }     // namespace philips_series_2200
} // namespace esphome