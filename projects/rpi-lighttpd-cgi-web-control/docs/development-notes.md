# Development Notes

## Lighttpd Verification

Check Lighttpd service status:

```bash
sudo systemctl status lighttpd.service
```

Get Raspberry Pi IP address:

```bash
hostname -I
```

Example:

```text
192.168.86.72
```

---

## Default Web Content

Check current web root contents:

```bash
ls -l /var/www/html
```

Find default Lighttpd page:

```bash
sudo find /usr/share -name index.html 2>/dev/null
```

Copy default page to web root:

```bash
sudo cp /usr/share/lighttpd/index.html /var/www/html/index.html
```

---

## HTTP Test

Test server access from another machine:

```bash
curl http://192.168.86.72
```

---

# Access Logs and Debugging

## Monitor access log

```bash
sudo tail -f /var/log/lighttpd/access.log
```

Shows incoming HTTP requests, client IP addresses, request methods, and response status codes.

---

## Enable verbose debug logging

Open Lighttpd main configuration:

```bash
sudo nano /etc/lighttpd/lighttpd.conf
```

Add:

```text
debug.log-request-header = "enable"
debug.log-response-header = "enable"
debug.log-request-handling = "enable"
debug.log-file-not-found = "enable"
debug.log-condition-handling = "enable"
debug.log-timeouts = "enable"
```

Apply configuration changes:

```bash
sudo service lighttpd force-reload
```

---

## Monitor verbose logs

```bash
sudo tail -f /var/log/lighttpd/error.log
```

Verbose logs help inspect:

- HTTP request headers
- HTTP response headers
- URI parsing
- logical to physical path mapping
- file access problems
- CGI execution issues
- timeout handling

---

# HTTPS / SSL Configuration

## Create self-signed certificate

Create certificate directory:

```bash
sudo mkdir -p /etc/lighttpd/certs
```

Generate self-signed certificate:

```bash
sudo openssl req -new -x509 \
    -keyout /etc/lighttpd/certs/lighttpd.pem \
    -out /etc/lighttpd/certs/lighttpd.pem \
    -days 365 \
    -nodes
```

The certificate fields can be filled arbitrarily for local network testing.

Restrict permissions:

```bash
sudo chmod 400 /etc/lighttpd/certs/lighttpd.pem
```

---

## Enable HTTPS support

Open Lighttpd configuration:

```bash
sudo nano /etc/lighttpd/lighttpd.conf
```

Add OpenSSL module:

```text
"mod_openssl",
```

Add HTTPS configuration:

```text
$SERVER["socket"] == ":443" {
  ssl.engine = "enable"
  ssl.pemfile = "/etc/lighttpd/certs/lighttpd.pem"
}
```

Restart service:

```bash
sudo systemctl restart lighttpd
```

Verify service status:

```bash
sudo systemctl status lighttpd.service
```

---

## HTTPS tests

Browser test:

```text
https://192.168.86.72
```

Self-signed certificate warnings are expected.

Test using curl:

```bash
curl --insecure https://192.168.86.72
```

Verify TLS connection:

```bash
curl -k -v https://192.168.86.72
```

---

# Static Web Pages

## Document root

Check document root contents:

```bash
ls -l /var/www/html
```

---

## Replace default Lighttpd page

Create backup directory:

```bash
sudo mkdir /var/www/html/orig
```

Move original page:

```bash
sudo mv /var/www/html/index.html /var/www/html/orig/
```

Create custom index page:

```bash
sudo nano /var/www/html/index.html
```

After deployment, the custom page becomes accessible through the Lighttpd web server.

---

# Lighttpd + CGI Setup

## Enable CGI module

```bash
sudo lighty-enable-mod cgi
```

Verify that the symlink was created:

```bash
ls -l /etc/lighttpd/conf-enabled/
```

Expected:

```text
10-cgi.conf -> ../conf-available/10-cgi.conf
```

---

## Configure CGI handlers

Open configuration:

```bash
sudo nano /etc/lighttpd/conf-enabled/10-cgi.conf
```

Replace the default assignment:

```text
cgi.assign = ( "" => "" )
```

with:

```text
cgi.assign = (
    ".sh"  => "/usr/bin/bash",
    ".py"  => "/usr/bin/python3",
    ".cgi" => ""
)
```

---

## Disable HTTP/2

Lighttpd CGI handling was more stable with HTTP/2 disabled.

Open configuration:

```bash
sudo nano /etc/lighttpd/lighttpd.conf
```

Add:

```text
server.feature-flags += ("server.h2proto" => "disable")
server.feature-flags += ("server.h2c"     => "disable")
```

Restart Lighttpd:

```bash
sudo systemctl restart lighttpd
```

---

# Python CGI

## Deploy Python CGI script

Copy script:

```bash
sudo cp pydemo.py /usr/lib/cgi-bin/
```

Make executable:

```bash
sudo chmod +x /usr/lib/cgi-bin/pydemo.py
```

Test in browser:

```text
http://192.168.86.72/cgi-bin/pydemo.py
```

Test using curl:

```bash
curl -i http://192.168.86.72/cgi-bin/pydemo.py
```

HTTPS test:

```bash
curl -k -i https://192.168.86.72/cgi-bin/pydemo.py
```

---

# GPIO CGI Applications

## Deploy GPIO CGI binaries

Copy binaries from host:

```bash
scp build/gpio-cgi/gpio-led-on rpi:/tmp/
scp build/gpio-cgi/gpio-led-off rpi:/tmp/
```

Install on Raspberry Pi:

```bash
sudo mv /tmp/gpio-led-on /usr/lib/cgi-bin/ledon.cgi
sudo mv /tmp/gpio-led-off /usr/lib/cgi-bin/ledoff.cgi
```

---

## GPIO permissions

CGI applications run as user `www-data`.

Add `www-data` to GPIO group:

```bash
sudo usermod -aG gpio www-data
```

Restart Lighttpd:

```bash
sudo systemctl restart lighttpd
```

---

## GPIO CGI tests

LED ON:

```text
http://192.168.86.72/cgi-bin/ledon.cgi
```

LED OFF:

```text
http://192.168.86.72/cgi-bin/ledoff.cgi
```

Check logs:

```bash
cat /tmp/led.log
tail -f /tmp/led.log
```

---

# CGI Query Parameters

The query CGI application parses HTTP query parameters from the CGI environment variable:

```text
QUERY_STRING
```

Example parameters:

```text
servoposition=90
led=on
```

The application:

- parses URL query parameters
- prints parsed values in browser
- writes events to `/tmp/cgi.log`

---

## Deploy query CGI application

Copy binary from host:

```bash
scp build/query-cgi/query-cgi rpi:/tmp/
```

Install on Raspberry Pi:

```bash
sudo mv /tmp/query-cgi /usr/lib/cgi-bin/env-cgi.cgi
```

---

## Query CGI tests

Without parameters:

```text
http://192.168.86.72/cgi-bin/env-cgi.cgi
```

With parameters:

```text
http://192.168.86.72/cgi-bin/env-cgi.cgi?servoposition=90&led=on
```

Check logs:

```bash
cat /tmp/cgi.log
tail -f /tmp/cgi.log
```

---

# Polling Demo

## Deploy polling CGI application

Copy binary from host:

```bash
scp build/polling-demo/polling-time-cgi rpi:/tmp/
```

Install on Raspberry Pi:

```bash
sudo mv /tmp/polling-time-cgi /usr/lib/cgi-bin/time.cgi
```

Test:

```text
http://192.168.86.72/cgi-bin/time.cgi
```

---

## Deploy polling HTML page

Copy HTML page:

```bash
scp polling-demo/poll-demo.html rpi:/tmp/
```

Install page:

```bash
sudo mv /tmp/poll-demo.html /var/www/html/
```

Open in browser:

```text
https://192.168.86.72/poll-demo.html
```

---

# Cross-Origin Resource Sharing (CORS)

Modern browsers restrict JavaScript requests across origins unless the server explicitly allows them.

The polling demo requires enabling CORS headers for CGI responses.

---

## Enable mod_setenv

```bash
sudo lighty-enable-mod setenv
```

---

## Configure CORS headers

Open configuration:

```bash
sudo nano /etc/lighttpd/lighttpd.conf
```

Add:

```text
setenv.add-response-header += (
   "Access-Control-Allow-Origin" => "*"
)
```

Apply configuration changes:

```bash
sudo systemctl restart lighttpd
```

---

## Verify browser requests

Browser developer tools can be used to inspect:

- request headers
- response headers
- CORS policy errors
- polling frequency
- CGI request timing

---

# CPU Load Monitoring

Monitor CPU usage during polling tests:

```bash
top
```

or:

```bash
htop
```

Approximate CPU load:

```text
CPU (%) = 100 - idle
```

Example:

```text
idle ≈ 80%
CPU  ≈ 20%
```
