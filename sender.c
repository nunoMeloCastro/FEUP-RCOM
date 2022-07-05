/*
 * sender.c
 * Non-Canonical Input Processing 
 * RC @ L.EIC 2122
 * Author: Miguel Rodrigues & Nuno Castro
 */

#include <sys/stat.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "protocol.h"

typedef enum { DUMMY, DATA, START, STOP } ctrlCmd;
typedef enum { SIZE, NAME } paramCmd;

int
main (int argc, char **argv)
{
        if (argc < 3)
        {
                fprintf(stderr, "usage: %s <port> <filename>\n", argv[0]);
                return 1;
        }

        int fd_file;
        fd_file = open(argv[2], O_RDONLY);
        if (fd_file < 0) {
                fprintf(stderr, "err: open() -> code: %d\n", errno);
                return -1;
        }

        int fd;
        fd = llopen(atoi(argv[1]), TRANSMITTER);
        if (fd < 0) {
                fprintf(stderr, "err: llopen() -> aborting...\n");
                return -1;
        }

        uint8_t fragment[MAX_PACKET_SIZE];

        struct stat st;
        fstat(fd_file, &st);
        const off_t size_file = st.st_size;

        fragment[0] = START;
        fragment[1] = SIZE;
        fragment[2] = sizeof(off_t);
        memcpy(fragment + 3, &size_file, sizeof(off_t));
        if (llwrite(fd, fragment, 3 + sizeof(off_t)) < 0)
                goto llwrite_error;    

        uint16_t n;
        n = size_file / (MAX_PACKET_SIZE - 4);
        if (size_file % (MAX_PACKET_SIZE - 4))
                n++;

        ssize_t rb;
        int i;
        for (i = 0; i < n; i++) {
                rb = read(fd_file, fragment + 4, MAX_PACKET_SIZE - 4);    

                fragment[0] = DATA;
                fragment[1] = i % 255;
                fragment[2] = rb / 256;
                fragment[3] = rb % 256;

                if (llwrite(fd, fragment, rb + 4) < 0)
                        goto llwrite_error;
        }

        fragment[0] = STOP;
        fragment[1] = SIZE;
        fragment[2] = sizeof(off_t);
        memcpy(fragment + 3, &size_file, sizeof(off_t));
        if (llwrite(fd, fragment, 3 + sizeof(off_t)) < 0)
                goto llwrite_error;

        llclose(fd);
        close(fd_file);

        return 0;

llwrite_error:
        fprintf(stderr, "err: llwrite() -> aborting...\n");
        return -1;
}

