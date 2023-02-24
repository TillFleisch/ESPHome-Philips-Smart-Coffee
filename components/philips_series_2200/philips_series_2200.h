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

            /**
             * @brief Set the reference to the uart port connected to the display
             *
             * @param pin display uart reference
             */
            void register_display_uart(uart::UARTComponent *uart) { display_uart_ = uart::UARTDevice(uart); };

            /**
             * @brief Set the reference to the uart port connected to the Mainboard
             *
             * @param pin Mainboard uart reference
             */
            void register_mainboard_uart(uart::UARTComponent *uart) { mainboard_uart_ = uart::UARTDevice(uart); };

        private:
            /// @brief reference to uart connected to the display unit
            uart::UARTDevice display_uart_;
            /// @brief reference to uart connected to the mainboard
            uart::UARTDevice mainboard_uart_;
        };

    } // namespace philips_series_2200
} // namespace esphome
