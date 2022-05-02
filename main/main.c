#include "dual-core-implementation.h"
#include "priority-tasks.h"
#include "equal-priority.h"
#include "esp32-wifi.h"
#include "lcd-display.h"
void app_main(void)
{
    dual_core_implementation();
    priority_tasks();
    esp32_wifi();
    equal_priority();
    lcd_display();
}
