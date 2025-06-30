# BLE Command and Notification Documentation

_Last updated: 2025-06-29 23:53_

---

# Architecture of the Service and Commands

Communication uses a custom BLE service with the following commands and responses.

## 📜 Command List

| **Command**             | **Code** | **Description**                                                    | **Params**                                    | **Response**                                     |
|-------------------------|----------|--------------------------------------------------------------------|-----------------------------------------------|--------------------------------------------------|
| `REQ_HR_SPO2_DATA`      | `0x01`   | Request Heart Rate & SpO₂ + timestamp                              | 0 bytes                                       | HR+SpO₂ notify                                   |
| `REQ_TEMP_DATA`         | `0x02`   | Request Temperature + timestamp                                    | 1 byte (0x01=Body, 0x02=Env, 0x03=Both)       | Temp notify                                      |
| `REQ_PRESSURE_DATA`     | `0x03`   | Request Pressure + timestamp                                       | 0 bytes                                       | Pressure notify                                  |
| `REQ_ALL_DATA`          | `0x04`   | Request all sensors (HR, SpO₂, Temp, Pressure) + timestamp         | 0 bytes                                       | Multiple notifies (HR/SpO₂ then Temp/Pressure)   |
| `REQ_HISTORICAL_DATA`   | `0x10`   | Historical data from given timestamp                               | 4 bytes (Unix time)                           | Multiple notifies                                |
| `SET_UNIX_TIME`         | `0x20`   | Set device Unix time                                               | 4 bytes (Unix time)                           | ACK notify                                       |
| `SET_SENSOR_CONFIG`     | `0x30`   | Configure sensor features                                          | 2 bytes (sensor ID, config value)             | ACK notify                                       |
| `START_STREAM`          | `0x40`   | Begin real-time streaming                                          | 1 byte (bitmask of sensors)                   | Periodic streaming notifies                      |
| `STOP_STREAM`           | `0x41`   | Stop streaming                                                     | 0 bytes                                       | ACK notify                                       |

---

# 🧩 BLE Communication Protocol

## ✏️ Write Command Structure

| **Field** | **Size** | **Description**                                                    |
|-----------|----------|--------------------------------------------------------------------|
| `CMD`     | 1 byte   | Command ID                                                         |
| `LEN`     | 1 byte   | Length of payload (excluding CRC)                                  |
| `PARAMS`  | 4 bytes  | Command-specific parameters (zeros if unused)                     |
| `CRC`     | 1 byte   | XOR checksum of all previous bytes                                 |

> **7 bytes total** per write command.

### 📘 Example: Request Live HR/SpO₂

```
CMD    = 0x01
LEN    = 0x06   ; CMD+LEN+PARAMS=6
PARAMS = 0x00 00 00 00
CRC    = XOR(01 ^ 06 ^ 00 ^ 00 ^ 00 ^ 00) = 0x07
```
Final: `01 06 00 00 00 00 07`

---

## 📥 Notify Response Structure (Live Sensor Read)

| **Field** | **Size** | **Description**                                                        |
|-----------|----------|------------------------------------------------------------------------|
| `CMD`     | 1 byte   | Command ID                                                              |
| `LEN`     | 1 byte   | Payload length (should be `0x09`; logs show `0x05` due to bug)         |
| `DATA`    | 9 bytes  | Sensor data payload                                                     |
| `CRC`     | 1 byte   | XOR of all bytes 0–10                                                   |

> **12 bytes total** per notify message.

### 🧩 Data Payload Format (9 bytes)

| Byte(s) | Description                                                                              |
|---------|------------------------------------------------------------------------------------------|
| 0       | Subtype (e.g., `0x01`=Body, `0x02`=Env, else `0x00`)                                     |
| 1–2     | First value (MSB first)                                                                  |
| 3–4     | Second value (MSB first) or zeros if unused                                             |
| 5–8     | Timestamp (Unix time, MSB first)                                                         |

---

### 🔍 Sensor-Specific Payloads

| **CMD ID** | **Name**           | **Bytes 0–8**                                                                                     |
|------------|--------------------|---------------------------------------------------------------------------------------------------|
| `0x01`     | HR & SpO₂          | `00` ● HR[MSB] HR[LSB] ● SpO₂[MSB] SpO₂[LSB] ● T[4 bytes]                                            |
| `0x02`     | Temperature        | `01` or `02` ● Temp×100[MSB] Temp×100[LSB] ● `00 00` ● T[4 bytes]                                  |
| `0x03`     | Pressure           | `00` ● Press[MSB] Press[Mid] Press[LSB] ● `00` ● T[4 bytes]                                        |
| `0x04`     | All Data           | Send two notifies: first `[0x01]` HR/SpO₂, second `[0x02]` Temp/Pressure                            |

> - **Temp decode**: raw / 100 → °C<br>- **Pressure**: use 3-byte raw in Pressure_value → rawhPa/10 hPa       

---

## 🧪 CRC Calculation Example

```
Notify: 01 05 00 00 62 00 63 60 D4 A0 00 9F
```
- CRC byte = `9F`  
- Computed:  
  `01 ^ 05 ^ 00 ^ 00 ^ 62 ^ 00 ^ 63 ^ 60 ^ D4 ^ A0 ^ 00 = 9F`

---

## ✅ BLE Log Examples

```
2025-06-30 01:37:18 Write: 01 01 00 00 00 00 02  Succeeded
2025-06-30 01:37:18 Notify: 01 05 00 00 62 00 63 60 D4 A0 00 9F

2025-06-30 01:37:23 Write: 02 01 02 00 00 00 05  Succeeded
2025-06-30 01:37:23 Notify: 02 05 02 0E 54 00 00 60 D4 A0 00 3F

2025-06-30 01:37:29 Write: 02 01 01 00 00 00 04  Succeeded
2025-06-30 01:37:29 Notify: 02 05 01 0E 7C 00 00 60 D4 A0 00 66

2025-06-30 01:37:44 Write: 03 00 00 00 00 00 03  Succeeded
2025-06-30 01:37:44 Notify: 03 05 00 00 24 CF 00 60 D4 A0 00 CF
```



### Decoded BLE Notifications

From that example, this is are the steps to decode

---

#### 1) Heart Rate & SpO₂

```
Notify: 01 05 00 00 62 00 63 60 D4 A0 00 9F
```

| Field         | Bytes           | Value                                     |
|---------------|-----------------|-------------------------------------------|
| **CMD**       | `01`            | HR & SpO₂                                 |
| **LEN**       | `05`            | (device bug—should be `09`)               |
| **Subtype**   | `00`            | unused                                    |
| **HR**        | `00 62`         | 0x0062 = **98 bpm**                       |
| **SpO₂**      | `00 63`         | 0x0063 = **99 %**                         |
| **Timestamp** | `60 D4 A0 00`   | 0x60D4A000 = **1 624 547 328** (dummy)    |
| **CRC**       | `9F`            | XOR checksum of bytes 0–10                |

---

#### 2) Environmental Temperature

```
Notify: 02 05 02 0E 54 00 00 60 D4 A0 00 3F
```

| Field         | Bytes           | Value                                     |
|---------------|-----------------|-------------------------------------------|
| **CMD**       | `02`            | Temperature                               |
| **LEN**       | `05`            | (device bug—should be `09`)               |
| **Subtype**   | `02`            | Environmental                             |
| **Temp×100**  | `0E 54`         | 0x0E54 = **3668** → **36.68 °C**           |
| **Padding**   | `00 00`         | unused                                    |
| **Timestamp** | `60 D4 A0 00`   | 0x60D4A000 = **1 624 547 328** (dummy)    |
| **CRC**       | `3F`            | XOR checksum of bytes 0–10                |

---

#### 3) Body Temperature

```
Notify: 02 05 01 0E 7C 00 00 60 D4 A0 00 66
```

| Field         | Bytes           | Value                                     |
|---------------|-----------------|-------------------------------------------|
| **CMD**       | `02`            | Temperature                               |
| **LEN**       | `05`            | (device bug—should be `09`)               |
| **Subtype**   | `01`            | Body                                      |
| **Temp×100**  | `0E 7C`         | 0x0E7C = **3708** → **37.08 °C**           |
| **Padding**   | `00 00`         | unused                                    |
| **Timestamp** | `60 D4 A0 00`   | 0x60D4A000 = **1 624 547 328** (dummy)    |
| **CRC**       | `66`            | XOR checksum of bytes 0–10                |

---

#### 4) Pressure

```
Notify: 03 05 00 00 24 CF 00 60 D4 A0 00 CF
```

| Field          | Bytes           | Value                                           |
|----------------|-----------------|-------------------------------------------------|
| **CMD**        | `03`            | Pressure                                        |
| **LEN**        | `05`            | (device bug—should be `09`)                     |
| **Subtype**    | `00`            | unused                                          |
| **Press Raw**  | `00 24 CF`      | 0x0024CF = **9423** raw units                   |
| **Press /10**  | —               | 9423 ÷ 10 = **942.3 Pa**                        |
| **Padding**    | `00`            | unused                                          |
| **Timestamp**  | `60 D4 A0 00`   | 0x60D4A000 = **1 624 547 328** (dummy)          |
| **CRC**        | `CF`            | XOR checksum of bytes 0–10                      |



#### CRC Calculation Example

For the first notify:

```
0x01 ^ 0x05 ^ 0x00 ^ 0x00 ^ 0x62 ^ 0x00 ^ 0x63 ^ 0x60 ^ 0xD4 ^ 0xA0 ^ 0x00 = 0x9F
```

Verify by XORing bytes 0–10 and comparing to byte 11.

