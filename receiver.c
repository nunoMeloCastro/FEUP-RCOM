/*
 * receiver.c
 * Non-Canonical Input Processing
 * RC @ L.EIC 2122
 * Author: Miguel Rodrigues
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "protocol.h"

typedef enum { DUMMY, DATA, START, STOP } ctrlCmd;

int 
main(int argc, char **argv)
{
        if (argc < 3)
        {
                fprintf(stderr, "usage: %s <port> <filename>\n", argv[0]);
                return 1;
        }

        int fd_file;
        fd_file = open(argv[2], O_CREAT | O_WRONLY, 0666);
        if (fd_file < 0) {
                fprintf(stderr, "err: open() -> code: %d\n", errno);
                return -1;
        }

        int fd;
        fd = llopen(atoi(argv[1]), RECEIVER);
        if (fd < 0) {
                fprintf(stderr, "err: llopen() -> aborting...\n");
                return -1;
        }

        uint8_t pkgn = 0;
        uint8_t fragment[MAX_PACKET_SIZE];
        while (1) {
                ssize_t rb;
                rb = llread(fd, fragment);
                if (rb < 0)
                        continue;

                uint16_t len;
                switch (fragment[0]) {
                case DATA:
                        if (fragment[1] >= (pkgn % 255)) {
                                len = fragment[2] * 256 + fragment[3];
                                write(fd_file, fragment + 4, len);
                                ++pkgn;
                        }
                        break;
                case START:
                        break;
                case STOP:
                        llread(fd, fragment);   // Take the last disc frame
                        goto finish;
                default:
                        break;
                }
        }
        
finish:
        llclose(fd);
        close(fd_file);
        return 0;
}

