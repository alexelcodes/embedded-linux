# Development Notes

## Setup Lighttpd

Update system and install web server:

```bash
sudo apt update
sudo apt upgrade
sudo apt install lighttpd
```

---

## Sync Sysroot (after Raspberry Pi update)

Sync required files for cross-compilation:

- headers
- user-space libraries
- crt objects
- runtime libraries
- dynamic loader

```bash
rsync -a --delete --rsync-path="sudo rsync" rpi:/usr/include/ ~/rpi/sysroot/usr/include/
rsync -a --delete --rsync-path="sudo rsync" rpi:/usr/lib/arm-linux-gnueabihf/ ~/rpi/sysroot/usr/lib/arm-linux-gnueabihf/
rsync -a --delete --rsync-path="sudo rsync" rpi:/lib/arm-linux-gnueabihf/ ~/rpi/sysroot/lib/arm-linux-gnueabihf/
rsync -a --delete --rsync-path="sudo rsync" rpi:/lib/ld-linux-armhf.so.3 ~/rpi/sysroot/lib/
```

---

### Lighttpd Verification

Check Lighttpd service status:

```bash
sudo systemctl status lighttpd.service
```

Expected:

- loaded
- enabled
- running

---

### Start Lighttpd service

```bash
sudo systemctl start lighttpd.service
```

---

### Reload configuration

```bash
sudo service lighttpd force-reload
```

Configuration reload works only if the service is already running.

---

### Service logs

Check recent service logs and configuration errors:

```bash
sudo journalctl -u lighttpd.service -n 20
```

---

### Get Raspberry Pi IP address

```bash
hostname -I
```

Example:

```text
192.168.86.72
```

Use the IPv4 address for browser access.

---

## Default Web Content

### Check document root contents

```bash
ls -l /var/www/html
```

If the folder is empty, Lighttpd returns HTTP 403.

---

### Find default Lighttpd page

```bash
sudo find /usr/share -name index.html 2>/dev/null
```

Typical location:

```text
/usr/share/lighttpd/index.html
```

---

### Copy default page to web root

```bash
sudo cp /usr/share/lighttpd/index.html /var/www/html/index.html
```

---

### HTTP test

Test server access from another machine:

```bash
curl http://192.168.86.72
```

Save response:

```bash
curl http://192.168.86.72 > results/placeholderpage.html
```

---

## Access Logs and Debugging

### Enable access log module

```bash
sudo lighty-enable-mod accesslog
```

Apply configuration:

```bash
sudo service lighttpd force-reload
```

---

### Inspect enabled modules

```bash
ls /etc/lighttpd/conf-enabled/
```

---

### Check access log configuration

Open configuration:

```bash
sudo nano /etc/lighttpd/conf-enabled/10-accesslog.conf
```

Example:

```text
accesslog.filename = "/var/log/lighttpd/access.log"
```

---

### Monitor access log

```bash
sudo tail -f /var/log/lighttpd/access.log
```

Shows:

- incoming HTTP requests
- client IP addresses
- HTTP methods
- response status codes

---

### Enable verbose debug logging

Open Lighttpd configuration:

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

### Monitor verbose logs

```bash
sudo tail -f /var/log/lighttpd/error.log
```

Useful for inspecting:

- HTTP request headers
- HTTP response headers
- URI parsing
- logical to physical path mapping
- CGI execution problems
- file access issues
- timeout handling

---

## HTTPS / SSL Configuration

### Create self-signed certificate

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

### Enable HTTPS support

Open configuration:

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

Verify service:

```bash
sudo systemctl status lighttpd.service
```

Expected:

- active (running)
- no fatal configuration errors

Browser certificate warnings are expected because the certificate is self-signed.

---

### HTTPS tests

Browser:

```text
https://192.168.86.72
```

Test using curl:

```bash
curl --insecure https://192.168.86.72
```

Save HTTPS page:

```bash
curl --insecure https://192.168.86.72 > results/httpspage.html
```

Verify TLS connection:

```bash
curl -k -v https://192.168.86.72
```

Detailed TLS verification:

```bash
curl -k -v https://192.168.86.72 2>&1 | grep -E "Connected|SSL connection|HTTP/2|:scheme"
```

---

## Static Web Pages

### Check document root permissions

```bash
ls -l /var/www/html
```

Verify:

- document root path
- file ownership
- read permissions for web server

---

### Replace default Lighttpd page

Create backup directory:

```bash
sudo mkdir /var/www/html/orig
```

Move original page:

```bash
sudo mv /var/www/html/index.html /var/www/html/orig/
```

Without index.html the server returns HTTP 403.

---

### Create custom index page

```bash
sudo nano /var/www/html/index.html
```

After deployment the custom page becomes accessible through the Lighttpd server.

---

### Verify custom pages

```bash
curl -k https://192.168.86.72/orig/ > results/original_page.html
```

```bash
curl -k https://192.168.86.72 > results/custom_page.html
```

---

## Lighttpd + CGI Setup

### Enable CGI module

```bash
sudo lighty-enable-mod cgi
```

Verify symlink:

```bash
ls -l /etc/lighttpd/conf-enabled/
```

Expected:

```text
10-cgi.conf -> ../conf-available/10-cgi.conf
```

---

### Configure CGI handlers

Open configuration:

```bash
sudo nano /etc/lighttpd/conf-enabled/10-cgi.conf
```

Replace:

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

### Disable HTTP/2

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

## Python CGI

### Deploy Python CGI script

Copy script:

```bash
sudo cp pydemo.py /usr/lib/cgi-bin/
```

Make executable:

```bash
sudo chmod +x /usr/lib/cgi-bin/pydemo.py
```

---

### Python CGI tests

Browser:

```text
http://192.168.86.72/cgi-bin/pydemo.py
```

Curl:

```bash
curl -i http://192.168.86.72/cgi-bin/pydemo.py
```

HTTPS:

```bash
curl -k -i https://192.168.86.72/cgi-bin/pydemo.py
```

---

### CGI troubleshooting

If Lighttpd returns HTTP 500:

Run script directly:

```bash
python3 /usr/lib/cgi-bin/pydemo.py
```

Run executable directly:

```bash
/usr/lib/cgi-bin/pydemo.py
```

Run as Lighttpd user:

```bash
sudo runuser -u www-data -- /usr/lib/cgi-bin/pydemo.py
```

---

## GPIO CGI Applications

### Deploy GPIO CGI binaries

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

### GPIO permissions

CGI applications run as user `www-data`.

Add `www-data` to GPIO group:

```bash
sudo usermod -aG gpio www-data
```

Restart Lighttpd:

```bash
sudo systemctl restart lighttpd
```

The restart is required because CGI child processes inherit group memberships from the Lighttpd service process.

---

### GPIO CGI tests

LED ON:

```text
http://192.168.86.72/cgi-bin/ledon.cgi
```

LED OFF:

```text
http://192.168.86.72/cgi-bin/ledoff.cgi
```

Verify logs:

```bash
cat /tmp/led.log
tail -f /tmp/led.log
```

---

## CGI Query Parameters

CGI applications receive HTTP query parameters through environment variables.

The query string is available via:

```text
QUERY_STRING
```

Example parameters:

```text
servoposition=90
led=on
```

The query CGI application:

- parses URL query parameters
- extracts parameter values
- generates browser output
- writes events to `/tmp/cgi.log`

---

### Deploy query CGI application

Copy binary from host:

```bash
scp build/query-cgi/query-cgi rpi:/tmp/
```

Install on Raspberry Pi:

```bash
sudo mv /tmp/query-cgi /usr/lib/cgi-bin/env-cgi.cgi
```

---

### Query CGI tests

Without parameters:

```text
http://192.168.86.72/cgi-bin/env-cgi.cgi
```

With parameters:

```text
http://192.168.86.72/cgi-bin/env-cgi.cgi?servoposition=90&led=on
```

Verify logs:

```bash
cat /tmp/cgi.log
tail -f /tmp/cgi.log
```

---

## Polling Demo

### Deploy polling CGI application

Copy binary from host:

```bash
scp build/polling-demo/polling-time-cgi rpi:/tmp/
```

Install on Raspberry Pi:

```bash
sudo mv /tmp/polling-time-cgi /usr/lib/cgi-bin/time.cgi
```

Test endpoint:

```text
http://192.168.86.72/cgi-bin/time.cgi
```

---

### Deploy polling HTML page

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

## Cross-Origin Resource Sharing (CORS)

Modern browsers restrict JavaScript requests across origins unless the server explicitly allows them.

The polling demo requires enabling CORS headers for CGI responses.

---

### Enable mod_setenv

```bash
sudo lighty-enable-mod setenv
```

---

### Configure CORS headers

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

Restart Lighttpd:

```bash
sudo systemctl restart lighttpd
```

---

### Browser debugging

Browser developer tools can be used to inspect:

- request headers
- response headers
- CORS policy errors
- polling frequency
- CGI request timing

---

## CPU Load Monitoring

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
