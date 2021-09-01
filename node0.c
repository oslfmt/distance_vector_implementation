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

struct distance_table 
{
  int costs[4][4];
} dt0;

// instantiate a new vector table for node0
struct vector_table vt0;

/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
  // initialize the distance table (hardcoded values)
  vt0.costs[0][0] = 0;
  vt0.costs[0][1] = 1;
  vt0.costs[0][2] = 3;
  vt0.costs[0][3] = 7;

  // set all other distances to infinity since we don't know
  for (unsigned int i = 1; i < 4; i++) {
    for (unsigned int j = 0; j < 4; j++) {
      vt0.costs[i][j] = 999;
    }
  }

  // set next hops
  vt0.next_hops[0] = 999;
  vt0.next_hops[1] = 1;
  vt0.next_hops[2] = 2;
  vt0.next_hops[3] = 3;

  // send to direct neighbors, node's distance vector by calling tolayer2() and passing in a routing packet (rtpkt)
  struct rtpkt pkt;
  pkt.sourceid = 0;
  for (unsigned int i = 0; i < 4; i++) {
    pkt.mincost[i] = vt0.costs[0][i];
  }

  // send to each neighbor
  for (unsigned int i = 1; i < 4; i++) {
    pkt.destid = i;
    tolayer2(pkt);
  }

  printf("NODE0 VECTOR TABLE:\n");
  printvt(&vt0);
  printf("-------------------\n");
}

// called when a routing pkt is received from a neighbor
// called when a routing pkt must be sent to another node
void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  // routing packet received from neighbor
  // copy vector to appropriate entry based on neighbor rcvd from
  int neighbor = rcvdpkt->sourceid;
  for (unsigned int i = 0; i < 4; i++) {
    vt0.costs[neighbor][i] = rcvdpkt->mincost[i];
  }

  // copy node0 distance vector
  int node0_dv_copy[4];
  for (unsigned int dest = 0; dest < 4; dest++) {
    node0_dv_copy[dest] = vt0.costs[0][dest];
  }

  // equal = 1
  // update node 0's distance vector (copy) using new vector
  for (unsigned int dest = 0; dest < 4; dest++) {
    int min_to_dest = node0_dv_copy[dest];
    for (unsigned int neighbor = 0; neighbor < 4; neighbor++) {
      int new_cost = node0_dv_copy[neighbor] + vt0.costs[neighbor][dest];
      if (new_cost < min_to_dest) {
        // this only changes the copy
        // equal = 0
        node0_dv_copy[dest] = new_cost;
        // update next hop - it is the hop to get to the neighbor (which leads to shortest path to ultimate dest)
        vt0.next_hops[dest] = vt0.next_hops[neighbor];
      }
    }
  }

  // if node0 dv changed, send updated vector to all neighbors
  int equal = compare_vectors(node0_dv_copy, vt0.costs[0], 4);
  if (equal == 0) {
    // update node0 vector table
    copy_vector(vt0.costs[0], node0_dv_copy, 4);

    // send updated dv to all neighbors
    struct rtpkt pkt;
    pkt.sourceid = 0;
    copy_vector(pkt.mincost, vt0.costs[0], 4);
    
    // send to each neighbor
    for (unsigned int i = 1; i < 4; i++) {
      pkt.destid = i;
      tolayer2(pkt);
    }
  }

  printf("NODE0 VECTOR TABLE (AFTER UPDATE):\n");
  printvt(&vt0);
  printf("---------------------\n");
}

void printdt0(dtptr)
  struct distance_table *dtptr;
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}
