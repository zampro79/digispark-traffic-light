# Installation and build setup

Arduino IDE 2 is the primary workflow. PlatformIO is an optional, separately
verified build path. The Digistump ecosystem is historical: confirm tool and
host compatibility before depending on it on a new operating-system release.

## Common prerequisites

- A classic USB Digispark ATtiny85 with Micronucleus bootloader.
- Arduino IDE 2.x.
- A compatible Digistump AVR Boards package that supplies the Digispark board
  definition and Micronucleus uploader.
- A data-capable USB connection.
- One sketch opened from its matching folder, for example
  `firmware/traffic_light_1_03/traffic_light_1_03.ino`.

Select the Digispark ATtiny85 board, the project's tested **16.5 MHz** clock
configuration, and the Micronucleus variant already present on the board. The
locally tested Arduino FQBN is `digistump:avr:digispark-tiny`. Menu wording
varies between core releases. The 16.5 MHz setting documents the configuration
used to build and test this firmware; it is not a universal requirement
imposed by the Micronucleus bootloader on every Digispark application.

The maintained Digistump package index is published by the ArminJo
DigistumpArduino project, but that project labels the core deprecated and
unsupported. Its cross-platform package contents do not guarantee operation on
every modern host. Record the exact IDE, core, uploader, OS, and architecture
when reporting a result.

Add this maintained package-index URL in the Arduino IDE additional Boards
Manager URLs setting:

```text
https://raw.githubusercontent.com/ArminJo/DigistumpArduino/master/package_digistump_index.json
```

## Compile and upload with Arduino IDE

1. Open the desired `.ino` file directly; each sketch folder contains one main
   file with the same base name as the folder.
2. Select the Digispark target and the tested 16.5 MHz clock configuration.
3. Use **Sketch > Verify/Compile** to build without touching the board.
4. For upload, disconnect battery power and unplug the Digispark from USB.
5. Start **Upload** and connect the Digispark only when Micronucleus asks for
   it. Its short bootloader window starts at power-up.
6. Treat the uploader's successful completion message as upload confirmation;
   then perform the functional checks in [Firmware](firmware.md).

Do not power the board from USB and an undocumented battery path at the same
time. This project does not require fuse changes or bootloader replacement.

## Linux

Arduino supplies IDE 2 as a 64-bit AppImage for Linux; distribution packaging
may also exist but is maintained independently. Make the AppImage executable.
Some distributions require FUSE support to run it.

Micronucleus access normally depends on a suitable udev rule for the device.
Use the rule supplied by the installed board/uploader package or its maintained
upstream instructions, reload udev rules, then reconnect the board. Verify USB
detection with normal system tools before retrying Arduino IDE. Avoid broad,
permanent world-writable USB rules when a narrower maintained rule is
available.

Common Linux failures are an absent udev rule, a stale rule not yet reloaded,
the board being connected before the uploader prompt, a charge-only cable, or
missing host libraries required by the packaged uploader.

### Fedora notes

The project's Arduino IDE workflow has been exercised on Fedora Linux.
The Arduino AppImage may need Fedora's FUSE compatibility package. Package
names and policy change by Fedora release, so follow current Fedora and Arduino
documentation rather than copying a command intended for another distribution.
After installing a udev rule, reconnect the board (or reload rules) and ensure
the current login session has the required device access.

## Windows

Install Arduino IDE 2 using Arduino's Windows installer, then add a compatible
Digistump AVR Boards package through Boards Manager using its maintained
package index. Micronucleus devices may require the Digispark USB driver
provided by that package or maintained upstream.

Install only a driver from a trusted project/package source. Reconnect the
board and inspect Device Manager if it is not detected. A warning icon,
unknown USB device, or device that appears only briefly usually points to the
driver, cable, port, or bootloader timing. Do not use fuse changes or a
bootloader rewrite as the first response.

Current behavior has not been tested for this repository on Windows. Driver
installation and modern Windows signing behavior must therefore be considered
platform validation tasks, not guaranteed steps.

## macOS

Install Arduino IDE 2 by copying the application to Applications. Choose the
IDE build appropriate to the Mac and follow current Arduino requirements.

The available Digistump package has historically included an Intel macOS
Micronucleus binary. Native Apple Silicon compatibility is not established by
this repository; Rosetta or a separately maintained uploader may be necessary,
but neither route has been verified here. macOS may ask the user to approve
software from an identified developer or removable-device access. Follow the
normal system prompts and maintained tool guidance; do not disable system
security protections as a default workaround.

Current Digistump/Micronucleus operation on recent Intel and Apple Silicon
macOS versions is unverified for this project.

## Optional PlatformIO workflow

PlatformIO Core with the Atmel AVR platform recognizes the board ID
`digispark-tiny`. In an existing PlatformIO project configured for this
repository, the verified build command was:

```sh
pio run -e digispark-tiny
```

The successful local verification used ATtiny85, Digistump framework 1.7.2,
and `F_CPU=16500000L`. No `platformio.ini` is stored in this repository, so the
following is illustrative and must be reviewed before use:

```ini
[env:digispark-tiny]
platform = atmelavr
board = digispark-tiny
framework = arduino
board_build.f_cpu = 16500000L
```

PlatformIO's published board page reports `digispark-tiny` but may show a
different default frequency. Explicitly inspect the effective build flags; do
not infer 16.5 MHz solely from the board ID. The local 1.03 build used 1,156 of
6,012 flash bytes and 9 of 512 RAM bytes.

## Sources and compatibility scope

- [Arduino IDE 2 installation guide](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing)
- [Maintained DigistumpArduino repository](https://github.com/ArminJo/DigistumpArduino)
- [PlatformIO Digispark USB board reference](https://docs.platformio.org/en/stable/boards/atmelavr/digispark-tiny.html)
- [Micronucleus project](https://github.com/micronucleus/micronucleus)

These links identify authoritative or maintained project information. Windows
and macOS instructions above remain unverified on physical hosts for this
repository.

See [Hardware](hardware.md) and [Troubleshooting](troubleshooting.md).
