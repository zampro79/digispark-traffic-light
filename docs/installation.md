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
- A local clone of this repository, with one sketch opened from its matching
  folder, for example
  `firmware/traffic_light_1_03/traffic_light_1_03.ino`.

Firmware 1.03 was confirmed on physical hardware with the Arduino IDE clock
option set to **16.5 MHz - For V-USB**. This is the confirmed configuration for
the successful hardware test. It is not a universal requirement imposed by the
Micronucleus bootloader on every Digispark application.

The full reference FQBN exposed by the currently installed tools is:

```text
digistump:avr:digispark-tiny:clock=clock165,micronucleus=default,burnmode=upgrade
```

The board and `clock165` selection match the confirmed hardware test. The
`micronucleus=default` and `burnmode=upgrade` options were detected in the
current local environment, but have not been independently confirmed as the
complete historical configuration used for that test. Menu names and available
options may vary between core releases. Do not change or replace the bootloader
solely to reproduce these menu selections.

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

## Current local environment and historical scope

The following environment was observed on the current Fedora Linux 44 x86-64
computer during a read-only inventory:

| Component | Currently installed version |
| --- | --- |
| Arduino IDE | 2.3.10 |
| Arduino CLI bundled with the IDE | 1.5.1 |
| Digistump AVR Boards | 1.7.5 |
| Board identifier | `digistump:avr:digispark-tiny` |
| Micronucleus package | 2.6 |
| Micronucleus executable self-reported version | 2.5 |

The Micronucleus package directory and package metadata identify version 2.6,
while the installed executable reports `micronucleus-cli` 2.5. Both values are
recorded rather than treating either one as the unambiguous uploader version.
The bootloader version installed on the physical Digispark has not been
identified.

These are the tools installed at the time of the inventory. They must not be
assumed to be the exact historical tool versions used for the successful 1.03
build and upload. The confirmed historical facts are the Digispark ATtiny85
target, the 16.5 MHz - For V-USB clock selection, a successful Arduino IDE and
Micronucleus upload, and successful operation on the physical hardware. Exact
historical Arduino IDE, core, CLI, uploader, and bootloader versions remain
unverified.

## Compile and upload with Arduino IDE

1. Clone or download this repository, then keep its directory structure intact.
2. In Arduino IDE, open
   `firmware/traffic_light_1_03/traffic_light_1_03.ino` from the local clone.
   Each sketch folder contains one main file with the same base name as the
   folder.
3. Select the Digispark ATtiny85 target and **16.5 MHz - For V-USB**, the clock
   configuration confirmed by the firmware 1.03 hardware test.
4. Leave the Micronucleus variant and bootloader method consistent with the
   bootloader already present on the board. The current environment displays
   `Default` and `Upgrade (via USB)`, but these two selections are not recorded
   as confirmed historical requirements.
5. Use **Sketch > Verify/Compile** to build without touching the board.
6. For upload, disconnect battery power and unplug the Digispark from USB.
7. Start **Upload** and connect the Digispark only when Micronucleus asks for
   it. Its short bootloader window starts at power-up.
8. Treat the uploader's successful completion message as upload confirmation;
   then perform the functional checks in [Firmware](firmware.md).

Do not power the board from USB and an undocumented battery path at the same
time. This project does not require fuse changes or bootloader replacement.

## Verified Arduino build

Firmware 1.03 was compiled successfully without upload on Fedora Linux 44
x86-64 using the following installed environment:

| Component | Verified build value |
| --- | --- |
| Arduino IDE AppImage | 2.3.10 |
| Bundled Arduino CLI | 1.5.1 |
| Digistump AVR Boards | 1.7.5 |
| AVR compiler | avr-g++ GCC 7.3.0-atmel3.6.1-arduino7 |
| MCU | ATtiny85 |
| CPU frequency | `F_CPU=16500000L` |
| Requested FQBN | `digistump:avr:digispark-tiny:clock=clock165,micronucleus=default,burnmode=upgrade` |
| Flash | 1,156 / 6,650 bytes (about 17%) |
| Global RAM | 9 / 512 bytes (about 1.8%) |

The bundled CLI was extracted temporarily from the Arduino IDE AppImage, with
no additional package installation. All build artifacts were generated outside
the repository. This was a compile-only verification: it did not upload the
firmware, repeat the hardware test, or verify the procedure from a completely
clean clone.

The 6,650-byte flash limit above comes from the Arduino build with the current
`micronucleus=default` board option. It is distinct from the historical
PlatformIO result below, which used a 6,012-byte flash limit.

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
The current read-only environment inventory was performed on Fedora Linux 44
x86-64, but this does not establish the Fedora release used for the historical
firmware 1.03 hardware test.
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
`digispark-tiny`. A historical local verification used an existing, separately
configured PlatformIO project and the following command:

```sh
pio run -e digispark-tiny
```

The successful local verification used ATtiny85, Digistump framework 1.7.2,
and `F_CPU=16500000L`. It produced a firmware size of 1,156 of 6,012 flash bytes
and 9 of 512 RAM bytes.

No `platformio.ini` or PlatformIO source-layout configuration is stored in this
repository. Consequently, `pio run -e digispark-tiny` is a record of the
historical verification, not a command that can be run directly from a clean
clone. Reproducing this workflow requires a reviewed project configuration that
selects `firmware/traffic_light_1_03` as its source, pins the intended platform
and framework versions, and explicitly sets `F_CPU=16500000L`. Such a
configuration must be tested from a clean clone before it is documented as a
supported procedure.

PlatformIO's published board page reports `digispark-tiny` but may show a
different default frequency. Explicitly inspect the effective build flags; do
not infer 16.5 MHz solely from the board ID. Do not infer successful upload or
hardware compatibility from a successful PlatformIO compilation alone.

## Sources and compatibility scope

- [Arduino IDE 2 installation guide](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing)
- [Maintained DigistumpArduino repository](https://github.com/ArminJo/DigistumpArduino)
- [PlatformIO Digispark USB board reference](https://docs.platformio.org/en/stable/boards/atmelavr/digispark-tiny.html)
- [Micronucleus project](https://github.com/micronucleus/micronucleus)

These links identify authoritative or maintained project information. Windows
and macOS instructions above remain unverified on physical hosts for this
repository.

See [Hardware](hardware.md) and [Troubleshooting](troubleshooting.md).
