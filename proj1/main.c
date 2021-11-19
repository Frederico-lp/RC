#include "application.h"

int main(int argc, char** argv)
{
    appL = (applicationLayer *)malloc(sizeof(applicationLayer));
    
    // linkL = (linkLayer *)malloc(sizeof(linkLayer));
    linkL.timeout = 20;
    linkL.sequenceNumber = 0;

    char *port;
    char *img;

    int fd, res;
    struct termios oldtio, newtio;
    char buf[255];

    /*
    1ºcriar estrutuas de dados -> missing some?
    2ºler os argumentos q lhe vamos passar -> DONE
    3ºabrir (llopen) -> TO_DO FROM THIS POINT DOWN
    4º
    SE FOR TRANSMITTER:
    -abrir ficheiro
    -mandar control packet (app)
    fazer ciclo para criar data packet (app) e fazer llwrite deste
    -mandar control packet (do fim)
    SE FOR RECIEVER:
    fazer ciclo while durante o tempo todo a ler os packets

    */

    // argc = 2 -> programa + porta = receptor
    // argc = 3 -> programa + porta + path da imagem = emissor

    if(argc < 2 || argc > 3){
       printf("Invalid Usage:\tInvalid number of arguments");
       exit(1);
    }

    for(int i = 1; i < argc; i++){
        if(strcmp(MODEMDEVICE_0, argv[i]) == 0 || strcmp(MODEMDEVICE_1, argv[i]) == 0){
            port = argv[i];
        }
        else {
			int accessableImg = access(argv[i], F_OK);
			if (accessableImg == 0) {
				img = argv[i];
			} 
            else {
				printf("Invalid Usage:\tInvalid arguments\n");
				exit(1);
			}
		}
    }

    if(img == NULL){ // Open comunications for receiver
        if(llopen(port, RECEIVER)){
            unsigned char *msg;
            unsigned char *msg_start;
            unsigned char *msg_end;

            msg_start = llread(port, argv[1]); // TO-DO

            if(msg_start[0] == REJ){
                write(STDOUT_FILENO, "Received start\n", 25);
                msg = llread(port, argv[1]);
                if(msg[0] != ESCAPE_OCTET){
                    msg_end = llread(port, argv[1]); // MISSING FLAG 2
                    if(msg_end[0] == 2){
                        write(STDOUT_FILENO, "Received end", 25);
                    }

                }
            }
        }
    }
    else if(llopen(port, TRANSMITTER)){ // Open comunications for transmitter

    } 

    //consoante o path for null ou nao, abrir recetor ou transmissor respetivamente
    

    /*
    if (tcsetattr(fd, TCSANOW, &oldtio) == -1) {
		perror("tcsetattr");
		exit(-1);
	}

	close(fd);
    */

	return 0;
}

int assemble_pic(unsigned char * pic_buffer){
    FILE * pic;
    pic = fopen("penguin.gif", "wb+");

    int picSize = pic_buffer[2]*255 + pic_buffer[3];
    unsigned char * aux = malloc(picSize);

    for(int i = 0; i < picSize; i++){
        aux[i] = pic_buffer[i+4];
    }

    fwrite(aux, 1, picSize-1, pic);
    fclose(pic);

    return 0;
}

unsigned char * process_pic(char* path, int* size){
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long int lenght = ftell(f);
    unsigned char * control = (unsigned char*)malloc(lenght+4);
    unsigned char *buffer = (unsigned char *)malloc(lenght);

    fseek(f, 0, SEEK_SET);
    fread(buffer, 1, lenght, f);
    fclose(f);

	control[0] = C_REJ;	// C
	control[1] = 0; // N
	control[2] = lenght / 255;	// L2
	control[3] = lenght % 255; // L1

	for (int i = 0; i < lenght; i++) {
		control[i+4] = buffer[i];
	}

	return control;
}