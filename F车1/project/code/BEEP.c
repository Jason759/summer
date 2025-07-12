#include"zf_common_headfile.h"
#include "beep.h"

uint8 count_beep = 0;                  // ·äÃùÆ÷¼ÆÊýÆ÷


void beep_on(void)
{
    count_beep=5;
}
void beep_progress(void)
{
    if(count_beep)
    {
        gpio_set_level(BEEP,GPIO_HIGH);
        count_beep--;
    }
    else
    {
        count_beep=0;
        gpio_set_level(BEEP, GPIO_LOW);
    }
}