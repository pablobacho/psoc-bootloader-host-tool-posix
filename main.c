
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cybtldr_parse.h"
#include "cybtldr_command.h"
#include "cybtldr_api.h"
#include "cybtldr_api2.h"
#include "cybtldr_uart.h"

CyBtldr_CommunicationsData commData;
char * cyacd_file;

void ProgramProgressUpdate(unsigned char arrayId, unsigned short rowNum){
    putchar('.'); fflush(stdout);
}

int main(int argc, char** argv) {
    int err;

    commData.OpenConnection = &OpenConnection;
    commData.CloseConnection = &CloseConnection;
    commData.ReadData = &ReadData;
    commData.WriteData = &WriteData;
    commData.MaxTransferSize = 32;
    
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'p':
                    cyacd_file = argv[++i];
                    printf("Programming\n"); fflush(stdout);
                    err = CyBtldr_Program(cyacd_file, NULL, &commData, &ProgramProgressUpdate);
                    printf("\n");
                break;
            }
        }
    }

    if(err != 0) {
        printf("ERROR: Return code %#8x\n", err);
    } else {
        printf("Finished\n");
    }
    
    return (err);
}
