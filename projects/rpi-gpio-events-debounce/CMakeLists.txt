cmake_minimum_required(VERSION 3.16)

project(rpi_gpio_events_debounce C)

add_executable(
    rpi_gpio_events_debounce
    src/gpio-events-debounce.c
)

target_link_libraries(
    rpi_gpio_events_debounce
    gpiod
    pthread
)
