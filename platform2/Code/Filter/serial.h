#ifndef __SERIAL_H__
#define __SERIAL_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define SERIAL_ERROR_OPEN        -1
#define SERIAL_ERROR_READ        -2
#define SERIAL_ERROR_WRITE       -3
#define SERIAL_ERROR_BAUDRATE    -4
#define SERIAL_ERROR_SETTING     -5
#define SERIAL_INVALID_RESOURCE  -6
#define SERIAL_INVALID_FILE      -7


#define FLOW_CONTROL_NONE         0
#define FLOW_CONTROL_HARDWARE     1
#define FLOW_CONTROL_SOFTWARE     2

#define PARITY_NONE  'N'
#define PARITY_EVEN  'E'
#define PARITY_ODD   'O'
#define PARITY_SPACE 'S'
#define PARITY_MARK  'M'

int serial_open(int port, int baud_rate);


int serial_close(int file_descriptor);

int serial_open_file(char *device_filename, int baud_rate);


int serial_set_attr(int file_descriptor, int data_bits, char parity, int stop_bits, int flow_ctrl);

int serial_set_timeout(int file_descriptor, int timeout);

int serial_set_baud_rate(int file_descriptor, int baud_rate) ;

int serial_flush(int file_descriptor);

int serial_data_available(int file_descriptor, unsigned int timeout_millisec);


int serial_send(int file_descriptor, void *buffer, size_t data_len);

int serial_receive(int file_descriptor, void *buffer,size_t data_len);



#ifdef __cplusplus
}
#endif


#endif 
