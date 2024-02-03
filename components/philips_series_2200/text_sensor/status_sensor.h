#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

// Feel free to lower this, you might get some invalid intermittent state though
#define REPEAT_REQUIREMENT 30
#define BLINK_THRESHOLD 750

namespace esphome
{
    namespace philips_series_2200
    {
        namespace philips_status_sensor
        {

            /**
             * @brief Reports status of the coffee machine
             */
            class StatusSensor : public text_sensor::TextSensor, public Component
            {
            public:
                void setup() override;
                void dump_config() override;

                /**
                 * @brief Updates the status of this sensor based on the messages sent by the mainboard
                 */
                void update_status(uint8_t *data, size_t len);

                /**
                 * @brief Sets the status to OFF
                 */
                void set_state_off()
                {
                    if (state != "OFF")
                        publish_state("OFF");
                };

                /**
                 * @brief set the cappuccino flag which indicates if cappuccino is reported instead of steam
                 *
                 */
                void set_use_cappuccino(bool value)
                {
                    use_cappuccino_ = value;
                }

                /**
                 * @brief Published the state if it's different form the currently published state.
                 *
                 */
                void update_state(const std::string &state)
                {
                    if (state == new_state_)
                    {
                        if (new_state_counter_ >= REPEAT_REQUIREMENT)
                        {
                            if (this->state != state)
                                publish_state(state);
                        }
                        else
                        {
                            new_state_counter_++;
                        }
                    }
                    else
                    {
                        new_state_counter_ = 0;
                        new_state_ = state;
                    }
                }

            private:
                /// @brief counter which count how often a message has been seen
                int new_state_counter_ = 0;

                /// @brief cache for counting new messages
                std::string new_state_ = "";

                /// @brief status of the play/pause led
                bool play_pause_led_ = false;

                /// @brief indicates if cappuccino should be reported instead of steam
                bool use_cappuccino_ = false;

                /// @brief time of play/pause change
                long play_pause_last_change_ = 0;
            };
        } // namespace philips_status_sensor
    }     // namespace philips_series_2200
} // namespace esphome