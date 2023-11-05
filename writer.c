#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sleep_ms = 100;

    // If there's a command line argument, use it as the sleep time
    if (argc > 1) {
        sleep_ms = atoi(argv[1]);
        if (sleep_ms <= 0) {
            fprintf(stderr, "Invalid sleep duration provided.  Using default of 100 (ms.)\n");
            sleep_ms = 100;
        }
    }
    else {
        fprintf(stderr, "Using default sleep value of 100 (ms)\n");
    }
    
    // Delete the log.txt file if it exists
    remove("./logs/log.txt");

    FILE *f = fopen("./logs/log.txt", "w");
    if (f == NULL) {
        printf("Error opening file (./logs/log.txt)!\n");
        return 1;
    }

    int count = 0;
    while(1) {
        time_t timestamp = time(NULL);
        fprintf(stderr, "[%ld] [%d] Wrote to stderr.\n", timestamp, count);
        fprintf(f, "[%ld] [%d] Wrote to stderr.\n", timestamp, count);
        fflush(f);
        usleep(sleep_ms * 1000); 
        count++;
    }

    fclose(f);
    return 0;
}
