# Architecture of the service and the cmds
The communication through the custom services is done using custom cmds that have the structure described below.

#  📜 Command List

| **Command**             | **Code** | **Description**                                                    | **Params**                                                                                 | **Response**                                     |
|-------------------------|----------|--------------------------------------------------------------------|---------------------------------------------------------------------------------------------|--------------------------------------------------|
| `REQ_HR_SPO2_DATA`      | `0x01`   | HR & SpO₂ + timestamp                                              | 0 bytes                                                                                     | Bitmask: `0b00000011` → HR + SpO₂               |
| `REQ_TEMP_DATA`         | `0x02`   | Temperature + timestamp                                            | 1 byte (0x01=Body, 0x02=Env, 0x03=Both)                                                     | Bitmask: `0b00001100`                          |
| `REQ_PRESSURE_DATA`     | `0x03`   | Pressure + timestamp                                               | 0 bytes                                                                                     | Bitmask: `0b00010000`                          |
| `REQ_ALL_DATA`          | `0x04`   | HR, SpO₂, Temp, Pressure + timestamp                               | 0 bytes                                                                                     | Multiple responses with bitmask                |
| `REQ_HISTORICAL_DATA`   | `0x10`   | Request historical data starting from given timestamp              | 4 bytes (Unix timestamp)                                                                    | Multiple responses with bitmask                |
| `SET_UNIX_TIME`         | `0x20`   | Set current Unix time on device                                    | 4 bytes (Unix time)                                                                         | ACK                                            |
| `SET_SENSOR_CONFIG`     | `0x30`   | Configure sampling rate or features (future extension)             | 2 bytes: 1st byte = sensor (same as data type), 2nd byte = config (details TBD)             | ACK                                            |
| `START_STREAM`          | `0x40`   | Begin real-time data notifications                                 | 1 byte (bitmask, e.g., `0x1F` = all sensors)                                                | Multiple periodic responses with bitmask       |
| `STOP_STREAM`           | `0x41`   | Stop real-time data notifications                                  | 0 bytes                                                                                     | ACK                                            |


# 🧩 BLE Communication Protocol – Write & Notify Structures

## ✏️ Write Command Structure (General)

| **Field**   | **Size** | **Description**                                                                 |
|-------------|----------|---------------------------------------------------------------------------------|
| `CMD`       | 1 byte   | Command ID (e.g., `0x04` for Sensor Data Request, `0x10` for Historical Data)   |
| `LEN`       | 1 byte   | Length of the packet (excluding the CRC byte)                                   |
| `PARAMS`    | 4 bytes  | Optional parameters (e.g., Unix time or zeros if unused)                        |
| `CRC`       | 1 byte   | Checksum for integrity (sum of all previous bytes % 256)                        |

> 💡 Total: **7 bytes** per write command


---

### 🧾 Example: Request Live Sensor Data

Request heart rate and SpO₂ from the device.

```
CMD     = 0x04
LEN     = 0x06 (CMD + LEN + PARAMS = 6 bytes)
PARAMS  = 0x00 00 00 00 (no additional data)
CRC     = 0x0A (example CRC)
```

Final payload: `04 06 00 00 00 00 0A`

---

## 📥 Notify Response Structure (Live Sensor Read – `CMD 0x04`)

| **Field**      | **Size** | **Description**                                                                 |
|----------------|----------|----------------------------------------------------------------------------------|
| `CMD`    | 1 byte   | Bitmask indicating which values are present (see bitmask table below)            |
| `LEN`          | 1 byte   | Payload length: always `0x05`                                                    |
| `DATA`         | 5 bytes  | One or two sensor values (2 bytes each × N, padded to 5 bytes if needed)         |
| `TIMESTAMP`    | 4 bytes  | Unix timestamp in seconds (UTC)                                                 |
| `CRC`          | 1 byte   | Checksum of bytes 0–10 (modulo 256 or CRC-8)                                    |

> 💡 Total: **12 bytes** per notify message  
> Sensor values are encoded as: `int(value * 100)` → decode with: `real_value = value / 100.0`

---

## 📥 Notify Response Structure (Historical Data – `CMD 0x10`)

Same structure as above, sent repeatedly for different timestamps.

| **Field**      | **Size** | **Description**                                                                 |
|----------------|----------|----------------------------------------------------------------------------------|
| `DATA_TYPE`    | 1 byte   | Bitmask to indicate included sensor data                                        |
| `LEN`          | 1 byte   | Always `0x05`                                                                   |
| `DATA`         | 5 bytes  | Packed 1–2 sensor readings                                                      |
| `TIMESTAMP`    | 4 bytes  | Unix time corresponding to the readings                                         |
| `CRC`          | 1 byte   | Checksum for data integrity                                                     |

---

## 🧮 Bitmask Table for `DATA_TYPE` Field

| **Bit** | **Value** | **Sensor**         |
|---------|-----------|--------------------|
| 0       | `0x01`    | Heart Rate (HR)    |
| 1       | `0x02`    | SpO₂               |
| 2       | `0x04`    | Temperature        |
| 3       | `0x08`    | Pressure           |
| 4       | `0x10`    | Resp. Rate (RR)\*  |
| 5       | `0x20`    | HRV\*              |



---