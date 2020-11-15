cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME time_diagnostics)

set(${PROJECT_NAME}_BOARD nano)
set(ARDUINO_CPU atmega328)
project(${PROJECT_NAME})

# Define additional source and header files or default arduino sketch files
set(
	${PROJECT_NAME}_SRCS

	tests/serial_printf.cpp
	tests/time_diagnostics_tests.cpp
	tests/disable_time_diagnostics_tests.cpp
	tests/timer_tests.cpp

	src/internal/timer.cpp

	time_diagnostics_runner.cpp
)

set(
	${PROJECT_NAME}_HDRS

	tests/time_diagnostics_tests.h
	tests/timer_tests.h

	identities.h
	src/time_diagnostics.h
	src/internal/time_post_receiver.h
	src/internal/timer.h

	tests/serial_printf.h
)

### Additional static libraries to include in the target.
# set(${PROJECT_NAME}_LIBS)

### Main sketch file
# set(${PROJECT_NAME}_SKETCH time_diagnostics.ino)

### Add project directories into the build
# add_subdirectory()

### Additional settings to add non-standard or your own Arduino libraries.
# For this example (libs will contain additional arduino libraries)
# An Arduino library my_lib will contain files in libs/my_lib/: my_lib.h, my_lib.cpp + any other cpp files
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

# For nested library sources replace ${LIB_NAME} with library name for each library
# set(_RECURSE true)

#### Additional settings for programmer. From programmers.txt
set(${PROJECT_NAME}_PROGRAMMER arduino)
set(${PROJECT_NAME}_PORT /dev/ttyUSB0)
set(nano.upload.speed 115200)

## Verbose build process
# set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND "${ARDUINO_SDK_PATH}/hardware/tools/avr/bin/avrdude"
	ARGS "-C${ARDUINO_SDK_PATH}/hardware/tools/avr/etc/avrdude.conf" -patmega328p -c${${PROJECT_NAME}_PROGRAMMER} -P${${PROJECT_NAME}_PORT} -b${nano.upload.speed} -D -Uflash:w:${PROJECT_NAME}.hex:i
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Uploading to Arduino..."
)
