#include "GoBackClient.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

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
		struct packet packet = receive_packet(socketFd,
				(struct sockaddr *) &src_addr, &error, &timeout, tv);
		if (error || timeout) {
			cout << "error occurred receiving packet" << endl;
		} else if (verifyChecksum(&packet)) {
			if (packet.len != PCK_HEADER_SIZE) {
				if (packet.seqno == base_ack_no + 1) {
					base_ack_no++;
					struct packet_core_data core_data = extract_pure_data(
							&packet);
					sink->feed_next_data(&core_data, packet.seqno);
					struct ack_packet ack_packet = create_ack_packet(
							packet.seqno);
					send_ack_packet(socketFd,
							(const struct sockaddr *) &src_addr, &ack_packet);
					cout << "Receving packet " << ack_packet.ackno << endl;
				}
			} else {
				struct ack_packet ack_packet = create_ack_packet(packet.seqno);
				send_ack_packet(socketFd, (const struct sockaddr *) &src_addr,
						&ack_packet);
				end = true;
			}
	//free (&src_addr);
      }
    }
     
  
}
