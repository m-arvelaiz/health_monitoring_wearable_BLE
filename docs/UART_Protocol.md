
# UART Communication Protocol Specification

This document describes the UART protocol used for communication between the BLE MCU and the Main MCU. It includes the packet structure for commands and responses, field definitions, and integrity checking via CRC.

---

## 📘 Overview

The UART protocol is designed for reliable bidirectional communication between a BLE module and the main microcontroller unit (MCU). It uses a custom, lightweight frame format to exchange command and response packets, including optional sensor data and timestamps.

Key Features:
- Fixed-size packets
- Simple CRC for data integrity
- Optional UNIX timestamp in responses for sensor measurements
- Clearly defined state and command handling

---

## 🔄 Communication Direction

- **BLE MCU → Main MCU**: Command packet (TX)
- **Main MCU → BLE MCU**: Response packet (RX)

---

## 🧩 Packet Structure

### 🔹 Command Packet (TX)
**Direction**: BLE MCU → Main MCU  
**Total Length**: 7 bytes

| Byte Index | Field Name   | Size | Description                               |
|------------|--------------|------|-------------------------------------------|
| 0          | Start Byte   | 1    | Constant start byte (`0xAA`)              |
| 1          | Command      | 1    | Command code (e.g., request type)         |
| 2–5        | Payload      | 4    | Optional parameters or data (if any)      |
| 6          | CRC          | 1    | CRC over bytes 1–5                        |

---

### 🔹 Response Packet (RX)
**Direction**: Main MCU → BLE MCU  
**Total Length**: 11 bytes

| Byte Index | Field Name   | Size | Description                                      |
|------------|--------------|------|--------------------------------------------------|
| 0          | Start Byte   | 1    | Constant start byte (`0xAA`)                    |
| 1          | Command      | 1    | Echo of the original command                    |
| 2–5        | Payload      | 4    | Response data (e.g., HR, SpO2, status codes)    |
| 6–9        | Timestamp    | 4    | UNIX timestamp (seconds since epoch); `0x00` if not a measurement |
| 10         | CRC          | 1    | CRC over bytes 1–9                              |

---

## 📦 Field Details

- **Start Byte (`0xAA`)**  
  Marks the beginning of a packet. Used to sync the parser.

- **Command**  
  Unique ID for the requested or responded operation. Should match predefined `enum`.

- **Payload (4 bytes)**  
  Data section. In requests, it's used to send parameters. In responses, it holds returned data.

- **Timestamp (4 bytes)**  
  Only valid in response packets. Filled with UNIX time if the response is a measurement. Otherwise, all zero (`0x00000000`).

- **CRC (1 byte)**  
  A simple checksum of the entire packet excluding the start byte.

---

## ✅ CRC Calculation

A simple additive checksum is used for integrity checking:

```c
// For TX packet (BLE → MCU), sum bytes 1 to 5
uint8_t UART_CalculateTXCRC(uint8_t *data) {
    uint8_t sum = 0;
    for (int i = 1; i <= 5; i++) {
        sum += data[i];
    }
    return sum;
}

// For RX packet (MCU → BLE), sum bytes 1 to 9
uint8_t UART_CalculateRXCRC(uint8_t *data) {
    uint8_t sum = 0;
    for (int i = 1; i <= 9; i++) {
        sum += data[i];
    }
    return sum;
}
```

---

## 📊 Example: Heart Rate Measurement Response

Assume the main MCU replies to `CMD_REQ_HR_SPO2` with:
- HR = 75 (`0x4B`)
- SpO2 = 98 (`0x62`)
- UNIX Time = `1650000000` → `0x62 0xEA 0xB0 0x00`

**Response Packet:**

| Index | Value (Hex) | Description          |
|-------|-------------|----------------------|
| 0     | `0xAA`      | Start byte           |
| 1     | `0x01`      | Command (HR request) |
| 2     | `0x4B`      | HR = 75              |
| 3     | `0x62`      | SpO2 = 98            |
| 4     | `0x00`      | Reserved             |
| 5     | `0x00`      | Reserved             |
| 6     | `0x62`      | Timestamp byte 1     |
| 7     | `0xEA`      | Timestamp byte 2     |
| 8     | `0xB0`      | Timestamp byte 3     |
| 9     | `0x00`      | Timestamp byte 4     |
| 10    | `CRC`       | CRC of bytes 1–9     |

---

## 🚦 UART State Machine (Optional)

In `uart_handler`, you may track the state using an enum like:

```c
typedef enum {
    UART_IDLE,
    UART_WAITING_FOR_RESPONSE,
    UART_RESPONSE_RECEIVED,
    UART_ERROR
} UART_State_t;
```

State transitions can depend on:
- Commands sent
- Response timeouts
- CRC errors
- Parsing errors

This improves robustness and allows for higher-level flow control.

---

## 🧠 Notes

- All fields are **big-endian** unless otherwise stated.
- Avoid using `0xAA` in payloads, or escape it if needed.
- Consider timeout/retry mechanisms on the BLE MCU side.

---
