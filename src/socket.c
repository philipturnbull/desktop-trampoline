#include "socket.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initializeNetwork(void) {
#ifdef WINDOWS
  // Initialize Winsock
  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (result != NO_ERROR) {
    fprintf(stderr, "ERROR: WSAStartup failed: %d\n", result);
    return 1;
  }
#endif

  return 0;
}

void terminateNetwork(void) {
#ifdef WINDOWS
    WSACleanup();
#endif
}

SOCKET openSocket(void) {
  return socket(AF_INET, SOCK_STREAM, 0);
}

void closeSocket(SOCKET socket) {
#ifdef WINDOWS
  closesocket(socket);
#else
  close(socket);
#endif
}

int connectSocket(SOCKET socket, unsigned short port) {
  struct sockaddr_in remote = {0};
  remote.sin_addr.s_addr = inet_addr("127.0.0.1");
  remote.sin_family = AF_INET;
  remote.sin_port = htons(port);

  return connect(socket, (struct sockaddr *)&remote, sizeof(struct sockaddr_in));
}

int writeSocket(SOCKET socket, const void *buffer, size_t length) {
  return (send(socket, buffer, length, 0) < (ssize_t)length ? -1 : 0);
}

int readSocket(SOCKET socket, void *buffer, size_t length) {
  return recv(socket, buffer, length, 0);
}

void printSocketError(char *fmt, ...) 
{    
  char formatted_string[4096];

  va_list argptr;
  va_start(argptr, fmt);
  vsprintf(formatted_string, fmt, argptr);
  va_end(argptr);

#ifdef WINDOWS
  fprintf(stderr, "%s: %ld\n", formatted_string, WSAGetLastError());
#else
  fprintf(stderr, "%s (%d): %s\n", formatted_string, errno, strerror(errno));
#endif
}
