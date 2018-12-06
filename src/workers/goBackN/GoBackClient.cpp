#include "GoBackClient.h"
#include <iostream>
#include <unistd.h>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataSink.h"

using namespace std;
void GoBackClient::recv_message(int socketFd, DataSink *sink, unsigned int window) {
     bool end = false;
     base_ack_no = 0;
     memset(&src_addr, 0 , sizeof(sockaddr_in));

     while (!end) {
       struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		struct packet packet = receive_packet(socketFd, (struct sockaddr *)&src_addr, &error, &time_out, tv);
		if (error || time_out) {
			cout << "error occurred receiving packet" << endl;
		}
      else if (verifyChecksum(&packet)) {
	if(packet.len != PCK_HEADER_SIZE) {
	  if( recvPacket.seqno == last_recv + 1) {
	    last_recv ++;
	    struct packet_core_data core_data = extract_pure_data(&packet);
            sink.feed_next_data(&core_data);

	    struct ack_packet ack_packet = create_ack_packet(packet.seqno);
	    send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, 	 &ack_packet);
         }
	}
	else {
	  struct ack_packet ack_packet = create_ack_packet(packet.seqno);
	  send_ack_packet(socketFd, (const struct sockaddr *)&src_addr, &ack_packet);
	  end = true;
	} 
	//free (&src_addr);
      }
    }
     
  
}
