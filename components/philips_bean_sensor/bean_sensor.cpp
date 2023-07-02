#include "esphome/core/log.h"
#include "bean_sensor.h"

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_bean_sensor
        {
            static const char *TAG = "philips_bean_sensor";

            void BeanSensor::setup()
            {
            }

            void BeanSensor::dump_config()
            {
                LOG_SENSOR(TAG, "Philips Bean Sensor", this);
            }

            void BeanSensor::update_status(uint8_t *data, size_t len)
            {
                // reject invalid messages
                if (len < 19 && data[0] != 0xD5 && data[1] != 0x55)
                    return;

                // only apply status if source is currently selected
                if ((source_ == COFFEE &&
                     status_sensor_->has_state() &&
                     (status_sensor_->get_raw_state().compare("Coffee selected") == 0 ||
                      status_sensor_->get_raw_state().compare("2x Coffee selected") == 0)) ||
                    (source_ == ESPRESSO &&
                     status_sensor_->has_state() &&
                     (status_sensor_->get_raw_state().compare("Espresso selected") == 0 ||
                      status_sensor_->get_raw_state().compare("2x Espresso selected") == 0)))
                {
                    if (data[9] == 0x07)
                    {
                        switch (data[8])
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
                        return;
                    }
                }

                update_state(NAN);
            }

        } // namespace philips_bean_sensor
    }     // namespace philips_series_2200
} // namespace esphome