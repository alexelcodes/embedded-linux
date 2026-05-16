cmake_minimum_required(VERSION 3.16)

project(rpi_hardware_pwm_servo_control C)

add_executable(${PROJECT_NAME}
    src/main.c
    src/pwm_hw.c
)

target_link_libraries(${PROJECT_NAME}
    rt
)
