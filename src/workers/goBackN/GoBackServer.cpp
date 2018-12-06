#include "GoBackServer.h"

using namespace std;

void send_message(DataFeeder dataFeeder, float loss_prob,
int sendSocket, const struct sockaddr * clientAddr) {
window = 2;

  while (dataFeeder.hasNext()) {
    while (dataFeeder.hasNext() && unacked_data_counter < window) {
      struct packet_core_data packet = dataFeeder.getNextDataSegment();

      struct packet* data_packet = create_data_packet(
      &packet, seq_num);
	seq_num++;
      if(send_packet(sendSocket,&clientAddr,&data_packet)) { 
	unacked_packet.push_back(data_packet);
      }
      else { //error
      }
      if (unacked_data_counter == 0 ) {
		//set timer
      }
      unacked_data_counter++;
    }
    while (unacked_data_counter > 0) {
	// to be changed as Amr said
      if (recvfrom(sendSocket, acknoledgement, sizeof(*acknoledgement), 	0,&clientAddr, sizeof(*clientAddr) == -1) {
	//error	detect timout and resend;
	int j = 0;
	unacked_data_counter = 0;
	while (j < unacked_packet.size()) {
	  if(send_packet(sendSocket,&clientAddr,&unacked_packet(i))) { 
		if (unacked_data_counter == 0 ) 
			// set timer
	  }
	  else { //error
          }
          //set timer
	}	
      }
	
      else {
	if (acknoledgement -> cksum == 
	calculateChecksumAck(&acknoledgement)) {
	   unacked_data_counter -= (acknoledgement -> ackno - last_ack_num);
	// while loop to remove ack_num from the vector 
		int i = 0; 
		while (i < acknoledgement -> ackno) {
			unacked_packet.pop_front();
			i++;
		}
		last_ack_num = acknoledgement -> ackno;	
	  // if ( unacked_data_counter == 0 ) stop timer else start timer;	
	}
      }
}
}
