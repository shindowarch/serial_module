/* ============================================================================
 Name        :     serial.c
 Copyright   :     AirBridge IOT Technology Co.Ltd
 Description :     Serial prot operation connection meter; 
 Author      :
 Date        :
 ============================================================================ */

#include <fcntl.h> 
#include <termios.h>        
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "serial.h"

char *gpac_tty_dev[] =
{
    "/dev/ttySP0",
    "/dev/ttySP1",
    "/dev/ttySP2",
    "/dev/ttySP3",
    "/dev/ttySP4"
};

int GSerial_GetBaud(unsigned long int baud_rate)
{
    switch(baud_rate)
    {
    case 600:
        return B600;
    case 1200:
        return B1200;
    case 2400:
        return B2400;
    case 4800:
        return B4800;
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    default:
        break;
    }
    return B9600;
}

int GSerial_Cfg( GSerial_Dev_Stru * p_info)
{
    struct termios old_opt,new_opt;
    int baud_rate;
    int result;

    memset(&old_opt,0,sizeof(old_opt));
    memset(&new_opt,0,sizeof(new_opt));

//    cfmakeraw(&new_opt);
    tcgetattr(p_info->port_fd,&old_opt);
    baud_rate = GSerial_GetBaud(p_info->baud_rate);
    cfsetispeed(&new_opt,baud_rate);
    cfsetospeed(&new_opt,baud_rate);

    new_opt.c_cflag |=CLOCAL;
    new_opt.c_cflag |=CREAD;

    
// data bits
    new_opt.c_cflag &=~CSIZE;
    switch(p_info->data_bits)
    {
        case 5:
            new_opt.c_cflag |=CS5;
            break;
        case 6:
            new_opt.c_cflag |=CS6;
            break;
        case 7:
            new_opt.c_cflag |=CS7;
            break;
        case 8:
            new_opt.c_cflag |=CS8;
            break;
        default:
            new_opt.c_cflag |=CS8;
        }
// parity
    switch(p_info->parity)
    {
        case 0:     // none
            new_opt.c_cflag &=~PARENB;
            break;
        case 1:     // odd
            new_opt.c_cflag |=PARENB;
            new_opt.c_cflag |=PARODD;
            break;
        case 2:     // even
            new_opt.c_cflag |=PARENB;
            new_opt.c_cflag &=~PARODD;
            break;
        default :     // none
            new_opt.c_cflag &=~PARENB;
    }
// stop bits
    if(p_info->stop_bit==2)
        new_opt.c_cflag |=CSTOPB;
    else
        new_opt.c_cflag &=~CSTOPB;
//flow control
    switch(p_info->flow_ctrl)
    {
        case 0:     // none
            new_opt.c_cflag &=~CRTSCTS;
            break;
        case 1:     // hard ware
            new_opt.c_cflag |=CRTSCTS;
            break;
        case 2:     // soft ware
            new_opt.c_iflag |= IXON | IXOFF | IXANY;  
            break;
    }

    //修改输出模式，原始数据输出
    new_opt.c_oflag &=~OPOST;
    //修改控制字符，读取字符的最少个数为1
    new_opt.c_cc[VMIN]=1;
    //修改控制字符，读取第一个字符等待1*（1/10）s
//    new_opt.c_cc[VTIME]=1;
    //如果发生数据溢出，接收数据，但是不再读取
//    tcflush(p_info->port_fd,TCIFLUSH);

    result=tcsetattr(p_info->port_fd,TCSANOW,&new_opt);
    if(result==-1)
    {
        perror("Cannot set the serial port parameters");
        return -1;
    }

    tcgetattr(p_info->port_fd,&old_opt);
    
    return result;
}

int GSerial_Open( char* pc_dev )
{
    return open( pc_dev, O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY );
}

int GSerial_Close( int i_fdcom )
{
    close(i_fdcom);
    return 0;
}

int GSerial_Snd( int i_fdcom, char * pc_buf, unsigned int ui_len )
{
    int len = 0, err = 0;
    int fs_sel;
    fd_set fs_write;
    struct timeval tv_timeout;

    if ( i_fdcom < 0 )
    {
        return -1;
    }
    FD_ZERO(&fs_write);
    FD_SET(i_fdcom, &fs_write);
    tv_timeout.tv_sec = TIMEOUT_SEC;
    tv_timeout.tv_usec = TIMEOUT_USEC;

    do
    {
        fs_sel = select(i_fdcom+1, NULL, &fs_write, NULL, &tv_timeout);
        if (fs_sel)
        {
            len = write(i_fdcom, pc_buf, ui_len);
            if( len <= 0 )
            {
                if ( err++ > 3 )
                {
                    printf("%s: Com Serial Snd Fail.\n", __FUNCTION__ ); 
                    tcflush(i_fdcom, TCOFLUSH);
                    return -1;
                }
            }
            ui_len  = ui_len - len;
            pc_buf  = pc_buf + len;
        }
    } while ( ui_len > 0 );

    return 0;
}

int GSerial_Rcv( int i_fdcom, char * pc_buf, unsigned int ui_len )
{
    fd_set fs_read;
    struct timeval tv_timeout;

    if( (pc_buf == NULL)||(ui_len < 0) )
    {
        return -1;
    }

    FD_ZERO(&fs_read);
    FD_SET(i_fdcom, &fs_read);
    tv_timeout.tv_sec  = TIMEOUT_SEC;
    tv_timeout.tv_usec = TIMEOUT_USEC;
    
    if(select(i_fdcom+1, &fs_read, NULL, NULL, &tv_timeout))
    {
        return read(i_fdcom, pc_buf, ui_len);
        //GSys_Dump( p_buf, readlen );
    }

    return -1;
}


