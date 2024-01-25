# Communication protocol

The underlining protocol used by this coffee machine is quite simple.
The mainboard does all the heavy lifting and the display unit is only responsible for I/O.
Upon powering on, the Display unit issues a 'power on' command and then continues to poll the machine status from the mainboard using a request message.
The mainboard replies to this request with a message containing all LED states.
If the user presses a button this this information is passed along in a message to the mainboard. Once the user releases the button the display returns to sending status requests.
If the machine is off/sleeping no messages are sent in either direction.
The messages were obtained by listening to the bus.

## Messages from the display to the mainboard

All messages have the following structure:

| Start Message | Content                                 | Checksum  |
| ------------- | --------------------------------------- | --------- |
| `D5     55`   | `00   01   02   00   02   00   00   00` | `11   36` |

The first 2 Bytes are always `D5 55`. The length of the message is not encoded but it also never changes.
The last 2 Bytes are some sort of checksum. The rule for determining this checksum is not known.

### Power on message

There seem to be 3 messages responsible for powering on the machine.

`D5 55 0A 01 02 00 02 00 00 00 0E 12`

Makes the machine beep, but does not actually power on the machine.

`D5 55 01 01 02 00 02 00 00 00 25 27`

Beeps and powers on the machine, but without a cleaning cycle during startup.

`D5 55 02 01 02 00 02 00 00 00 38 15`

Beeps, powers on the machine and includes a cycle during startup.

The last 2 Messages are used within this project.
The 3rd byte is likely responsible for turning the coffee machine on, since it's `00` in all other messages.

### Power off message

`D5 55 00 01 02 00 02 01 00 00 1D 3B`

When sent, the mainboard proceeds to shut down the machine. The display unit goes to sleep after powering off.

### Status request message

`D5 55 00 01 02 00 02 00 00 00 11 36`

### Play/Pause button message

`D5 55 00 01 02 00 02 00 00 01 19 32`

### Drink selection

Byte nr. 8 is used to transmit drink selections. The 4 least significant bits are used. The bit indices and the resulting checksums are listed below.
Button|Message|
-|-
Espresso| `D5 55 00 01 02 00 02 02 00 00 09 2D`
Hot Water|`D5 55 00 01 02 00 02 04 00 00 21 01`
coffee|`D5 55 00 01 02 00 02 08 00 00 39 1C`
steam|`D5 55 00 01 02 00 02 10 00 00 09 26`

### Settings buttons

The 9th byte is used to transmit the right hand side button group in a similar fashion.

| Button     | Message                               |
| ---------- | ------------------------------------- |
| Bean       | `D5 55 00 01 02 00 02 00 02 00 09 2F` |
| Size       | `D5 55 00 01 02 00 02 00 04 00 20 05` |
| Aqua clean | `D5 55 00 01 02 00 02 00 10 00 0D 36` |
| Calc clean | `D5 55 00 01 02 00 02 00 20 00 28 37` |

### Play/Pause button

`D5 55 00 01 02 00 02 00 00 01 19 32`

### Encoding simultaneous button presses

This should be possible but determining the correct checksum is required.

## Messages from the mainboard to the display

All messages have the following structure:

| Start Message | Content                                        | Checksum |
| ------------- | ---------------------------------------------- | -------- |
| `D5     55`   | `00 00 00 00 00 00 00 00 00 00 00 00 00 00 00` | `39 0D`  |

The structure is similar to the previous messages but the content part is longer. The message above means all LEDs on the display are turned off.
The following table show the purpose of each byte and their known states

| Byte | Purpose           | Detail                                                |
| ---- | ----------------- | ----------------------------------------------------- |
| 0    | START             |
| 1    | START             |
| 2    | INSTRUCTION       |
| 3    | Espresso-LED      | `03`/`07` - half/full brightness ; `38` - 2x espresso          |
| 4    | Hot Water-LED     | `03`/`07` - half/full brightness                               |
| 5    | Coffee-LED        | `03`/`07` - half/full brightness; `38` - 2x coffee             |
| 6    | Steam-LED         | `03`/`07` - half/full brightness                               |
| 7    |                   | unknown                                                        |
| 8    | Bean-LED          | `00` - 1 LED; `38` - 2 LEDs; `3F` - 3 LEDs                     |
| 9    | Bean-LED          | `07` - show led group; `38` - powder selected                  |
| 10   | Size-LED          | `00` - 1 LED; `38` - 2 LEDs; `3F` - 3 LEDs; `07` -  Top LED    |
| 11   | Size-LED          | `07` - show led group                                          |
| 12   |                   | probably aqua_clean/calc_clean                                 |
| 13   |                   | probably aqua_clean/calc_clean                                 |
| 14   | Waste&Warning-LED | `07` - waste; `38` - warning sign                              |
| 15   | Play/Pause-LED    | `07` - on                                                      |
| 16   | checksum          |
| 17   | checksum          |

## Off LED states

`D5 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 39 0D`

## Idle LED states

`D5 55 00 07 07 07 07 00 00 00 00 00 00 00 00 00 00 07 2B`
