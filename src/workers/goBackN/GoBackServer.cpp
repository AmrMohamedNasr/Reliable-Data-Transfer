#include "GoBackServer.h"
#include <iostream>
#include <unistd.h>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataFeeder.h"
using namespace std;

void GoBackServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
 memset(&src_addr, 0 , sizeof(sockaddr_in));
  unacked_data_counter = 0;
  seqno = 0;
  last_acked = 0;
  while (dataFeeder.hasNext()) {
    while (dataFeeder.hasNext() && unacked_data_counter < window) {
      struct packet_core_data core_data = dataFeeder.getNextDataSegment();

      struct packet packet = create_data_packet(&core_data, seqno);
	seqno++;
      if(send_packet(sendSocket,(struct sockaddr *)&clientAddr,&packet)) { 
	unacked_packet.push_back(packet);
      }
      else { //error
      }
      /*if (unacked_data_counter == 0 ) {
		//set timer
      }*/
      unacked_data_counter++;
    }
    while (unacked_data_counter > 0) {
	struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
	struct ack_packet ack_packet = receive_ack_packet(sendSocket, (struct sockaddr *)&src_addr, &error, &time_out, tv);
	if (verifyChecksumAck(&ack_packet)) {
		if (!error && !timeout) {
			unsigned int j = last_acked; 
			while (j < ack_packet.ackno) {
				unacked_packet.pop_front();
				j++;
			}
			unacked_data_counter -= ack_packet.ackno;
			last_acked = ack_packet.ackno;
		}
		else if (timeout) {
			int j = 0;
			unacked_data_counter = 0;
			while (j < unacked_packet.size()) {
				if (send_packet(sendSocket,(struct sockaddr *)&clientAddr,&unacked_packet.at(j))) {
					/*if (unacked_data_counter == 0 ) {
		//set timer
      }*/
					unacked_data_counter++;	
				}
				else {//error
				}
				j++;
			}
		}
		
		
	}
	
    }
  }
}












