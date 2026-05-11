cmake_minimum_required(VERSION 3.16)

project(rpi_gpio_button_led C)

add_executable(${PROJECT_NAME} src/button-led.c)

# Link this executable with libgpiod (sysroot)
target_link_libraries(${PROJECT_NAME} gpiod)
