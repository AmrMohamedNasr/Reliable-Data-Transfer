#include "GoBackServer.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataFeeder.h"
using namespace std;

void GoBackServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
    uint32_t seq_no = 0;
	base_seq_no = 0;
    while(dataFeeder->hasNext()) {
        if (base_seq_no + window >= seq_no) {
            struct packet_core_data packet_data = dataFeeder->getNextDataSegment();
			struct packet packet = create_data_packet( &packet_data, seq_no);
    		if (send_packet(sendSocket, clientAddr, &packet)) {
				seq_no++;
				unacked_packet.push_back(packet);
				
				while (hasData(sendSocket)) {
					receive_ack(sendSocket, window);
				}
				
			} else {
				cout << "Error on sending..." << endl;
				return;
			}
        } else {
            receive_ack(sendSocket, window);
        }
    }
  }
  
  bool GoBackServer::receive_ack(int sendSocket, unsigned int window) {
	struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	struct sockaddr_in clAddr;
	struct ack_packet ack_packet = receive_ack_packet(sendSocket,
			(struct sockaddr *) &clAddr, &error, &time_out, tv);
	if (time_out) {
		cout << "Time out occurred" << endl;

		return false;
	} else if (error) {
		perror("Reception Error ");
		cout << "Error occurred on receiving packet..." << endl;
		cout << "Ending program.." << endl;

		return false;
	} else if (verifyChecksumAck(&ack_packet) && ack_packet.ackno >= base_seq_no
			&& ack_packet.ackno < base_seq_no + window) {
		        while (base_seq_no <= ack_packet.ackno) {
		            unacked_packet.erase(unacked_packet.begin());
		            base_seq_no++;
		        }
		        
		return true;
	}

	return false;
}
