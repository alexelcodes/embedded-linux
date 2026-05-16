# Polling vs GPIO Events

There are two common ways to detect GPIO input changes in embedded Linux applications:

- polling
- GPIO events (interrupt-style handling)

This project demonstrates why GPIO events are usually preferred for responsive and efficient input handling.

---

## Polling

Polling means repeatedly reading the GPIO input state inside a loop.

Typical polling flow:

1. read GPIO state
2. wait some time
3. repeat

Example:

```c
while (1)
{
    value = gpiod_line_get_value(line);

    if (value == 0)
    {
        printf("Button pressed\n");
    }

    usleep(10000);
}
```

---

## Problems With Polling

Polling has several limitations:

- constant CPU activity
- wasted wakeups
- delayed response time
- poor scalability
- inefficient low-power behavior

The polling interval creates a tradeoff:

| Polling Interval | Result                            |
| ---------------- | --------------------------------- |
| short interval   | faster response, higher CPU usage |
| long interval    | lower CPU usage, slower response  |

---

## GPIO Events

GPIO event handling allows the Linux kernel to notify the application when a GPIO edge occurs.

Instead of repeatedly checking the input state, the application waits for events.

Example:

```c
gpiod_line_request_both_edges_events(line, CONSUMER);

gpiod_line_event_wait(line, &timeout);
```

The application sleeps until:

- an event occurs
- timeout expires

This approach is significantly more efficient than polling.

---

## Blocking Behavior

Functions such as:

```c
gpiod_line_event_wait()
```

are blocking.

This means the current thread stops execution while waiting for an event.

If blocking happens inside the main application loop, the rest of the program may stop responding.

---

## Using Threads

This project solves the blocking problem by moving GPIO event handling into a separate POSIX thread.

Architecture:

```text
Main thread
 ├─ LED updates
 └─ application logic

GPIO thread
 └─ waits for button events
```

Benefits:

- responsive main loop
- independent event handling
- cleaner program structure

---

## Switch Bouncing

Mechanical push buttons do not create perfect transitions.

A single button press may generate multiple rapid edges.

Example:

```text
HIGH LOW HIGH LOW HIGH LOW
```

instead of:

```text
HIGH -> LOW
```

This effect is called switch bouncing.

Without debounce logic, one physical press may trigger multiple events.

---

## Software Debounce

This project uses a simple software debounce method:

1. detect GPIO edge
2. ignore edges occurring too soon after the previous event
3. verify final GPIO state after debounce delay

The debounce window is:

```c
#define DEBOUNCE_NS 5000000L
```

which corresponds to:

```text
5 ms
```

---

## Conclusion

GPIO event handling provides better responsiveness and efficiency than polling.

Combining GPIO events with POSIX threads and software debounce creates a more reliable and scalable embedded Linux input handling design.
