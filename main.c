/***************************************************************************************
* COPYRIGHT (c) <2015>, Airbridge IOT Technology Co.,Ltd..
* Filename:  main.c
* Version:  1.0
* Description:  
* Created:  01/19/2016 09:51:54 PM
* Revision:  none
* Compiler:  gcc
* Author:  Wen Zhen (Z.S.Wen), zhen.wen@airbridge.com.cn
* Function List:
*              
* History:
*       when              who            why              what
*    -------------     -----------   ------------    -------------
*     01/19/2016        Z.S.Wen       Create         
****************************************************************************************/
#include <stdio.h>
#include "serial.h"

int gi_wtdg_handle;

int GWtdg_Init(void)
{
    GSerial_Dev_Stru st_wtdg;

    gi_wtdg_handle = GSerial_Open(gpac_tty_dev[4]);
    if( gi_wtdg_handle < 0)
    {
        printf("Open Dev[%s] Fail!\n", gpac_tty_dev[4]);
        return -1;
    }

    st_wtdg.port_fd   = gi_wtdg_handle;
    st_wtdg.baud_rate = 9600;
    st_wtdg.data_bits = 8;
    st_wtdg.parity    = 0;
    st_wtdg.stop_bit  = 1;
    st_wtdg.flow_ctrl = 0;
    GSerial_Cfg(&st_wtdg);

    printf("Cfg Dev[%s] Success.\n", gpac_tty_dev[4]);
    
    return 0;
}

int main(int argc, const char *argv[])
{
    unsigned char uc_test[9] = {0x11, 0x22, 0x33, 0x44 ,0x55, 0x66, 0x77, 0x88, 0x99};
    char ac_test[] = "This is a test!\n";

    GWtdg_Init();
    GSerial_Snd(gi_wtdg_handle, (char*)uc_test, sizeof(uc_test));
    GSerial_Snd(gi_wtdg_handle, ac_test, sizeof(ac_test));

    return 0;
}
