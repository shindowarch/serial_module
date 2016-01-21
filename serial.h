#ifndef __SERIAL_H__
#define __SERIAL_H__

#define TIMEOUT_SEC     ( 0 )
#define TIMEOUT_USEC    ( 1000*100 )     // 100 ms milliseconds

typedef struct
{
    int  baud_rate;
    int  port_fd;
    char parity;
    char stop_bit;
    char flow_ctrl;
    char data_bits;
} GSerial_Dev_Stru;

extern char *gpac_tty_dev[];
extern int GSerial_Cfg( GSerial_Dev_Stru *p_info);
extern int GSerial_Open( char *pc_dev );
extern int GSerial_Snd( int i_handle, char * p_buf, unsigned int buf_len );
extern int GSerial_Rcv( int i_handle, char * p_buf, unsigned int len );

#endif
