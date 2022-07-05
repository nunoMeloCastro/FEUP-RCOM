/*
 * protocol.h
 * Serial port protocol
 * RC @ L.EIC 2122
 * Authors: Miguel Rodrigues & Nuno Castro
 */

#ifndef _PROTOCOL_H_

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define RECEIVER 0x01
#define TRANSMITTER 0x03

#define MAX_PACKET_SIZE 250

/***
 * Sets up the terminal, in order to send information packets
 * @param int[in] - port x corresponding to the file /dev/ttySx
 * @param const uint8_t[in] - determines whether is the RECEIVER or TRANSMITTER called
 * @param int[out] - file descriptor corresponding to the opened file 
 */
int
llopen(int port, const uint8_t endpt);

/***
 * Writes a given chunck of information in the file pointed by the first param
 * @param int[in] - file descriptor pointing to the file where information will be written
 * @param uint8_t *[in] - information to be written
 * @param ssize_t[in] - size in bytes of the chunck of information 
 * @param ssize_t[out] - number of bytes written
 */
ssize_t
llwrite(int fd, uint8_t *buffer, ssize_t len);

/***
 * Reads a given chunck of information in the file pointed by the first param
 * @param int[in] - file descriptor pointing to the file where information will be read
 * @param uint8_t *[in] - place where to place the information after performing the reading
 * @param ssize_t[out] - number of bytes read
 */
ssize_t
llread(int fd, uint8_t *buffer);

/***
 * Reverts to the previous terminal settings and shutdowns all the resources in use
 * @param int[in] - file descriptor corresponding to the opened file 
 * @param int[out] - 0 if no errors occur, negative value otherwise
 */
int 
llclose(int fd);

#endif /* _PROTOCOL_H_ */

