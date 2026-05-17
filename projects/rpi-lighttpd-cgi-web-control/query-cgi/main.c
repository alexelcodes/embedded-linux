#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yuarel.h"

int main(void)
{
    // get query string from CGI environment
    const char *query_env = getenv("QUERY_STRING");

    // send HTTP response header
    printf("Content-type: text/html\r\n\r\n");
    printf("<html><body>\n");

    // handle missing query
    if (!query_env)
    {
        printf("<p>QUERY_STRING is not set</p>\n");
        printf("</body></html>\n");
        return 0;
    }

    // copy query because parser modifies the string
    char query_buf[256];
    strncpy(query_buf, query_env, sizeof(query_buf) - 1);
    query_buf[sizeof(query_buf) - 1] = '\0';

    // parse query parameters into key-value pairs
    struct yuarel_param params[10];
    int n = yuarel_parse_query(query_buf, '&', params, 10);

    // pointers to extracted parameters
    const char *servo = NULL;
    const char *led = NULL;

    // find needed parameters
    for (int i = 0; i < n; i++)
    {
        if (strcmp(params[i].key, "servoposition") == 0)
        {
            servo = params[i].val;
        }
        else if (strcmp(params[i].key, "led") == 0)
        {
            led = params[i].val;
        }
    }

    int wrote_log = 0;
    FILE *f = NULL;

    // open log only if something to write
    if (servo || led)
    {
        f = fopen("/tmp/cgi.log", "a");
    }

    // handle servoposition parameter
    if (servo)
    {
        printf("<p>servoposition = %s</p>\n", servo);
        if (f)
        {
            fprintf(f, "servoposition=%s ", servo);
            wrote_log = 1;
        }
    }

    // handle led parameter
    if (led)
    {
        printf("<p>led = %s</p>\n", led);
        if (f)
        {
            fprintf(f, "led=%s ", led);
            wrote_log = 1;
        }
    }

    // no valid parameters case
    if (!servo && !led)
    {
        printf("<p>No supported parameters found</p>\n");
    }

    // finalize log entry
    if (f)
    {
        if (wrote_log)
        {
            fprintf(f, "\n");
        }
        fclose(f);
    }

    printf("</body></html>\n");
    return 0;
}
