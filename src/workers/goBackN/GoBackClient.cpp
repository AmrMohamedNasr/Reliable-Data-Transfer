#include "GoBackClient.h"
#include "../web_models/packet.h"
#include "../web_models/packet_utils.h"
#include "../utils/socketUtils.h"
#include <sys/socket.h>
using namespace std;
void recv_message(int socketFd, DataSink sink) {

     memset (&corePacket, 0, sizeof(packet_core_data));
     memset (&recvPacket, 0, sizeof(packet));
     memset (&src_addr, 0, sizeof(sockaddr_in));
     recvPacket = receive_packet(socketFd, &src_addr, &error,
	&timeout);
 
     if (error || timeout) {
        
     }
    else {
	corePacket = extract_pure_data(&recvPacket);
        sink.feed_next_data(&corePacket);
	struct ack_packet acknoledgement;
	memset (acknoledgement, 0, sizeof(ack_packet));
	acknoledgment = create_ack_packet(recvPacket -> seqno);
	uint16_t check_sum = calculateChecksumAck(&acknoledgement);
	acknoledgement -> cksum = check_sum;
	sendto(socketFd, acknoledgement, sizeof(*acknoledgement),0,
	src_addr, sizeof(*src_addr))
    }   
}
