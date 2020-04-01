#include <stdio.h>

#include <cog.h>
#include <cognetworkmanager.h>

int main(int argc, char **argv)
{
  CogInit();
  NetworkManagerInit();
  
  Socket in, out;
  AddressIPv4 serverAddress;
  
  // open sockets
  in = NetworkManagerOpenSocket((AddressIPv4){.a = 127, .b = 0, .c = 0, .d = 1, .port = 9001}, False);
  out = NetworkManagerOpenSocket((AddressIPv4){.a = 127, .b = 0, .c = 0, .d = 1, .port = 9002}, False);
  
  CogJsonObject request = NULL, response = NULL;
  request = CogJsonObjectAlloc();
  CogJsonObjectSetString(request, "request", "Gife moneh! Much good. So yes.");
  
  // send message
  while(1) {
    usleep(1000 * 250);
    NetworkManagerSendJson(out, (AddressIPv4){.a = 127, .b = 0, .c = 0, .d = 1, .port = 9001}, request);
    
    // recieve messages
    while(response = NetworkManagerPollMessages(in, &serverAddress), response != NULL) {
      CogJsonObjectInspect(response);
      AddressIPv4Inspect(&serverAddress);
      CogJsonObjectFree(response);
    }
  }
  
  CogJsonObjectFree(request);
  NetworkManagerCleanUp();
  CogCleanUp();
  return 0;
}
