#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

int set_serial(int fd,int nSpeed,int nBits,char nEvent,int nStop);
int serialInit(char *portPath,int nSpeed);


#ifdef __cplusplus
}
#endif

#endif
