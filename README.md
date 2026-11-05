# Digispark Traffic Light

An Arduino traffic-light controller for the classic USB Digispark board with
an ATtiny85. Each firmware revision is kept in a separate Arduino sketch
folder. Version 1.03 is the recommended revision.

> **Educational use only:** This is an educational LED project and must not be
> used to control real traffic or safety-critical equipment.

## Project Background

This little traffic-light project was originally built during the COVID-19
pandemic as a playful electronics gadget for my son.

Years later, I revisited the original Digispark firmware, fixed a few issues,
added visual diagnostics and power-saving features, and improved the
documentation so others could build and enjoy it too.

## Features

- Three active-HIGH external lights with break-before-make switching.
- Ten timed green/yellow/red cycles followed by ten yellow warning flashes.
- Independent onboard diagnostic patterns from version 1.02 onward.
- Permanent ATtiny85 Power-down after the sequence in version 1.03.
- No external libraries.

## Hardware and pins

The confirmed firmware 1.03 setup is powered from USB at 5 V. The three
external LEDs are driven directly by the GPIO pins without transistors. Each
external LED has its own 330 ohm current-limiting resistor and shares the board
ground.

| Digispark pin | Function | Series resistor |
| --- | --- | ---: |
| P0 | External green LED | 330 ohm |
| P1 | Onboard diagnostic LED | Onboard circuit |
| P2 | External yellow LED | 330 ohm |
| P4 | External red LED | 330 ohm |

The external LED models and actual currents have not been documented or
measured. Firmware 1.00 and 1.01 instead use P1 for the external yellow LED and
have no independent diagnostic LED. See [Hardware](docs/hardware.md) before
wiring the board.

## Operating sequence

Versions 1.01 through 1.03 perform:

1. A 2-second initial test.
2. Ten 45-second cycles: green for 20 seconds, yellow for 5 seconds, and red
   for 20 seconds.
3. Ten final yellow flashes: 1 second ON and 1 second OFF.
4. All controlled LEDs OFF.

The nominal runtime is **472 seconds**. It is derived from the source and was
verified in simulation, but has not been independently measured with a
stopwatch on the physical board.

During normal operation, firmware 1.02 and 1.03 repeat a diagnostic pattern
every 3 seconds: one 200 ms pulse for green, two for yellow, and three for red.
The final yellow-flash phase uses the two-pulse diagnostic pattern without
changing the external 1-second ON/OFF timing.

After final shutdown, version 1.03 disables unused ATtiny85 peripherals and
repeatedly enters `SLEEP_MODE_PWR_DOWN`. Normal execution resumes only after
reset or a power cycle; an unexpected non-reset wake does not restart the
traffic-light sequence.

## Firmware versions

| Version | Summary | Status |
| --- | --- | --- |
| 1.00 | Original counter-based implementation | Preserved for reference |
| 1.01 | Corrected counts and simplified output logic | Source/simulation verified |
| 1.02 | Yellow moved to P2; independent P1 diagnostics | Built and hardware-tested |
| 1.03 | Adds permanent Power-down after shutdown | Built, simulated, and hardware-tested |

See [Firmware](docs/firmware.md) for exact differences, timings, build figures,
and validation scope.

## Quick start with Arduino IDE 2

1. Install Arduino IDE 2 and a compatible Digistump AVR Boards package.
2. Open `firmware/traffic_light_1_03/traffic_light_1_03.ino` directly.
3. Select the Digispark ATtiny85 target and the project's tested **16.5 MHz**
   clock configuration.
4. Compile with **Sketch > Verify/Compile**.
5. To upload with Micronucleus, leave the Digispark unplugged, start the
   upload, and connect it only when the uploader requests it.

Package setup, OS-specific USB requirements, and the optional PlatformIO
workflow are documented in [Installation](docs/installation.md). Consult
[Troubleshooting](docs/troubleshooting.md) before changing drivers or settings.

## Validation status

- Version 1.02 was compiled for Digispark at 16.5 MHz and tested on real
  hardware: sequence, diagnostic patterns, counts, and final shutdown passed.
- Version 1.03 was compiled with PlatformIO using Digistump 1.7.2, simulated
  for the full 472,000 ms timeline, and statically checked for sleep ordering.
- Version 1.03 was also compiled with Arduino IDE and uploaded through
  Micronucleus. On the physical board, all ten cycles, the 1/2/3-pulse
  diagnostics, ten final yellow flashes, final LED shutdown, absence of a
  spontaneous restart, reset and power-cycle restart, and a subsequent upload
  were verified successfully.
- The nominal 472-second runtime has not been independently timed, and
  board-level current has not been measured.

## Known limitations

- The historical Digistump AVR core is deprecated and current compatibility
  varies by operating system and host architecture.
- P4 is associated with the classic Digispark USB circuit, so attached loads
  may affect bootloader communication.
- Onboard LED polarity varies between board revisions; the tested board is
  active HIGH and the firmware exposes a polarity constant.
- Power-down reduces ATtiny85 activity, not the current drawn by the complete
  board's regulator, power LED, USB network, or clone-specific components.
- The tested setup uses USB power at 5 V. Battery operation, chemistry, voltage,
  current capability, and protection are not specified or verified.

## Documentation

- [Hardware and wiring](docs/hardware.md)
- [Installation and build setup](docs/installation.md)
- [Firmware versions and timing](docs/firmware.md)
- [Troubleshooting](docs/troubleshooting.md)

## License

This project is licensed under the [MIT License](LICENSE).
