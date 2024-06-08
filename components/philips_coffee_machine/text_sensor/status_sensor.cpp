#include "esphome/core/log.h"
#include "status_sensor.h"

namespace esphome
{
    namespace philips_coffee_machine
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

            void StatusSensor::update_status(uint8_t *data)
            {

                // Check if the play/pause button is on/off/blinking
                if ((data[16] == led_on) != play_pause_led_)
                {
                    play_pause_last_change_ = millis();
                }
                play_pause_led_ = data[16] == led_on;

                if ((data[11] == led_on) != show_size_led_last_change_)
                {
                    show_size_led_last_change_ = millis();
                }
                show_size_led_last_change_ = data[11] == led_on;

                // Check for idle state (selection led on)
#ifdef PHILIPS_EP3243
                if (data[3] == led_on && data[4] == led_on && data[5] == led_on && data[13] == led_off && data[14] == led_off && data[15] == led_off)
#else
                if (data[3] == led_on && data[4] == led_on && data[5] == led_on && data[6] != led_off)
#endif
                {
                    // selecting a beverage can result in a short "busy" period since the play/pause button has not been blinking
                    // This can be circumvented: if the user is on the selection screen/idle we can reset the timer
                    play_pause_last_change_ = millis();

                    update_state(state_idle);
                    return;
                }

                bool is_play_pause_blinking = millis() - play_pause_last_change_ < BLINK_THRESHOLD;
                bool show_size_changed_recently = show_size_led_last_change_ < BLINK_THRESHOLD;

                // Check for rotating icons - pre heating
                if (data[3] == led_half || data[4] == led_half || data[5] == led_half || data[6] == led_half)
                {
                    if (play_pause_led_)
                        update_state(state_cleaning);
                    else
                        update_state(state_preparing);
                    return;
                }

                // 3 warning lights indicate an internal error (i.e. overheating)
                if (data[15] != led_off && data[14] == led_second)
                {
                    update_state(state_internal_error);
                    return;
                }

                // Warning/Error led
                if (data[15] == led_second)
                {
                    update_state(state_error);
                    return;
                }

                // Water empty led
                if (data[14] == led_second)
                {
                    update_state(state_water_empty);
                    return;
                }

                // Waste container led
                if (data[15] == led_on)
                {
                    update_state(state_waste_warning);
                    return;
                }

                // Coffee selected
                if (data[3] == led_off && data[4] == led_off && (data[5] == led_on || data[5] == led_second) && data[6] == led_off)
                {
                    if (is_play_pause_blinking)
                    {
                        if (data[9] == led_second)
                        {
                            update_state(state_ground_coffee_selected);
                        }
                        else if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_coffee_programming_mode);
                        }
                        else
                        {
                            update_state((data[5] == led_on) ? state_coffee_selected : state_coffee_2x_selected);
                        }
                    }
                    else
                    {
                        update_state((data[5] == led_on) ? state_coffee_brewing : state_coffee_2x_brewing);
                    }
                    return;
                }

                // Steam selected
                if (data[3] == led_off && data[4] == led_off && data[5] == led_off && (data[6] == led_on || data[6] == led_third))
                {
#ifdef PHILIPS_EP2235
                    if (is_play_pause_blinking)
                    {
                        if (data[9] == led_second)
                        {
                            update_state(state_ground_cappuccino_selected);
                        }
                        else if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_cappuccino_programming_mode);
                        }
                        else
                        {
                            update_state(state_cappuccino_selected);
                        }
                    }
                    else
                    {
                        update_state(state_cappuccino_brewing);
                    }
#elif defined(PHILIPS_EP3243)
                    if (is_play_pause_blinking)
                    {
                        if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_latte_programming_mode);
                        }
                        else
                        {
                            update_state(data[9] == led_second ? state_ground_latte_selected : state_latte_selected);
                        }
                    }
                    else
                    {
                        update_state(state_latte_brewing);
                    }
#else
                    if (is_play_pause_blinking)
                    {
                        update_state(state_steam_selected);
                    }
                    else
                    {
                        update_state(state_steam_brewing);
                    }
#endif
                    return;
                }

                // Hot water selected
#ifdef PHILIPS_EP3243
                if (data[3] == led_off && data[4] == led_off && data[5] == led_off && data[6] == led_off && data[7] == led_second)
#else
                if (data[3] == led_off && data[4] == led_on && data[5] == led_off && data[6] == led_off)
#endif
                {
                    if (is_play_pause_blinking)
                    {
                        if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_hot_water_programming_mode);
                        }
                        else
                        {
                            update_state(state_hot_water_selected);
                        }
                    }
                    else
                    {
                        update_state(state_hot_water_brewing);
                    }
                    return;
                }

                // Espresso selected
                if ((data[3] == led_on || data[3] == led_second) && data[4] == led_off && data[5] == led_off && data[6] == led_off)
                {
                    if (is_play_pause_blinking)
                    {
                        if (data[9] == led_second)
                        {
                            update_state(state_ground_espresso_selected);
                        }
                        else if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_espresso_programming_mode);
                        }
                        else
                        {
                            update_state((data[3] == led_on) ? state_espresso_selected : state_espresso_2x_selected);
                        }
                    }
                    else
                    {
                        update_state((data[3] == led_on) ? state_espresso_brewing : state_espresso_2x_brewing);
                    }
                    return;
                }

#ifdef PHILIPS_EP3243
                // Cappuccino selected
                if (data[3] == led_off && data[4] == led_on && data[5] == led_off && data[6] == led_off)
                {
                    if (is_play_pause_blinking)
                    {
                        if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_cappuccino_programming_mode);
                        }
                        else
                        {
                            update_state(data[9] == led_second ? state_ground_cappuccino_selected : state_cappuccino_selected);
                        }
                    }
                    else
                    {
                        update_state(state_cappuccino_brewing);
                    }
                    return;
                }

                // Americano selected
                if (data[3] == led_off && data[4] == led_off && data[5] == led_off && (data[6] == led_second || data[7] == led_on))
                {
                    if (is_play_pause_blinking)
                    {
                        if (data[9] == led_second)
                        {
                            update_state(state_ground_americano_selected);
                        }
                        else if (data[11] == led_off && show_size_changed_recently)
                        {
                            update_state(state_americano_programming_mode);
                        }
                        else
                        {
                            update_state((data[6] == led_second) ? state_americano_selected : state_americano_2x_selected);
                        }
                    }
                    else
                    {
                        update_state((data[6] == led_second) ? state_americano_brewing : state_americano_2x_brewing);
                    }
                    return;
                }
#endif
            }

        } // namespace philips_status_sensor
    }     // namespace philips_coffee_machine
} // namespace esphome