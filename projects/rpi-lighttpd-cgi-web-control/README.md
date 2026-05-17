# Raspberry Pi Lighttpd CGI Web Control

Embedded web control system for Raspberry Pi using Lighttpd, CGI applications, C, Python, and GPIO control.

The project demonstrates how embedded Linux systems can expose hardware control and monitoring functionality through a lightweight web server and CGI-based applications.

The system includes:

- Python CGI applications
- C CGI applications
- GPIO control through HTTP requests
- URL query parsing
- polling-based browser updates
- Lighttpd web server configuration
- Raspberry Pi deployment workflow

---

## Architecture Overview

Browser clients communicate with a Lighttpd web server running on Raspberry Pi.

Lighttpd executes CGI applications written in Python and C. The CGI applications can:

- control GPIO outputs
- process HTTP query parameters
- generate JSON responses
- provide dynamic browser updates through polling

The project demonstrates a lightweight embedded web control architecture without requiring heavyweight backend frameworks.

---

## Embedded Linux Concepts

This project demonstrates:

- embedded web servers
- CGI execution model
- Linux process spawning
- environment variable based IPC
- GPIO access control through Linux groups
- HTTP request handling
- browser-to-device communication
- polling-based client/server communication

---

## Components

### Python CGI

Simple Python CGI application served through Lighttpd.

Features:

- Python CGI execution
- HTTP response generation
- browser integration
- Lighttpd CGI configuration

---

### GPIO CGI

GPIO control applications written in C using `libgpiod`.

Features:

- GPIO output control
- LED ON/OFF control through browser requests
- CGI execution using native binaries
- event logging

Example requests:

    /cgi-bin/ledon.cgi
    /cgi-bin/ledoff.cgi

---

### Query CGI

C CGI application that parses HTTP query parameters using `libyuarel`.

Features:

- CGI environment variable handling
- URL query parsing
- parameter extraction
- event logging
- HTTP request processing

Example request:

    /cgi-bin/env-cgi.cgi?servoposition=90&led=on

---

### Polling Demo

Browser polling demo using JavaScript and CGI-generated JSON responses.

Features:

- periodic browser polling
- JSON response generation
- dynamic browser updates
- client-server interaction

The browser periodically requests updated data from a CGI application running on Raspberry Pi.

---

## Demo Endpoints

    /cgi-bin/ledon.cgi
    /cgi-bin/ledoff.cgi
    /cgi-bin/env-cgi.cgi?servoposition=90&led=on
    /cgi-bin/time.cgi

Polling page:

    https://192.168.86.72/poll-demo.html

---

## GPIO Mapping

| Function | GPIO |
| -------- | ---- |
| LED      | 23   |

---

## Build

Create build directory:

    mkdir build
    cd build

Configure project:

    cmake ..

Build:

    make

---

## Deployment

Example deployment of CGI binaries to Raspberry Pi:

    scp build/gpio-cgi/gpio-led-on rpi:/tmp/
    scp build/gpio-cgi/gpio-led-off rpi:/tmp/
    scp build/query-cgi/query-cgi rpi:/tmp/
    scp build/polling-demo/polling-time-cgi rpi:/tmp/

Install CGI applications:

    sudo mv /tmp/gpio-led-on /usr/lib/cgi-bin/ledon.cgi
    sudo mv /tmp/gpio-led-off /usr/lib/cgi-bin/ledoff.cgi
    sudo mv /tmp/query-cgi /usr/lib/cgi-bin/env-cgi.cgi
    sudo mv /tmp/polling-time-cgi /usr/lib/cgi-bin/time.cgi

---

## How It Works

When a browser accesses a CGI endpoint, Lighttpd launches the corresponding CGI executable and passes HTTP request data through environment variables.

The CGI application processes the request, generates an HTTP response, and returns the result back to the browser.

The project demonstrates both Python-based and native C-based CGI applications running on Raspberry Pi.

---

## Polling

The polling demo periodically requests updated time data from the Raspberry Pi web server using JavaScript `fetch()` requests.

The CGI application responds with JSON data which is dynamically displayed in the browser without reloading the page.

---

## Additional Notes

Additional setup notes and deployment commands:

- [Development Notes](docs/development-notes.md)

---

## Project Structure

    .
    ├── CMakeLists.txt
    ├── README.md
    ├── docs
    │   └── development-notes.md
    ├── gpio-cgi
    │   ├── CMakeLists.txt
    │   ├── led_off.c
    │   └── led_on.c
    ├── polling-demo
    │   ├── CMakeLists.txt
    │   ├── poll-demo.html
    │   └── time.c
    ├── python-cgi
    │   └── pydemo.py
    └── query-cgi
        ├── CMakeLists.txt
        ├── main.c
        ├── yuarel.c
        └── yuarel.h
