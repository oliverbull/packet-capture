#include <stdio.h>
#include <stdint.h>
#include <logging.h>

void logger(const char *str, FILE *logfile, bool terminal) {
    // if terminal is set, we print
    if (terminal) {
        printf("log: %s\n", str);
    }
    // if logfile is not null we also write to file
    if (logfile != NULL) {
        fprintf(logfile, "%s\n", str);
    }
}

void dataLogger(const char *str, FILE *logfile, bool terminal) {
    // if terminal is set, we print
    if (terminal) {
        printf("%s", str);
    }
    // if logfile is not null we also write to file
    if (logfile != NULL) {
        fprintf(logfile, "%s", str);
    }
}

void error(const char * str) {
    // just print for now
    printf("err: %s\n", str);
}