#include "cybtldr_uart.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/fcntl.h>

int fd;

int OpenConnection(void)
{
	int err = 0;
	struct termios term;

	fd = open(UARTFILE, O_RDWR);
	if(fd <= 0)
	{
		return -1;
	}

	sleep(3);
	
 	// Get the current options:
 	tcgetattr( fd, &term );
 	
    //
    // Input flags - Turn off input processing
    //
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    //
    //term.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    term.c_iflag &= ~(BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
    
    //
    // Output flags - Turn off output processing
    //
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    //
    // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
    //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
    term.c_oflag = 0;
 
    //
    // No line processing
    //
    // echo off, echo newline off, canonical mode off, 
    // extended input processing off, signal chars off
    //
    term.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
 
    //
    // Turn off character processing
    //
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    //
    term.c_cflag &= ~(CSIZE | PARENB);
    term.c_cflag |= CS8;
 
    //
    // One input byte is enough to return from read()
    // Inter-character timer off
    //
    term.c_cc[VMIN]  = 0; // 0 character is enough
    term.c_cc[VTIME] = 10; // 1 second timeout
 
    
 	// Set the input/output speed to 9600bps
 	cfsetispeed( &term, B9600 );
 	cfsetospeed( &term, B9600 );
  
 	// Now set the term options (set immediately)
 	tcsetattr(fd, TCSAFLUSH, &term);
	
    tcflush(fd, TCIOFLUSH);
    
	return CYRET_SUCCESS;
}


int CloseConnection(void)
{
	close(fd);
	
	return CYRET_SUCCESS;
}


int WriteData(unsigned char* wrData, int byteCnt)
{
    int err;

    err = write(fd, wrData, byteCnt);
	
	usleep(60000);
	
    if (err < 0) {
        return CYRET_ERR_FILE;
    }

	return CYRET_SUCCESS;
}


int ReadData(unsigned char* rdData, int byteCnt)
{
    int err;

    err = read(fd, rdData, byteCnt);

    if (err < 0) {
        return CYRET_ERR_FILE;	  
    }

    return CYRET_SUCCESS;
}


