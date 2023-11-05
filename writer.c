#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main() {
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
        usleep(100 * 1000); // Sleep for 100ms
        count++;
    }

    fclose(f);
    return 0;
}
