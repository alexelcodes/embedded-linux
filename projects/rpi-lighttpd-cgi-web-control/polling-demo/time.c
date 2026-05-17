// could compile locally on Raspberry Pi
// gcc time.c -o time.cgi
// copy executable to /usr/lib/cgi-bin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    time_t rawtime;
    char textbuf[30];

    printf("Status: 200 OK\r\n");

    // encapsulate current time info to JSON
    printf("Content-type: application/json\r\n\r\n");
    printf("{ \"time\": ");

    time(&rawtime);
    ctime_r(&rawtime, textbuf);

    // remove trailing newline from ctime_r()
    textbuf[strlen(textbuf) - 1] = 0;

    printf("\"%s\"", textbuf);
    printf("}\n");
}
