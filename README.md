Note:- All the information about the variables are already commented in the code the following documentation only contains the theory

## Folder Structure

```
.
├── CMakeLists.txt
├── components
│   ├── dual-core-implementation
│   │   ├── CMakeLists.txt
│   │   ├── dual-core-implementation.c
│   │   └── include
│   │       └── dual-core-implementation.h
│   ├── eps32-wifi
│   │   ├── CMakeLists.txt
│   │   ├── esp32-wifi.c
│   │   ├── include
│   │   │   └── esp32-wifi.h
│   │   └── Kconfig.projbuild
│   ├── equal-priority
│   │   ├── CMakeLists.txt
│   │   ├── equal-priority.c
│   │   └── include
│   │       └── equal-priority.h
│   ├── lcd-display
│   │   ├── CMakeLists.txt
│   │   ├── include
│   │   │   └── lcd-display.h
│   │   └── lcd-display.c
│   └── priority-tasks
│       ├── CMakeLists.txt
│       ├── include
│       │   └── priority-tasks.h
│       └── priority-tasks.c
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md
```

1. Dual Core

## Table Of Contents

1.  [Introduction](#1)
2.  [New Project](#2)
3.  [Implementation](#3)

### <a id="1">1. Introduction</a>

ESP32 module has two cores. Each core can be used separately. In this example, we are going to use one core for logging Hello world and a second core to blink LED light

### <a id="2">2. New Project</a>

> Connect your board to your computer or laptop before continuing furthur
> {: .prompt-warning }

- _Cntrl + Shift + P_ in VSCode will open a drop down ,type _ESP-IDF:New Project_
- Project Name:_dual-core-implementation_
- Choose ESP-IDF Board :Custom Board
- ESP32 Target: ESP32 Module
- Click on Choose template
- Chose ESP-IDF by clicking on extension
- Click on sample_project
- Click on Create Project using the template sample_project .
- To make sure that Intellisense works properly Go to VSCode --> .vscode folder --> settings.json
  Change the value of C_Cpp.intelliSenseEngine to "Default"
- Add following lines to _c_cpp_properties.json_ in .vscode folder to disable #include errors

```json
"compileCommands": "${workspaceFolder}/build/compile_commands.json"
```

- Run the following commands

```bash
$ get_idf
$ mkdir -p build && cd build
$ cmake .. -G Ninja
$ ninja build && ninja flash
$ ninja monitor
```

### <a id="3">3.Implementation</a>

#### 1. Dual Core

To use cores separately we need to create Tasks and pin them down to a core
Before we go any further we will need two callback functions one is of hello world another is of blinking LED light. Once the flash is done each core is assigned a task to do which is visible when we monitor the output.

```c
BaseType_t xTaskCreatePinnedToCore(
    TaskFunction_t pvTaskCode, // Function that should be called
    const char * const pcName,  // Name of the task (for debugging)
    const uint32_t usStackDepth, // Stack size (bytes)
    void * const pvParameters,  // Parameter to pass    
    UBaseType_t uxPriority,  // Task priority
    TaskHandle_t * const pvCreatedTask,   // Task handle
    const BaseType_t xCoreID) // Core you want to run the task on (0 or 1)
```

The function xTaskCreatePinnedToCore takes a task function, name of the task, stack depth, PV parameters, priority and id.

#### 2. Priority in single core

There is a need to create two task handle to have access to the tasks.

TaskHandle_t hello_world;
TaskHandle_t blink_led;

Task priority vary from `0 to configMAX_PRIORITIES - 1` where `configMAX_PRIORITIES - 1` is the highest priority

There is a little change in the code from the last one here we need to use one core and a different priority. So whenever there is a task that needs to be completed the one with higher priority will be given preference.

#### 3. Equal Priority

Here the two tasks have equal priority so ESP32 uses Round Robin scheduling for the same

#### 4.WIFI

Change the id and pass word using `ninja menuconfig`

We are using following Wifi authentication methods

```
ESP_WIFI_AUTH_OPEN
ESP_WIFI_AUTH_WEP
ESP_WIFI_AUTH_WPA_PSK
ESP_WIFI_AUTH_WPA2_PSK
ESP_WIFI_AUTH_WPA_WPA2_PSK
ESP_WIFI_AUTH_WPA3_PSK
ESP_WIFI_AUTH_WPA2_WPA3_PSK
ESP_WIFI_AUTH_WAPI_PSK
```

There are three major user defined components

- WIFI_SSID
  - Your network SSID
- WIFI_PASS
  - Network Password
- MAXIMUM_RETRY
  - Number of times esp32 should retry connecting

#### 4.LCD Display

Connections
| LCD Pins | ESP32 Pins |
|----------|------------|
| VSS      | GND        |
| VDD      | +5V        |
| V0       | GND        |
| RS       | 33         |
| RW       | 5          |
| E        | 19         |
| D0       | 12         |
| D1       | 13         |
| D2       | 14         |
| D3       | 18         |
| D4       | 25         |
| D5       | 26         |
| D6       | 27         |
| D7       | 32         |
| A        | +5V        |
| K        | GND        |

