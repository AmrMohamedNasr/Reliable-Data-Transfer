#include "GoBackClient.h"
#include "packet.h"
#include "socketUtils.h"

using namespace std;
void recv_message(int socketFd, DataSink sink) {
     bool error = false;
     bool timeout = false;
     struct packet * recvPacket;
     struct sockaddr * src_addr;
     struct packet_core_data * corePacket;
     memset (corePacket, 0, sizeof(packet_core_data));
     memset (recvPacket, 0, sizeof(packet));
     memset (src_addr, 0, sizeof(sockaddr));
     recvPacket = receive_packet(socketFd, &src_addr, &error,
	&timeout);
 
     if (!error && !timeout) {
        corePacket -> size = recvPacket -> len;
        corePacket -> data = recvPacket -> data;
        sink.feed_next_data(corePacket);
        

     }
     
     
}
