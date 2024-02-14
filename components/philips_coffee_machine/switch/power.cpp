#include "esphome/core/log.h"
#include "power.h"

namespace esphome
{
    namespace philips_coffee_machine
    {
        namespace philips_power_switch
        {

            static const char *TAG = "philips_power_switch";

            void Power::setup()
            {
            }

            void Power::loop()
            {
                if (should_power_trip_ && millis() - last_power_trip_ > power_trip_delay_ + POWER_TRIP_RETRY_DELAY)
                {
                    if (power_trip_count_ >= MAX_POWER_TRIP_COUNT)
                    {
                        should_power_trip_ = false;
                        ESP_LOGE(TAG, "Power tripping display failed!");
                        return;
                    }

                    // Perform power trip (invert state twice)
                    power_pin_->digital_write(!(*initial_state_));
                    delay(power_trip_delay_);
                    power_pin_->digital_write(*initial_state_);

                    last_power_trip_ = millis();
                    power_trip_count_++;
                }
            }

            void Power::write_state(bool state)
            {
                if (state)
                {
                    // Send pre-power on message
                    for (unsigned int i = 0; i <= power_message_repetitions_; i++)
                        mainboard_uart_->write_array(command_pre_power_on);

                    // Send power on message
                    if (cleaning_)
                    {
                        // Send power on command with cleaning
                        for (unsigned int i = 0; i <= power_message_repetitions_; i++)
                            mainboard_uart_->write_array(command_power_with_cleaning);
                    }
                    else
                    {
                        // Send power on command without cleaning
                        for (unsigned int i = 0; i <= power_message_repetitions_; i++)
                            mainboard_uart_->write_array(command_power_without_cleaning);
                    }

                    mainboard_uart_->flush();

                    // Perform power trip in component loop
                    should_power_trip_ = true;
                    power_trip_count_ = 0;
                }
                else
                {
                    // Send power off message
                    for (unsigned int i = 0; i <= power_message_repetitions_; i++)
                        mainboard_uart_->write_array(command_power_off);
                    mainboard_uart_->flush();
                }

                // The state will be published once the display starts sending messages
            }

            void Power::dump_config()
            {
                ESP_LOGCONFIG(TAG, "Philips Coffee Machine Power Switch");
            }

            void Power::update_state(bool state)
            {
                if (this->state != state)
                {
                    // Stop further power trips after successfully tripping
                    if (state && should_power_trip_)
                    {
                        ESP_LOGD(TAG, "Performed %i power trip(s).", power_trip_count_);
                        should_power_trip_ = false;
                        power_trip_count_ = 0;
                    }

                    publish_state(state);
                }
            }

        } // namespace philips_power_switch
    }     // namespace philips_coffee_machine
} // namespace esphome