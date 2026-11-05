# Troubleshooting

Start with the least invasive checks. Do not change fuse bits or replace the
bootloader as routine troubleshooting.

## Digistump board package is not visible

- Confirm that the compatible package-index URL was entered in Arduino IDE
  preferences without transcription errors.
- Refresh Boards Manager, search for Digistump AVR Boards, and record the
  installed core version.
- Restart the IDE after installation if menus remain stale.
- Remember that the historical core is deprecated; availability does not
  guarantee compatibility with every current OS.

## Wrong board or clock selection

Select the Digispark ATtiny85 target and the project's tested **16.5 MHz**
clock configuration. A different application clock can change the firmware's
timing. The documented setting reproduces the tested build; it is not a
universal Micronucleus requirement.

## Compilation errors

- Open the sketch from its matching folder; folder and `.ino` base names must
  match.
- Ensure only that version's main `.ino` is in the sketch folder.
- Capture the first compiler error, IDE version, board package version, board,
  clock, and host OS.
- For 1.03, verify that the selected AVR toolchain provides `avr/interrupt.h`,
  `avr/power.h`, `avr/sleep.h`, and `avr/wdt.h`.
- Do not install random libraries: these sketches have no external dependency.

## Micronucleus device is not detected or upload times out

1. Disconnect the board.
2. Start upload.
3. Connect the Digispark only when the uploader requests it.
4. Try a known data-capable cable and a direct USB port.
5. Remove battery power and unintended external supply paths.
6. Temporarily disconnect external loads, especially P4, to diagnose USB-line
   loading.
7. Inspect OS USB/device logs and the full uploader message.

The Micronucleus device normally appears only during its power-up bootloader
window. A timeout can therefore be procedural even when the firmware is valid.

## Linux USB permission errors

- Confirm that the device is visible to the kernel.
- Install the maintained Micronucleus/Digispark udev rule appropriate to the
  device, reload udev rules, and reconnect it.
- Check rule matching and current-session permissions.
- Prefer a specific maintained rule over making all USB devices writable.

## Windows driver problems

- Inspect Device Manager while reconnecting the board during the upload
  window.
- Use the Digispark/Micronucleus driver supplied by the trusted board package
  or maintained upstream project.
- Check driver status, cable, port, and architecture before retrying.
- Current Windows driver/signing behavior is unverified for this repository.

## macOS compatibility problems

- Confirm IDE and uploader architecture (Intel versus Apple Silicon).
- Check normal macOS approval prompts and USB/removable-device permissions.
- The packaged Micronucleus executable may be Intel-only; native Apple Silicon
  operation is unverified here.
- Use maintained project guidance for any compatibility layer. Do not disable
  macOS security protections as a default solution.

## Incorrect LED mapping

Firmware 1.00/1.01 use P1 for yellow. Firmware 1.02/1.03 use P2 for external
yellow and P1 for diagnostics. Verify the loaded version, resistor paths,
common ground, LED polarity, and connector labels before changing code.

## Diagnostic LED behaves incorrectly

- Confirm P1 is not also connected to the external yellow LED.
- Verify the board revision's onboard LED polarity. The tested board is active
  HIGH, but clones may differ.
- Expected patterns are one pulse for green, two for yellow, and three for red;
  pulses are 200 ms and the pattern period is 3 seconds.
- Confirm the 16.5 MHz clock selection if timing appears wrong.

## Firmware does not restart after Power-down

That is expected for 1.03. Application execution remains asleep after the
472-second sequence. Press reset or fully remove and restore power. To upload
new firmware, unplug the board, start upload, and reconnect it when requested
so Micronucleus runs during power-up.

If reset or a new upload does not work, remove external supply paths and loads,
then retry with USB only. Do not assume that sleep modified the bootloader or
fuses; 1.03 does neither.

## Battery power and brown-out symptoms

Symptoms may include resets, a repeated initial test, dim or unstable LEDs,
wrong timing perception, or failed USB detection when supplies are combined.
Before testing, document battery chemistry, measured voltage under load,
connection point (VIN or regulated rail), polarity, and protection. Use one
known source, check connections with power removed, and never place an
ammeter directly across the battery.

No battery specification or current measurement is currently recorded, so do
not infer a safe battery configuration from this repository.

See [Installation](installation.md), [Hardware](hardware.md), and
[Firmware](firmware.md).
