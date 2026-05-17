add_executable(gpio-led-on led_on.c)
target_link_libraries(gpio-led-on gpiod)

add_executable(gpio-led-off led_off.c)
target_link_libraries(gpio-led-off gpiod)
