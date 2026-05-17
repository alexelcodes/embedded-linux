# Development Notes

## MQTT Command-Line Demo

### macOS (Host)

Install Mosquitto broker:

```bash
brew install mosquitto
```

Start MQTT broker:

```bash
mosquitto -v
```

---

### Test locally (Host)

Start subscriber:

```bash
mosquitto_sub -h localhost -t test
```

Publish message:

```bash
mosquitto_pub -h localhost -t test -m "hello"
```

Stop broker:

```bash
pkill mosquitto
```

---

### MQTTX

Connect to broker:

- host: localhost
- port: 1883
- topic: test

---

### Raspberry Pi

Install MQTT clients:

```bash
sudo apt update
sudo apt install mosquitto-clients -y
```

---

### Test Pi → Host

Get host IP address.

Wi-Fi:

```bash
ipconfig getifaddr en0
```

Ethernet:

```bash
ipconfig getifaddr en6
```

Check connection from Raspberry Pi:

```bash
ping 192.168.86.66
```

Publish from Raspberry Pi:

```bash
mosquitto_pub -h 192.168.86.66 -t test -m "hello from pi"
```

---

## Native MQTT C Client (Paho)

### Setup Paho library

Create dependency directory:

```bash
mkdir deps
```

Clone Paho repository:

```bash
cd deps
git clone https://github.com/eclipse-paho/paho.mqtt.c.git
```

---

### Native build

Create build directory:

```bash
cd paho.mqtt.c
mkdir -p build
cd build
```

Configure build:

```bash
cmake -DPAHO_WITH_SSL=TRUE ..
```

Build library:

```bash
make
```

---

### Project structure

Create project folders:

```bash
mkdir -p mqtt-vm mqtt-raspi
```

Copy sample MQTT client:

```bash
cp deps/paho.mqtt.c/src/samples/MQTTClient_publish.c \
mqtt-vm/mqtt-client.c
```

---

### Configure MQTT client

Broker:

```c
#define ADDRESS "tcp://localhost:1883"
```

Topic:

```c
#define TOPIC "test"
```

---

### CMake setup

Created:

- root `CMakeLists.txt`
- `mqtt-vm/CMakeLists.txt`
- `mqtt-raspi/CMakeLists.txt`

Linked against local Paho build:

- includes: `deps/paho.mqtt.c/src`
- library: `paho-mqtt3c`

---

### VS Code setup

Configured:

- `.vscode/settings.json`
- separate build folders for native and ARM builds
- `launch.json` run configuration for native debugging

---

## Cross-built MQTT Client (Raspberry Pi)

### Toolchain notes

The ARM cross-build required additional linker search paths in:

```text
cmake/toolchain-armhf-macos.cmake
```

The toolchain file includes `-B` paths and `rpath-link` options so the linker can resolve target-side libraries and indirect dependencies during cross-compilation.

This was needed for dependencies such as:

- OpenSSL libraries
- `libatomic`
- C runtime libraries

---

### Prepare Raspberry Pi dependencies

Install OpenSSL headers and libraries:

```bash
sudo apt update
sudo apt install -y libssl-dev
```

---

### Sync OpenSSL headers

```bash
rsync -avz \
rpi:/usr/include/openssl/ \
~/rpi/sysroot/usr/include/openssl/
```

---

### Sync OpenSSL libraries

```bash
rsync -avz \
'rpi:/usr/lib/arm-linux-gnueabihf/libcrypto.so*' \
~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/

rsync -avz \
'rpi:/usr/lib/arm-linux-gnueabihf/libssl.so*' \
~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/
```

---

### Sync multiarch OpenSSL headers

```bash
rsync -avz \
'rpi:/usr/include/arm-linux-gnueabihf/openssl/' \
~/rpi/sysroot/usr/include/arm-linux-gnueabihf/openssl/
```

---

### Sync libatomic

```bash
rsync -avz \
'rpi:/usr/lib/arm-linux-gnueabihf/libatomic.so*' \
~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/
```

---

### Configure ARM build

```bash
cmake -S deps/paho.mqtt.c \
      -B deps/paho.mqtt.c/build-armhf \
      -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/toolchain-armhf-macos.cmake" \
      -DPAHO_WITH_SSL=TRUE
```

Notes:

- `-S` → source directory
- `-B` → build directory
- toolchain file enables cross-compilation
- SSL support requires OpenSSL in sysroot

---

### Build ARM libraries

```bash
cmake --build deps/paho.mqtt.c/build-armhf
```

---

### Install libraries into sysroot

```bash
DESTDIR="$HOME/rpi/sysroot" \
cmake --install deps/paho.mqtt.c/build-armhf --prefix /usr/local
```

The application links against libraries installed inside the sysroot to mirror the Raspberry Pi filesystem layout.

---

## Raspberry Pi Deployment

### Copy libraries

```bash
rsync -avz \
~/rpi/sysroot/usr/local/lib/libpaho-mqtt3c.so* \
rpi:/tmp/
```

---

### Install libraries on Raspberry Pi

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

### Remove temporary files

```bash
ssh rpi "rm -f /tmp/libpaho-mqtt3c.so*"
```

---

## Local MQTT Broker Configuration

### Create broker config

```bash
echo -e "listener 1883\nallow_anonymous true" > ~/mosquitto.conf
```

---

### Run broker

```bash
mosquitto -c ~/mosquitto.conf -v
```

---

### Configure MQTTX

- host → Host IP
- port → 1883
- topic → test

---

## Raspberry Pi MQTT Client

### Run application

```bash
./mqtt-client
```

Result:

- MQTT client runs on Raspberry Pi
- connects to host MQTT broker
- publishes messages successfully

---

## GPIO + MQTT

Added GPIO button input using `libgpiod`:

- GPIO22 button input
- internal pull-up resistor
- default state → 1
- pressed → 0

Integrated with MQTT client:

- on press → publish `button_pressed`
- on release → publish `button_released`

Result:

- button events detected on Raspberry Pi
- MQTT messages published in real time
