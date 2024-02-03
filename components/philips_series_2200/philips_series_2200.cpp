#include "esphome/core/log.h"
#include "philips_series_2200.h"

#define BUFFER_SIZE 32

namespace esphome
{
    namespace philips_series_2200
    {

        static const char *TAG = "philips_series_2200";

        void PhilipsSeries2200::setup()
        {
            power_pin_->setup();
            power_pin_->pin_mode(gpio::FLAG_OUTPUT);
            power_pin_->digital_write(initial_pin_state_);
        }

        void PhilipsSeries2200::loop()
        {
            uint8_t buffer[BUFFER_SIZE];

            // Pipe display to mainboard
            if (display_uart_.available())
            {
                uint8_t size = std::min(display_uart_.available(), BUFFER_SIZE);
                display_uart_.read_array(buffer, size);

                // Check if a action button is currently performing a long press
                bool long_pressing = false;
#ifdef USE_BUTTON
                for (philips_action_button::ActionButton *button : action_buttons_)
                {
                    if (button->is_long_pressing())
                    {
                        long_pressing = true;
                        break;
                    }
                }
#endif

                // Drop messages if button long-press is currently injecting messages
                if (!long_pressing)
                    mainboard_uart_.write_array(buffer, size);
                last_message_from_display_time_ = millis();
            }

            // Read until start index
            while (mainboard_uart_.available())
            {
                uint8_t buffer = mainboard_uart_.peek();
                if (buffer == message_header[0])
                    break;
                display_uart_.write(mainboard_uart_.read());
            }

            // Pipe to display
            if (mainboard_uart_.available())
            {
                uint8_t size = std::min(mainboard_uart_.available(), BUFFER_SIZE);
                mainboard_uart_.read_array(buffer, size);

                display_uart_.write_array(buffer, size);

                // Only process messages starting with start bytes
                if (size > 1 && buffer[0] == message_header[0] && buffer[1] == message_header[1])
                {
                    last_message_from_mainboard_time_ = millis();

#ifdef USE_TEXT_SENSOR
                    // Update status sensors
                    for (philips_status_sensor::StatusSensor *status_sensor : status_sensors_)
                        status_sensor->update_status(buffer, size);

#ifdef USE_NUMBER
                    // Update beverage settings
                    for (philips_beverage_setting::BeverageSetting *beverage_setting : beverage_settings_)
                        beverage_setting->update_status(buffer, size);
#endif
#endif
                }
            }

            // Publish power state if required as long as the display is requesting messages
            if (millis() - last_message_from_display_time_ > POWER_STATE_TIMEOUT)
            {
#ifdef USE_SWITCH
                // Update power switches
                for (philips_power_switch::Power *power_switch : power_switches_)
                    power_switch->update_state(false);
#endif

#ifdef USE_TEXT_SENSOR
                // Update status sensors
                for (philips_status_sensor::StatusSensor *status_sensor : status_sensors_)
                    status_sensor->set_state_off();
#endif
            }
            else
            {
#ifdef USE_SWITCH
                // Update power switches
                for (philips_power_switch::Power *power_switch : power_switches_)
                    power_switch->update_state(true);
#endif
            }

            display_uart_.flush();
            mainboard_uart_.flush();
        }

        void PhilipsSeries2200::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Philips Series 2200");
            display_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
            mainboard_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
        }

    } // namespace philips_series_2200
} // namespace esphome