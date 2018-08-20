#include <system/system.h>
#include <driver/buffer.h>
#include <json/json.h>


void system_init()
{
    buffer_init();
    json_init();
    

}
