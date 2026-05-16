cmake_minimum_required(VERSION 3.16)

project(rpi_multipurpose_knob_controller C)

add_executable(
    rpi_multipurpose_knob_controller
    src/main.c
    src/controller.c
    src/button.c
    src/knob.c
    src/mode_led.c
    src/servo.c
    src/ws2812_led.c
)

target_link_libraries(
    rpi_multipurpose_knob_controller
    gpiod
    pthread
)
