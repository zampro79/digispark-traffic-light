# Hardware

## Supported board

The project targets a classic USB Digispark with an ATtiny85 and Micronucleus
bootloader. Clone layouts and component values vary, so identify the board
revision and verify its schematic before relying on board-level details.

This is a hobby electronics gadget for education and experimentation. It is
not suitable for real traffic control or any safety-critical application.

## Current wiring (firmware 1.02 and 1.03)

| Pin | Connection | Active state |
| --- | --- | --- |
| P0 | External green LED | `HIGH` |
| P1 | Onboard diagnostic LED | Configurable; `HIGH` on the tested board |
| P2 | External yellow LED | `HIGH` |
| P4 | External red LED | `HIGH` |

Connect every external LED through a suitable current-limiting resistor and
use a common ground. The repository does not specify resistor values because
LED characteristics, supply voltage, and board wiring have not been fully
documented.

Do not exceed either the per-pin output-current limits or the total GPIO and
device current limits. Consult the ATtiny85 datasheet and the schematic for the
specific Digispark revision before choosing resistor values or adding loads.

Firmware 1.00 and 1.01 use P1 for the external yellow LED. Starting with 1.02,
the yellow wire must move from P1 to P2, leaving P1 exclusively for the onboard
diagnostic LED.

## Hardware validation of firmware 1.03

The author compiled 1.03 with Arduino IDE and uploaded it successfully through
Micronucleus. Testing on the physical Digispark confirmed ten normal cycles,
the one/two/three-pulse diagnostic patterns, ten final yellow flashes, and all
controlled LEDs switching off. No spontaneous restart was observed. Reset and
power cycling restarted the complete sequence, and a subsequent firmware
upload remained possible.

Actual current consumption and the nominal 472-second runtime have not been
independently measured.

## Output and LED polarity

The three external outputs are active HIGH. Before selecting a new traffic
light, current firmware turns all three external outputs LOW to avoid overlap.

Onboard LEDs are not wired identically on all Digispark revisions. The tested
board's P1 LED is active HIGH. Firmware 1.02 and 1.03 declare this explicitly
with `LED_DIAGNOSTICO_ATTIVO_HIGH`; verify the physical board before changing
that constant. The source uses Italian identifiers for historical continuity.

## USB-related pin considerations

P4 participates in the USB circuitry on many classic Digispark designs. An
external red LED and resistor on P4 may load the line during bootloader
enumeration or upload. If Micronucleus is not detected, disconnect external
loads for diagnosis and verify wiring; do not treat fuse or bootloader changes
as routine troubleshooting.

## Battery power

Battery chemistry, voltage, connector polarity, protection, and whether power
enters through VIN or the regulated rail are not specified. Determine all of
these before connecting a battery. Observe the voltage limits of the exact
board revision and ATtiny85, and never assume that simultaneous USB and battery
power is protected against backfeed.

Use only one known supply path during initial checks. Confirm polarity with a
meter before connection, use current limiting where available, and inspect for
unexpected heating. Do not connect an ammeter directly across a supply.

## Power-down and board consumption

Firmware 1.03 drives P0, P1, P2, and P4 to their OFF levels, leaves them as
outputs, disables unused ATtiny85 peripherals, and enters Power-down. This
reduces microcontroller activity but does not switch off the rest of the board.
Residual whole-board current may include:

- the power indicator LED;
- regulator quiescent current;
- USB pull-up, protection, and related circuitry;
- components specific to a clone or board revision.

No current has been measured for this project. A board-input measurement
reports total Digispark current, not the isolated ATtiny85 current. Measuring
the microcontroller alone would require a different test arrangement and is
outside the current non-invasive validation scope.

## Safe current-measurement outline

Use the same verified voltage and board input for comparisons. With all power
disconnected, move the meter lead to the current input, select the highest
appropriate current range, and insert the meter **in series** between source
and board. Then energize the circuit and step down ranges only when safe.
Disconnect USB to avoid an unintended second supply path.

Record voltage at the board, current, supply input, firmware version, operating
state, USB connection state, and meter range. Compare 1.02 and 1.03 under the
same conditions after their final LED shutdown.

## Known hardware limitations and pending checks

- Exact Digispark/clone model and schematic revision are unconfirmed.
- External resistor values and LED currents are unrecorded.
- Battery chemistry, voltage range, polarity, and protection are unrecorded.
- P1 polarity is verified only on the user's current board.
- P4 worked during the reported upload tests, but compatibility with other
  Digispark revisions and external loads remains unverified.
- Version 1.03 board current and independently measured timing remain
  unverified.

See [Installation](installation.md), [Firmware](firmware.md), and
[Troubleshooting](troubleshooting.md).
