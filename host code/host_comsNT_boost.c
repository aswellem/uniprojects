
/*****************************************************************

file: host_comsNT_boost.c

Sample host program to demonstrate communication with IlMatto.
Use together with embedded_coms_boost.c running on IlMatto.


Author: Klaus-Peter Zauner, 2014
Updated: William Barber, 2017
Licence: GPL version 2

Based on the code made available 
by Teunis van Beelen under GPL2 at:

   http://www.teuniz.net/Timer_code/
and by William Barber under MIT at:
   http://www.github.com/WillB97/VCP/

To compile on Linux:
  gcc host_comsNT_boost.c -Wall -s -o2 -o hostcoms

To compile on Windows:
  gcc host_comsNT_boost.c -Wall -s -o2 -o host.exe

*****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/*  ::: MODIFIED :::  [WBB-14.12.2017] */
#include "VCP.h"


#define DELAY_US 100000
#define KEY_IN_BUFLEN   256


void print_serial(void);
char *get_line (char *s, size_t n, FILE *f);
char *p;



#define SER_IN_BUFLEN   4096
char cport_nr[32]=/*"/dev/ttyUSB0"; */ "\\\\.\\COM5";
char serbuf[SER_IN_BUFLEN];


#define CMD_BUFFSIZE 50
void send_command(const char *cmd, int val);



int main()
{
  int bdrate=9600;       /* 9600 baud */

  char line[KEY_IN_BUFLEN];

  /*   ::: MODIFIED :::  [WBB-14.12.2017] */
  if(init_COM_port(cport_nr, bdrate, 0, 1, 0))
  {
    fflush (stdout);
    return(1);
  }


  printf("\nEnter an Integer between 0 and 10. \n");
  //delay(5);
  //printf("To cancel type Cancel.\n");
  

  for(;;){
      if (!get_line(line, KEY_IN_BUFLEN, stdin)){	  
	  print_serial();
 
	  continue;
      } 
      
      printf( "Command:  \"%s\" -->  ", line );
      
      if( !strcmp( "Cancel", line )) {
      	  break;
      } else if( !strcmp( "6", line )) {
	  send_command( "P", 6);	  

      }	
	  else if (!strcmp ("2", line)){
		  send_command ("P", 2);
	  }
	  else if (!strcmp ("3", line)){
		  send_command( "P", 3);
	  }
	  else if (!strcmp ("4", line)){
		  send_command("P", 4);
	  }
	  else if( !strcmp( "0", line )) {
	  send_command( "P", 0);
      }	
	  else if( !strcmp( "5", line )) {
	  send_command( "P", 5);
	   }	
	   else if( !strcmp( "7", line )) {
	  send_command( "P", 7);
	   }	
	   else if( !strcmp( "8", line )) {
	  send_command( "P", 8);
      } 
	  else if( !strcmp( "9", line )) {
	  send_command( "P", 9);
      } 
	 	else if( !strcmp( "10", line )) {
	  send_command( "P", 10);
		}
	else{
		printf("ERROR \n");
	}

      print_serial();	  
  }
    
  	scanf("%*s");	
	close_COM_port();
  return(0);
}


void send_command(const char *cmd, int val) 
{
    char cmdstring[CMD_BUFFSIZE];
    
    snprintf(cmdstring,CMD_BUFFSIZE, "%s %u\r\n", cmd, val);
    /*   ::: MODIFIED :::  [WBB-14.12.2017] */
    COM_port_send_buffer(cmdstring, strlen(cmdstring));	    

}



void print_serial(void)
{
    int i, n;

    /*   ::: MODIFIED :::  [WBB-14.12.2017] */
    n = poll_COM_port(serbuf, SER_IN_BUFLEN-1);

    if(n > 0)
    {
      serbuf[n] = 0;   /* always put a "null" at the end of a string! */
    
      for(i=0; i < n; i++)
      {
        if(serbuf[i] < 32 && serbuf[i] != '\n')  /* replace control-codes by dots */
        {
          serbuf[i] = '.';
        }
      }
    
      /* printf("received %i bytes: %s\n", n, (char *)serbuf); */
      printf("%s\n", (char *)serbuf);
      fflush (stdout);
    }

    printf("received %i bytes\n", n);
    
}



/* Read a line from stdin 

   Thomas Wolf at
   http://home.datacomm.ch/t_wolf/tw/c/getting_input.html

 */
char *get_line (char *s, size_t n, FILE *f)
{
  char *p = fgets (s, n, f);

  if (p != NULL) {
    size_t last = strlen (s) - 1;

    if (s[last] == '\n') s[last] = '\0'; 
  }
  return p;
}
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
}
