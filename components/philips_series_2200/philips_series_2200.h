#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome
{
    namespace philips_series_2200
    {

        class PhilipsSeries2200 : public Component
        {
        public:
            void setup() override;
            void loop() override;
            void dump_config() override;
        };

    } // namespace philips_series_2200
} // namespace esphome
