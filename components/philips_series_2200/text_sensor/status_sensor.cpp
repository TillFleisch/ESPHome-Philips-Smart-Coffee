#include "esphome/core/log.h"
#include "status_sensor.h"

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_status_sensor
        {
            static const char *TAG = "philips_status_sensor";

            void StatusSensor::setup()
            {
            }

            void StatusSensor::dump_config()
            {
                ESP_LOGCONFIG(TAG, "Philips Status Text Sensor");
            }

            void StatusSensor::update_status(uint8_t *data, size_t len)
            {
                // reject invalid messages
                if (len < 19 && data[0] != message_header[0] && data[1] != message_header[1])
                    return;

                // TODO: figure out how the checksum is calculated and only parse valid messages

                // Check if the play/pause button is on/off/blinking
                if ((data[16] == led_on) != play_pause_led_)
                {
                    play_pause_last_change_ = millis();
                }
                play_pause_led_ = data[16] == led_on;

                // Check for idle state (selection led on)
                if (data[3] == led_on && data[4] == led_on && data[5] == led_on && data[6] == led_on)
                {
                    // selecting a beverage can result in a short "busy" period since the play/pause button has not been blinking
                    // This can be circumvented: if the user is on the selection screen/idle we can reset the timer
                    play_pause_last_change_ = millis();

                    update_state("Idle");
                    return;
                }

                // Check for rotating icons - pre heating
                if (data[3] == led_half || data[4] == led_half || data[5] == led_half || data[6] == led_half)
                {
                    if (play_pause_led_)
                        update_state("Cleaning");
                    else
                        update_state("Preparing");
                    return;
                }

                // Water empty led
                if (data[14] == led_second)
                {
                    update_state("Water empty");
                    return;
                }

                // Waste container led
                if (data[15] == led_on)
                {
                    update_state("Waste container warning");
                    return;
                }

                // Warning/Error led
                if (data[15] == led_second)
                {
                    update_state("Error");
                    return;
                }

                // Coffee selected
                if (data[3] == led_off && data[4] == led_off && (data[5] == led_on || data[5] == led_second) && data[6] == led_off)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                    {
                        if (data[9] == led_second)
                        {
                            update_state("Ground Coffee selected");
                        }
                        else if (data[11] == led_off)
                        {
                            update_state("Coffee programming mode selected");
                        }
                        else
                        {
                            update_state((data[5] == led_on) ? "Coffee selected" : "2x Coffee selected");
                        }
                    }
                    else
                    {
                        update_state("Busy");
                    }
                    return;
                }

                // Steam selected
                if (data[3] == led_off && data[4] == led_off && data[5] == led_off && data[6] == led_on)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                        update_state(use_cappuccino_ ? "Cappuccino selected" : "Steam selected");
                    else
                        update_state("Busy");
                    return;
                }

                // Hot water selected
                if (data[3] == led_off && data[4] == led_on && data[5] == led_off && data[6] == led_off)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                    {
                        if (data[11] == led_on)
                        {
                            update_state("Hot water selected");
                        }
                        else
                        {
                            update_state("Hot water programming mode selected");
                        }
                    }
                    else
                    {
                        update_state("Busy");
                    }
                    return;
                }

                // Espresso selected
                if ((data[3] == led_on || data[3] == led_second) && data[4] == led_off && data[5] == led_off && data[6] == led_off)
                {
                    if (millis() - play_pause_last_change_ < BLINK_THRESHOLD)
                    {
                        if (data[9] == led_second)
                        {
                            update_state("Ground Espresso selected");
                        }
                        else if (data[11] == led_off)
                        {
                            update_state("Espresso programming mode selected");
                        }
                        else
                        {
                            update_state((data[3] == led_on) ? "Espresso selected" : "2x Espresso selected");
                        }
                    }
                    else
                    {
                        update_state("Busy");
                    }
                    return;
                }
            }

        } // namespace philips_status_sensor
    }     // namespace philips_series_2200
} // namespace esphome