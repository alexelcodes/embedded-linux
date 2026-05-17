cmake_minimum_required(VERSION 3.16)

project(rpi_mqtt_button_publisher C)

add_executable(
    rpi_mqtt_button_publisher
    src/main.c
    src/mqtt.c
)

target_include_directories(
    rpi_mqtt_button_publisher
    PRIVATE
    ${CMAKE_SYSROOT}/usr/local/include
)

target_link_directories(
    rpi_mqtt_button_publisher
    PRIVATE
    ${CMAKE_SYSROOT}/usr/local/lib
)

target_link_libraries(
    rpi_mqtt_button_publisher
    paho-mqtt3c
    gpiod
)
