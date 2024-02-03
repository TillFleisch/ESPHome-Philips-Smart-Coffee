#include "esphome/core/log.h"
#include "beverage_setting.h"

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_beverage_setting
        {
            static const char *TAG = "philips_beverage_setting";

            void BeverageSetting::setup()
            {
            }

            void BeverageSetting::dump_config()
            {
                LOG_NUMBER(TAG, "Philips Beverage Setting", this);
            }

            void BeverageSetting::control(float value)
            {
                target_amount_ = (std::isnan(value) || std::isnan(state)) ? -1 : value;
            }

            void BeverageSetting::update_status(uint8_t *data, size_t len)
            {
                // reject invalid messages
                if (len < 19 && data[0] != 0xD5 && data[1] != 0x55)
                    return;

                // only apply status if source is currently selected
                if (status_sensor_->has_state() &&
                    (((source_ == COFFEE || source_ == ANY) &&
                      (status_sensor_->get_raw_state().compare("Coffee selected") == 0 ||
                       status_sensor_->get_raw_state().compare("2x Coffee selected") == 0 ||
                       (!is_bean_ && status_sensor_->get_raw_state().compare("Ground Coffee selected") == 0))) ||
                     ((source_ == ESPRESSO || source_ == ANY) &&
                      (status_sensor_->get_raw_state().compare("Espresso selected") == 0 ||
                       status_sensor_->get_raw_state().compare("2x Espresso selected") == 0 ||
                       (!is_bean_ && status_sensor_->get_raw_state().compare("Ground Espresso selected") == 0))) ||
                     (!is_bean_ && (source_ == HOT_WATER || source_ == ANY) &&
                      status_sensor_->get_raw_state().compare("Hot water selected") == 0) ||
                     ((source_ == CAPPUCCINO || source_ == ANY) &&
                      status_sensor_->get_raw_state().compare("Cappuccino selected") == 0)))
                {
                    if (data[is_bean_ ? 9 : 11] == 0x07)
                    {
                        switch (data[is_bean_ ? 8 : 10])
                        {
                        case 0x00:
                            update_state(1);
                            break;
                        case 0x38:
                            update_state(2);
                            break;
                        case 0x3F:
                            update_state(3);
                            break;
                        default:
                            break;
                        }

                        // press the size/bean button until the target value has been reached
                        if (target_amount_ != -1 && state != target_amount_ && millis() - last_transmission_ > SETTINGS_BUTTON_SEQUENCE_DELAY)
                        {
                            for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                                if (is_bean_)
                                    mainboard_uart_->write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x09, 0x2F});
                                else
                                    mainboard_uart_->write_array({0xD5, 0x55, 0x00, 0x01, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x20, 0x05});
                            mainboard_uart_->flush();
                            last_transmission_ = millis();
                        }

                        // Unset the target state to allow for manual control
                        if (state == target_amount_)
                        {
                            target_amount_ = -1;
                        }

                        return;
                    }
                }

                update_state(NAN);
            }

        } // namespace philips_beverage_setting
    }     // namespace philips_series_2200
} // namespace esphome