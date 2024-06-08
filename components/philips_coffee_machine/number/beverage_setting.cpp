#include "esphome/core/log.h"
#include "beverage_setting.h"

namespace esphome
{
    namespace philips_coffee_machine
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

            void BeverageSetting::update_status(uint8_t *data)
            {

                if (!status_sensor_->has_state())
                    return;

                // only apply status if source is currently selected
                std::string status = status_sensor_->get_raw_state();
                if ((type_ != MILK && (source_ == COFFEE || source_ == ANY) &&
                     (status.compare(state_coffee_selected) == 0 ||
                      status.compare(state_coffee_2x_selected) == 0 ||
                      (type_ != BEAN && status.compare(state_ground_coffee_selected) == 0))) ||
                    (type_ != MILK && (source_ == ESPRESSO || source_ == ANY) &&
                     (status.compare(state_espresso_selected) == 0 ||
                      status.compare(state_espresso_2x_selected) == 0 ||
                      (type_ != BEAN && status.compare(state_ground_espresso_selected) == 0))) ||
                    (type_ != MILK && (source_ == AMERICANO || source_ == ANY) &&
                     (status.compare(state_americano_selected) == 0 ||
                      status.compare(state_americano_2x_selected) == 0 ||
                      (type_ != BEAN && status.compare(state_ground_americano_selected) == 0))) ||
                    ((source_ == CAPPUCCINO || source_ == ANY) &&
                     (status.compare(state_cappuccino_selected) == 0 ||
                      (type_ != BEAN && status.compare(state_ground_cappuccino_selected) == 0))) ||
                    ((source_ == LATTE_MACCHIATO || source_ == ANY) &&
                     (status.compare(state_latte_selected) == 0 ||
                      (type_ != BEAN && status.compare(state_ground_latte_selected) == 0))) ||
                    (type_ != BEAN && type_ != MILK && (source_ == HOT_WATER || source_ == ANY) &&
                     status.compare(state_hot_water_selected) == 0))
                {
                    uint8_t enable_byte = type_ == BEAN ? 9 : 11;
                    uint8_t amount_byte = type_ == BEAN ? 8 : (type_ == SIZE ? 10 : 13);

                    if (data[enable_byte] == led_on)
                    {
                        switch (data[amount_byte])
                        {
                        case led_off:
                            update_state(1);
                            break;
                        case led_second:
                            update_state(2);
                            break;
                        case led_third:
                            update_state(3);
                            break;
                        default:
                            break;
                        }

                        // press the size/bean button until the target value has been reached
                        if (target_amount_ != -1 && state != target_amount_ && millis() - last_transmission_ > SETTINGS_BUTTON_SEQUENCE_DELAY)
                        {
                            for (unsigned int i = 0; i <= MESSAGE_REPETITIONS; i++)
                            {
                                switch (type_)
                                {
                                case BEAN:
                                    mainboard_uart_->write_array(command_press_bean);
                                    break;
                                case SIZE:
                                    mainboard_uart_->write_array(command_press_size);
                                    break;
#ifdef PHILIPS_EP3243
                                case MILK:
                                    mainboard_uart_->write_array(command_press_milk);
                                    break;
#endif
                                default:
                                    break;
                                }
                            }

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
    }     // namespace philips_coffee_machine
} // namespace esphome