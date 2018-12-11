#include "GoBackServer.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "../../utils/socketUtils.h"
#include "../../web_models/packet_utils.h"
#include "../../web_models/ack_packet.h"
#include "../../data_managers/DataFeeder.h"
#include <sys/time.h>
#include <random>

using namespace std;



void GoBackServer::send_message(DataFeeder *dataFeeder, float loss_prob,
				int sendSocket, const struct sockaddr * clientAddr, unsigned int window) {
    uint32_t seq_no = 0;
    base_seq_no = 0;
    
    while (dataFeeder->hasNext()) {
		if (base_seq_no + window > seq_no) {
			struct packet_core_data packet_data =
					dataFeeder->getNextDataSegment();
			struct packet packet = create_data_packet(&packet_data, seq_no);
			bool sent;
			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			if (r < loss_prob) {
				sent = true;
				cout << "Packet " << seq_no << " dropped" << endl;
			} else {
				sent = send_packet(sendSocket, clientAddr, &packet);
			}
			if (sent) {
				if (unacked_packet.size() == 0) {
					struct timeval sendTime2;
					gettimeofday(&sendTime2, NULL);
					sendTime.tv_sec = sendTime2.tv_sec;
					sendTime.tv_usec = sendTime2.tv_usec;
				}
				seq_no++;
				unacked_packet.push_back(packet);

				while (hasData(sendSocket)) {
					receive_ack(sendSocket, window);
				}
				updateTimer(sendSocket, clientAddr, loss_prob);
			} else {
				cout << "Error on sending..." << endl;
				return;
			}
		} else {
			bool no_err = true;
			while (no_err && hasData(sendSocket)) {
				no_err = receive_ack(sendSocket, window);
			}
			if (!updateTimer(sendSocket, clientAddr, loss_prob)) {
				return;
			}
		}
	}
	while (unacked_packet.size() != 0) {
		bool no_err = true;
		while (no_err && hasData(sendSocket)) {
			no_err = receive_ack(sendSocket, window);
		}
		if (!updateTimer(sendSocket, clientAddr, loss_prob)) {
			return;
		}
	}
  }
  
  bool GoBackServer::receive_ack(int sendSocket, unsigned int window) {
	  struct timeval tv;
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;
	struct sockaddr_in clAddr;
	bool mini_timeout = update_remaining_timeout_nc(&tv, &sendTime);
	struct ack_packet ack_packet;
	if (!mini_timeout || hasData(sendSocket)) {
		mini_timeout = false;
		ack_packet = receive_ack_packet(sendSocket,
				(struct sockaddr *) &clAddr, &error, &time_out, tv);
	}
	if (time_out || mini_timeout) {
		cout << "Time out occurred " << base_seq_no << endl;
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
				if (unacked_packet.size() != 0) {
					struct timeval sendTime2;
					gettimeofday(&sendTime2, NULL);
					sendTime.tv_sec = sendTime2.tv_sec;
					sendTime.tv_usec = sendTime2.tv_usec;
				}
				return true;
	}
	return false;
}


bool GoBackServer::updateTimer (int sendSocket, const struct sockaddr * clientAddr, float loss_prob) {
	
		struct timeval tv;
		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		bool mini_timeout = update_remaining_timeout_nc(&tv, &sendTime);
		if (mini_timeout) {
		    
			cout << "Timeout " << base_seq_no << endl;
			unsigned int i = 0;
			while (i < unacked_packet.size()) {
			    struct packet packet = unacked_packet[i];
			    cout << "Retransmitting packet " << packet.seqno << endl;

    			bool sent;
    			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    			if (r < loss_prob) {
    				sent = true;
    				cout << "Packet " << packet.seqno << " dropped" << endl;
    			} else {
    			    
    				sent = send_packet(sendSocket, clientAddr, &packet);
    			}
    			if (sent) {
    				if (i == 0) {
    					struct timeval sendTime2;
    					gettimeofday(&sendTime2, NULL);
    					sendTime.tv_sec = sendTime2.tv_sec;
    					sendTime.tv_usec = sendTime2.tv_usec;
    				}
    			} else {
    				cout << "Error on sending packets..." << endl;
    				return false;
    			}
    			i++;
			}
		} else {
			return true;
		}
	
	return true;
}


