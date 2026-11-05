# Firmware

Each revision is an independent Arduino sketch. Open only the `.ino` file in
the matching directory so Arduino does not combine revisions.

## Version overview

### 1.00 — original implementation

Path: `../firmware/traffic_light_1_00/traffic_light_1_00.ino`

The original counter-based firmware uses P0 green, P1 yellow, and P4 red. Its
inclusive condition executes 11 normal cycles (counter 0 through 10), then 10
yellow flashes (counter 11 through 20), and rapidly counts to 10,001 before
holding the outputs off. It also contains disabled battery-monitor code. This
version is preserved as historical source, not as the recommended firmware.

### 1.01 — corrected sequence

Path: `../firmware/traffic_light_1_01/traffic_light_1_01.ino`

Version 1.01 retains P0 green, P1 yellow, and P4 red. It performs exactly ten
normal cycles and ten final yellow flashes, removes the unnecessary terminal
count, initializes outputs explicitly, and switches all external lights off
before selecting the next one.

### 1.02 — independent diagnostics

Path: `../firmware/traffic_light_1_02/traffic_light_1_02.ino`

Version 1.02 moves the external yellow LED from P1 to P2 and assigns P1 to the
onboard diagnostic LED. Diagnostic timing is integrated into 200 ms intervals
so it does not extend traffic-light phases. This version was compiled and
tested successfully on the user's physical board.

### 1.03 — permanent Power-down

Path: `../firmware/traffic_light_1_03/traffic_light_1_03.ino`

Version 1.03 preserves the 1.02 sequence and diagnostics. After final shutdown
it disables the watchdog, ADC, analog comparator, timers, USI, PLL, and unused
interrupt sources, then repeatedly enters `SLEEP_MODE_PWR_DOWN` with BOD
disabled during sleep. An unexpected wake returns directly to sleep rather
than restarting the application.

## Pin mapping by version

| Version | P0 | P1 | P2 | P4 |
| --- | --- | --- | --- | --- |
| 1.00 | Green | External yellow and onboard LED share the pin | Unused by traffic lights | Red |
| 1.01 | Green | External yellow and onboard LED share the pin | Unused | Red |
| 1.02 | Green | Onboard diagnostic LED | External yellow | Red |
| 1.03 | Green | Onboard diagnostic LED | External yellow | Red |

All external traffic-light outputs are active HIGH. The 1.02/1.03 diagnostic
polarity is explicit and configured active HIGH for the tested board.

## Exact sequence and timing

For versions 1.01, 1.02, and 1.03:

| Stage | Count | Timing | Total |
| --- | ---: | --- | ---: |
| Initial test | 10 rapid green/yellow/red/off sets | 25 ms per state plus 1 s final pause | 2 s |
| Normal sequence | 10 cycles | 20 s green + 5 s yellow + 20 s red | 450 s |
| Final warning | 10 flashes | 1 s yellow ON + 1 s OFF | 20 s |
| **Nominal runtime** |  |  | **472 s** |

The 472-second value is verified from source constants and full host
simulation (472,000 ms). It has not been independently measured with a
stopwatch on physical hardware.

## Diagnostic patterns (1.02 and 1.03)

Patterns repeat every 3 seconds and use 200 ms ON pulses separated by 200 ms:

| Traffic-light state | Diagnostic pulses |
| --- | ---: |
| Green | 1 |
| Yellow | 2 |
| Red | 3 |
| Final yellow warning | Yellow pattern (2) |

The initial test alternates the diagnostic LED rapidly to remain visually
distinct. Diagnostic updates share the phase's fixed 200 ms intervals and do
not add runtime.

## Shutdown, reset, and power cycle

Version 1.01 and 1.02 leave all controlled LEDs off in an empty `loop()` until
reset. Version 1.03 enters permanent Power-down after setting P0, P1, P2, and
P4 to their OFF levels. Reset or a complete power cycle starts Micronucleus and
then the firmware from the initial test. A normal upload remains dependent on
connecting the board during the bootloader window after power-up.

## Validation and resource use

| Version | Validation |
| --- | --- |
| 1.00 | Source reviewed; historical behavior identified |
| 1.01 | Source and counts reviewed |
| 1.02 | PlatformIO build and physical-board functional test passed |
| 1.03 | Simulation, sleep-order static check, PlatformIO and Arduino IDE builds, Micronucleus upload, and physical-board functional tests passed |

Verified PlatformIO results at 16.5 MHz:

| Version | Flash | RAM |
| --- | ---: | ---: |
| 1.02 | 1,076 / 6,012 bytes (17.9%) | 9 / 512 bytes (1.8%) |
| 1.03 | 1,156 / 6,012 bytes (19.2%) | 9 / 512 bytes (1.8%) |

## Limitations

- `delay()` timing depends on the selected clock/core and is nominal rather
  than a calibrated real-time measurement.
- No battery sensing is active in 1.01 through 1.03.
- Version 1.03 final sleep behavior was observed functionally on hardware, but
  board current was not measured.
- The nominal 472-second runtime was not measured with an independent timer.
- The complete board continues to power components outside the ATtiny85.
- The historical Digistump core and Micronucleus host tools may require
  platform-specific validation on modern systems.

See [Hardware](hardware.md), [Installation](installation.md), and
[Troubleshooting](troubleshooting.md).
