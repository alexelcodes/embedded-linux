# Development Notes

## MQTT Command-Line Testing

Start Mosquitto broker:

```bash
mosquitto -v
```

Subscribe to topic:

```bash
mosquitto_sub -h localhost -t test
```

Publish test message:

```bash
mosquitto_pub -h localhost -t test -m "hello"
```

Stop Mosquitto:

```bash
pkill mosquitto
```

---

## Network Testing

Get macOS IP address.

Wi-Fi:

```bash
ipconfig getifaddr en0
```

Ethernet:

```bash
ipconfig getifaddr en6
```

Check connectivity from Raspberry Pi:

```bash
ping 192.168.86.66
```

---

## Raspberry Pi MQTT Clients

Install MQTT client utilities:

```bash
sudo apt update
sudo apt install -y mosquitto-clients
```

Publish message from Raspberry Pi:

```bash
mosquitto_pub -h 192.168.86.66 -t test -m "hello from pi"
```

---

# Paho MQTT Library

## Clone repository

```bash
mkdir deps
cd deps

git clone https://github.com/eclipse-paho/paho.mqtt.c.git
```

---

## Native build

```bash
cd paho.mqtt.c

mkdir -p build
cd build

cmake -DPAHO_WITH_SSL=TRUE ..
make
```

---

## Copy MQTT sample client

```bash
mkdir -p mqtt-vm mqtt-raspi

cp deps/paho.mqtt.c/src/samples/MQTTClient_publish.c \
mqtt-vm/mqtt-client.c
```

---

## Example MQTT configuration

Broker:

```c
#define ADDRESS "tcp://localhost:1883"
```

Topic:

```c
#define TOPIC "test"
```

---

# Cross-Compilation

## Install OpenSSL on Raspberry Pi

```bash
sudo apt update
sudo apt install -y libssl-dev
```

---

## Sync OpenSSL headers

```bash
rsync -avz \
rpi:/usr/include/openssl/ \
~/rpi/sysroot/usr/include/openssl/
```

---

## Sync OpenSSL libraries

```bash
rsync -avz \
'rpi:/usr/lib/arm-linux-gnueabihf/libcrypto.so*' \
~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/

rsync -avz \
'rpi:/usr/lib/arm-linux-gnueabihf/libssl.so*' \
~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/
```

---

## Sync multiarch OpenSSL headers

```bash
rsync -avz \
'rpi:/usr/include/arm-linux-gnueabihf/openssl/' \
~/rpi/sysroot/usr/include/arm-linux-gnueabihf/openssl/
```

---

## Sync libatomic

```bash
rsync -avz \
'rpi:/usr/lib/arm-linux-gnueabihf/libatomic.so*' \
~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/
```

---

## Configure ARM build

```bash
cmake -S deps/paho.mqtt.c \
      -B deps/paho.mqtt.c/build-armhf \
      -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/toolchain-armhf-macos.cmake" \
      -DPAHO_WITH_SSL=TRUE
```

---

## Build ARM libraries

```bash
cmake --build deps/paho.mqtt.c/build-armhf
```

---

## Install libraries into sysroot

```bash
DESTDIR="$HOME/rpi/sysroot" \
cmake --install deps/paho.mqtt.c/build-armhf --prefix /usr/local
```

---

# Raspberry Pi Deployment

## Copy libraries

```bash
rsync -avz \
~/rpi/sysroot/usr/local/lib/libpaho-mqtt3c.so* \
rpi:/tmp/
```

---

## Install libraries on Raspberry Pi

```bash
ssh rpi "
sudo cp /tmp/libpaho-mqtt3c.so.1.3.16 /usr/local/lib/ && \
cd /usr/local/lib && \
sudo ln -sf libpaho-mqtt3c.so.1.3.16 libpaho-mqtt3c.so.1 && \
sudo ln -sf libpaho-mqtt3c.so.1 libpaho-mqtt3c.so && \
sudo ldconfig
"
```

---

## Remove temporary files

```bash
ssh rpi "rm -f /tmp/libpaho-mqtt3c.so*"
```

---

# Local MQTT Broker Configuration

Create broker config:

```bash
echo -e "listener 1883\nallow_anonymous true" > ~/mosquitto.conf
```

Run broker:

```bash
mosquitto -c ~/mosquitto.conf -v
```

---

# Raspberry Pi MQTT Client

Run application:

```bash
./mqtt-client
```

---

# GPIO + MQTT

Final application behavior:

- GPIO22 button input
- internal pull-up resistor
- MQTT publish on button state changes

Published messages:

- `button_pressed`
- `button_released`
