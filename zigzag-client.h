#ifndef ZIGZAG_H
#define ZIGZAG_H

void error(const char *msg);
int sending(int socketFD, char *bufferSend);
int recieve(int socketFD, char *bufferRecieve, char *nick);
char *welcome();

#endif
