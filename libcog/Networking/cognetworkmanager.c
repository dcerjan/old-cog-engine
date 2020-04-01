#include "cognetworkmanager.h"
#include "cogplatform.h"
#include "coglogger.h"

struct {
  CogList(CogJsonObject)  messages;
  CogArray(CogSocket)     sockets;
} __cog_network_manager__;

void CogAddressIPv4Inspect(const CogAddressIPv4* self) {
  printf("<AddressIPv4 %u.%u.%u.%u:%u>\n", self->a, self->b, self->c, self->d, self->port);
}

void CogNetworkManagerInit(void) {
  #ifdef WIN32
  
  #endif
  
  __cog_network_manager__.messages = CogListAlloc(CogJsonObject);
  __cog_network_manager__.sockets = CogArrayAlloc(CogSocket);
  CogLoggerInfo(CogPlatformGetLogger(), "NetworkManager initialized\n");
}

void CogNetworkManagerCleanUp(void) {
  while(__cog_network_manager__.messages->size > 0) {
    CogJsonObjectFree(CogListPopBack(__cog_network_manager__.messages));
  }
  CogListFree(__cog_network_manager__.messages);
  CogArrayFree(__cog_network_manager__.sockets);
  CogLoggerInfo(CogPlatformGetLogger(), "NetworkManager cleaned up\n");
}

CogJsonObject NetworkManagerPollMessages(const CogSocket sock, CogAddressIPv4* senderAddress) {
  
#ifdef WIN32
  
#else
  struct sockaddr_in addr;
  size_t recievedBytes = 0;
  do {
    memset(&addr, 0, sizeof(addr));
    int senderLen = sizeof(addr);
    char buffer[1024 * 4];
    memset(buffer, 0, sizeof(char) * 1024 * 4);
    recievedBytes = recvfrom(sock.sock, buffer, 1024 * 4, 0, (struct sockaddr*)&addr, (socklen_t*)&senderLen);
    if(recievedBytes > 0) {
      CogJsonObject msg = JsonParserParseString(NULL, buffer);
      CogListPushBack(__cog_network_manager__.messages, msg);
      if(senderAddress!= NULL) {
        senderAddress->port = ntohs(addr.sin_port);
        senderAddress->address = ntohl( ((unsigned int*)&addr.sin_addr)[0] );
      }
    }
  } while(recievedBytes > 0);
  
  if(__cog_network_manager__.messages->size > 0) {
    return CogListPopFront(__cog_network_manager__.messages);
  }
#endif
  return NULL;
}

CogSocket CogNetworkManagerOpenSocket(const CogAddressIPv4 address, const Bool blocking) {
  CogSocket sock;
#ifdef WIN32
  return sock;
#else
  struct sockaddr_in addr;
  
  sock.sock = socket(AF_INET, SOCK_DGRAM, 0);
  sock.blocking = blocking;
  if(sock.sock < 0) {
    LoggerError(PlatformGetLogger(), "Unable to acquire an open socket! \n");
    exit(5);
  }
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(address.port);
  addr.sin_addr.s_addr = htonl(address.address);
  if(bind(sock.sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
    close(sock.sock);
    LoggerError(PlatformGetLogger(), "Unable to bind socket to port %u! \n", address.port);
    exit(5);
  }
  if(!blocking) {
    if(fcntl(sock.sock, F_SETFL, O_NONBLOCK, 1) < 0) {
      LoggerError(PlatformGetLogger(), "Unable to set socket to nonblocking! \n");
      close(sock.sock);
      exit(5);
    }
  }
  CogArrayPush(__cog_network_manager__.sockets, sock);
  LoggerInfo(PlatformGetLogger(), "Socket succesfully bound to port %u\n", address.port);
  return sock;
#endif
}

Bool CogNetworkManagerSendJson(const CogSocket sock, const CogAddressIPv4 address, const CogJsonObject json) {
#ifdef WIN32
  return False;
#else
  CogString msg = CogJsonObjectSerialzeToString(json);
  
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  unsigned int ip = htonl(address.address);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(address.port);
  memcpy(&addr.sin_addr, &ip, sizeof(ip));
  
  size_t sentBytes = sendto(sock.sock, (const char*)msg->data, msg->len, 0, (const struct sockaddr*)&addr, sizeof(addr));
  if(sentBytes < msg->len) {
      LoggerError(PlatformGetLogger(), "Unable to send data! errcode: %d \n", sentBytes);
      CogStringFree(msg);
      return False;
  }
  CogStringFree(msg);
  return True;
#endif
}
