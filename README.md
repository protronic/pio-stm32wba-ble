# pio-stm32wba-ble

PlatformIO project demonstrating BLE on the **STM32WBA5MMG** module
(eval board **B-WBA5M-WPAN**) using the
[STM32duinoBLE](https://github.com/protronic/STM32duinoBLE/tree/feature/stm32wba-transport)
WBA transport backend.

## Hardware

| Item | Details |
|------|---------|
| Board | [B-WBA5M-WPAN](https://www.st.com/en/evaluation-tools/b-wba5m-wpan.html) |
| MCU | STM32WBA5MMG (Cortex-M33, single-core) |
| BLE stack | STM32CubeWBA (linked library on M33) |

## Prerequisites

1. **PlatformIO** (VS Code extension or CLI)
2. **Arduino_Core_STM32** with STM32WBA5MMG / B-WBA5M-WPAN variant support
3. **STM32CubeWBA BLE middleware** installed as part of the core
   (`ble_stack.h`, `ll_intf.h`, `ll_sys.h`)

## Quick start

```bash
git clone https://github.com/protronic/pio-stm32wba-ble.git
cd pio-stm32wba-ble
pio run -e B_WBA5M_WPAN        # build
pio run -e B_WBA5M_WPAN -t upload  # flash via ST-Link
pio device monitor              # open serial monitor
```

## BLE service

| | UUID |
|---|---|
| Service | `19B10000-E8F2-537E-4F6C-D104768A1214` |
| Characteristic | `19B10001-E8F2-537E-4F6C-D104768A1214` |

Connect with **nRF Connect**, **LightBlue** or similar app,  
write `0x01` to turn the LED on, `0x00` to turn it off.

## Architecture note

STM32WBA is **single-core** (Cortex-M33). Unlike STM32WB (dual-core with
IPCC mailbox), the BLE stack runs as a linked library on the same core.
The `HCIWBATransport` backend handles initialisation, clock setup (HSE + LSE)
and HCI packet routing directly via CubeWBA API calls — no SHCI/IPCC needed.

## Related

- [protronic/STM32duinoBLE @ feature/stm32wba-transport](https://github.com/protronic/STM32duinoBLE/tree/feature/stm32wba-transport)
- [stm32duino/STM32duinoBLE issue #88](https://github.com/stm32duino/STM32duinoBLE/issues/88)
