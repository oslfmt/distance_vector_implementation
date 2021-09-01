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
} dt3;

int connectcosts3[4] = { 7, 999, 2, 0 };
struct vector_table vt3;

/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  // initialize vector table
  for (unsigned int i = 0; i < 4; i++) {
    for (unsigned int j = 0; j < 4; j++) {
      vt3.costs[i][j] = 999;
    }
  }
  copy_vector(vt3.costs[3], connectcosts3, 4);

  vt3.next_hops[0] = 0;
  vt3.next_hops[1] = 999;
  vt3.next_hops[2] = 2;
  vt3.next_hops[3] = 999;

  // send to direct neighbors
  struct rtpkt pkt;
  pkt.sourceid = 3;
  for (unsigned int i = 0; i < 4; i++) {
    pkt.mincost[i] = vt3.costs[3][i];
  }
  
  for (unsigned int i = 0; i < 3; i++) {
    pkt.destid = i;
    tolayer2(pkt);
  }

  printf("NODE3 VECTOR TABLE:\n");
  printvt(&vt3);
  printf("-------------------\n");
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  // copy vector to appropriate entry based on neighbor rcvd from
  int neighbor = rcvdpkt->sourceid;
  for (unsigned int i = 0; i < 4; i++) {
    vt3.costs[neighbor][i] = rcvdpkt->mincost[i];
  }

  // copy node dv to a temporary vector
  int node3_dv_copy[4];
  for (unsigned int dest = 0; dest < 4; dest++) {
    node3_dv_copy[dest] = vt3.costs[3][dest];
  }

  for (unsigned int dest = 0; dest < 4; dest++) {
    int min_to_dest = node3_dv_copy[dest];
    for (unsigned int neighbor = 0; neighbor < 4; neighbor++) {
      int new_cost = node3_dv_copy[neighbor] + vt3.costs[neighbor][dest];
      if (new_cost < min_to_dest) {
        node3_dv_copy[dest] = new_cost;
        vt3.next_hops[dest] = vt3.next_hops[neighbor];;
      }
    }
  }

  int equal = compare_vectors(node3_dv_copy, vt3.costs[3], 4);
  if (equal == 0) {
    // update vector table
    copy_vector(vt3.costs[3], node3_dv_copy, 4);

    // send updated dv to all neighbors
    struct rtpkt pkt;
    pkt.sourceid = 3;
    copy_vector(pkt.mincost, vt3.costs[3], 4);

    // send to each neighbor
    for (unsigned int i = 0; i < 3; i++) {
      pkt.destid = i;
      tolayer2(pkt);
    }
  }

  printf("NODE3 VECTOR TABLE (AFTER UPDATE):\n");
  printvt(&vt3);
  printf("---------------------\n");
}


printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}
