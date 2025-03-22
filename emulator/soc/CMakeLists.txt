# SoC libraries for the emulator

# External libs
find_package(PkgConfig REQUIRED)
pkg_check_modules(UNICORN REQUIRED unicorn)

add_library(fireplace_soc STATIC fb/fb.c gpio/exynos_gpio.c gpio/gpio_alive.c hardware_buttons/hardware_buttons.c memmap.c soc.c uart/uart.c usb/usb.c)
target_include_directories(fireplace_soc PRIVATE ${UNICORN_INCLUDE_DIRS})
target_link_libraries(fireplace_soc PRIVATE fireplace_include ${UNICORN_LIBRARIES})
