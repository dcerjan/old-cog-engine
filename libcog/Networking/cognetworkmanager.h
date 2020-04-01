#ifndef _COG_NETWORK_MANAGER_H_
#define _COG_NETWORK_MANAGER_H_

#include "cogjson.h"
#include "cogtypes.h"
#include "cogstring.h"

#ifdef WIN32
  #include "windows.h"
#else
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <fcntl.h>
#endif

struct CogSocket {
  Bool  blocking;
  int   sock;
};
typedef struct CogSocket CogSocket;
CogDefineArray(CogSocket);

struct CogAddressIPv4 {
  union {
    struct {
      unsigned int d : 8;
      unsigned int c : 8;
      unsigned int b : 8;
      unsigned int a : 8;
    };
    unsigned int address; 
  };
  ushort port;
};
typedef struct CogAddressIPv4 CogAddressIPv4;
void        CogAddressIPv4Inspect(const CogAddressIPv4* self);

void          CogNetworkManagerInit(void);
void          CogNetworkManagerCleanUp(void);
CogJsonObject CogNetworkManagerPollMessages(const CogSocket sock, CogAddressIPv4* senderAddress);
CogSocket     CogNetworkManagerOpenSocket(const CogAddressIPv4 address, const Bool blocking);
Bool          CogNetworkManagerSendJson(const CogSocket sock, const CogAddressIPv4 address, const CogJsonObject json);

#endif