#include "link.h"

#define ESCAPE_OCTET   0X7d

typedef struct {
int fileDescriptor; /*Descritor correspondente à porta série*/
int status;         /*TRANSMITTER | RECEIVER*/
} applicationLayer;

applicationLayer *appL;

int llopen(char port,int flag);

int llwrite(int fd, char * buffer, int length);

int llread(int fd, char * buffer);

int llclose(int fd);

void create_packet();

