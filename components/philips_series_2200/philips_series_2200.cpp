#include "esphome/core/log.h"
#include "philips_series_2200.h"

namespace esphome
{
    namespace philips_series_2200
    {

        static const char *TAG = "philips_series_2200";

        void PhilipsSeries2200::setup()
        {
        }

        void PhilipsSeries2200::loop()
        {

        }

        void PhilipsSeries2200::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Philips Series 2200");
            // TODO: add uart config
        }

    } // namespace philips_series_2200
} // namespace esphome