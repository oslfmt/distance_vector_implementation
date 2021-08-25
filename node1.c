#include <stdio.h>
#include "helper.h"

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };


extern int TRACE;
extern int YES;
extern int NO;

int connectcosts1[4] = { 1,  0,  1, 999 };

struct distance_table 
{
  int costs[4][4];
} dt1;

struct vector_table vt1;

/* students to write the following two routines, and maybe some others */

void rtinit1() 
{
  // initialize all entries to 999
  for (unsigned int i = 0; i < 4; i++) {
    for (unsigned int j = 0; j < 4; j++) {
      vt1.costs[i][j] = 999;
    }
  }
  // copy node1's dv to the table
  copy_vector(vt1.costs[1], connectcosts1, 4);

  // send to direct neighbors
  struct rtpkt pkt;
  pkt.sourceid = 1;
  for (unsigned int i = 0; i < 4; i++) {
    pkt.mincost[i] = vt1.costs[1][i];
  }

  // send to each neighbor
  for (unsigned int i = 0; i < 4; i++) {
    if (i == 0 || i == 2) {
      pkt.destid = i;
      tolayer2(pkt);
    }
  }

  printf("NODE1 VECTOR TABLE:\n");
  printvt(&vt1);
  printf("-------------------\n");
}


void rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  // copy vector to appropriate entry based on neighbor rcvd from
  int neighbor = rcvdpkt->sourceid;
  for (unsigned int i = 0; i < 4; i++) {
    vt1.costs[neighbor][i] = rcvdpkt->mincost[i];
  }

  // copy node1 distance vector to a temporary vector
  int node1_dv_copy[4];
  for (unsigned int dest = 0; dest < 4; dest++) {
    node1_dv_copy[dest] = vt1.costs[1][dest];
  }

  for (unsigned int dest = 0; dest < 4; dest++) {
    int min_to_dest = node1_dv_copy[dest];
    for (unsigned int neighbor = 0; neighbor < 4; neighbor++) {
      int new_cost = node1_dv_copy[neighbor] + vt1.costs[neighbor][dest];
      if (new_cost < min_to_dest) {
        // this only changes the copy
        node1_dv_copy[dest] = new_cost;
      }
    }
  }

  // if node0 dv changed, send updated vector to all neighbors
  int equal = compare_vectors(node1_dv_copy, vt1.costs[1], 4);
  if (equal == 0) {
    // update vector table
    copy_vector(vt1.costs[1], node1_dv_copy, 4);

    // send updated dv to all neighbors
    struct rtpkt pkt;
    pkt.sourceid = 1;
    copy_vector(pkt.mincost, vt1.costs[1], 4);
    
    // send to each neighbor
    for (unsigned int i = 1; i < 4; i++) {
      pkt.destid = i;
      tolayer2(pkt);
    }

    // send to each neighbor
    for (unsigned int i = 0; i < 4; i++) {
      if (i == 0 || i == 2) {
        pkt.destid = i;
        tolayer2(pkt);
      }
    }
  }
}

printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

linkhandler1(linkid, newcost)   
int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

