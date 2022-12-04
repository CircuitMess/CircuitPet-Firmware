cmake_minimum_required(VERSION 3.20)
project(CircuitPet-Firmware)
set(PORT /dev/ttyUSB0)
set(DEVICE cm:esp32:circuitpet)
set(CMAKE_CXX_STANDARD 17)

set(ROOT ${CMAKE_SOURCE_DIR})
set(ARDUINO-CLI arduino-cli)
set(ARDUINO-CLI-UPLOAD arduino-cli)

if(EXISTS /mnt/c/windows)
    set(WSL2 1)
    #as wsl2 does not have usb port access, we use the arduino cli on wls2 for building, and the windows tool for uploading
    #WSL container needs the following:
    # install the CLI tool
    # curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/local/bin sh
    # add cm to its packages
    # run arduino-cli config init
    # edit the arduino-cli.yaml, add the following to board_manager > additional_urls
    # https://raw.githubusercontent.com/CircuitMess/Arduino-Packages/master/package_circuitmess.com_esp32_index.json
    # run arduino-cli core update-index
    # run arduino-cli core install cm:esp32
    # change package to use python 3
    # run sed -i -e 's/=python /=python3 /g' ~/.arduino15/packages/cm/hardware/esp32/*/platform.txt
    # install pyserial
    # run sudo apt install python3-pip
    # run pip install pyserial
endif()

if (WIN32 OR CYGWIN)
    set(Arduino15 $ENV{APPDATA}/../Local/Arduino15)
    set(ArduinoLibs $ENV{USERPROFILE}/OneDrive/Documents/Arduino/libraries)

    string(REPLACE "\\" "/" Arduino15 ${Arduino15})
    string(REPLACE "\\" "/" ArduinoLibs ${ArduinoLibs})

    if (CYGWIN)
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" Arduino15 ${Arduino15})
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" ArduinoLibs ${ArduinoLibs})
        string(REGEX REPLACE "^/cygdrive/([A-Za-z])" "\\1:/" ROOT ${ROOT})
    endif ()
else ()
    set(Arduino15 ~/.arduino15)
    set(ArduinoLibs ~/Arduino/libraries)
    if (WSL2)
        get_filename_component(ARDUINO-CLI ~/local/bin/arduino-cli ABSOLUTE )
        message(${ARDUINO-CLI})
        set(ARDUINO-CLI-UPLOAD arduino-cli.exe)
        set(PORT COM10)
    endif()
endif ()

include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/cores/esp32)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/esp32)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/freertos)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/spi_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/nvs_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/log)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/driver)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/tools/sdk/include/driver/driver)

# Include directories
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/Wire/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/FS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/SD/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/SPI/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/WiFi/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/SPIFFS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/LovyanGFX/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/CircuitOS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/Update/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/Chatter-Library/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/CircuitOS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.5.0/libraries/glm)

include_directories(${ArduinoLibs}/CircuitPet-Library/src)

# Source directories
file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.c" "src/**.hpp" "src/**.h")

# Defines
add_definitions(-DDEBUG)
add_definitions(-DESP32)
add_definitions(-DCIRCUITOS_FREERTOS)
add_definitions(-DCIRCUITOS_NVS)
add_definitions(-DCIRCUITOS_LOVYANGFX)
add_definitions(-DCIRCUITOS_PIEZO_ESP32)
add_definitions(-DCIRCUITOS_PIEZO_ESP32_PWM_CHANNEL=0)
add_definitions(-DRADIOLIB_BUILD_ARDUINO)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(BUILD ${ROOT}/build)
set(UPLOAD_FOLDER ${BUILD})

if (WSL2)
    #save project output files internal to the wsl file system and let a post build move them back to the windows partition
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /tmp/${CMAKE_PROJECT_NAME}/build)
    #change the upload folder path to relative to the windows cli tool, not wsl namespace
    string(REGEX REPLACE "^/mnt/([A-Za-z])" "\\1:" UPLOAD_FOLDER ${UPLOAD_FOLDER})
endif()

SET_SOURCE_FILES_PROPERTIES(${CMAKE_PROJECT_NAME}.ino PROPERTIES LANGUAGE CXX)

add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf
        COMMAND ${ARDUINO-CLI} compile --fqbn ${DEVICE} --output-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_PROJECT_NAME}.ino
        DEPENDS ${CMAKE_PROJECT_NAME}.ino ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(CMBuild
        SOURCES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf)

if (WSL2)
    #Copy the build output to a different location on wsl2
    add_custom_command(
            TARGET CMBuild POST_BUILD
            COMMAND cp -r ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${ROOT}
    )
endif()

add_custom_target(CMUpload ALL
        COMMAND ${ARDUINO-CLI-UPLOAD} upload  -v --fqbn ${DEVICE} -p ${PORT} --input-dir ${UPLOAD_FOLDER}
        DEPENDS CMBuild
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME}.bin ${CMAKE_PROJECT_NAME}.ino ${SOURCES})