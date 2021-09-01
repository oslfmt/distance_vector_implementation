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
} dt2;

int connectcosts2[4] = { 3, 1, 0, 2 };
struct vector_table vt2;

/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
  // initialize vector table
  for (unsigned int i = 0; i < 4; i++) {
    for (unsigned int j = 0; j < 4; j++) {
      vt2.costs[i][j] = 999;
    }
  }
  copy_vector(vt2.costs[2], connectcosts2, 4);

  vt2.next_hops[0] = 0;
  vt2.next_hops[1] = 1;
  vt2.next_hops[2] = 999;
  vt2.next_hops[3] = 3;

  // send to direct neighbors
  struct rtpkt pkt;
  pkt.sourceid = 2;
  for (unsigned int i = 0; i < 4; i++) {
    pkt.mincost[i] = vt2.costs[2][i];
  }
  
  for (unsigned int i = 0; i < 4; i++) {
    if (i != 2) {
      pkt.destid = i;
      tolayer2(pkt);
    } 
  }

  printf("NODE2 VECTOR TABLE:\n");
  printvt(&vt2);
  printf("-------------------\n");
}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  // copy vector to appropriate entry based on neighbor rcvd from
  int neighbor = rcvdpkt->sourceid;
  for (unsigned int i = 0; i < 4; i++) {
    vt2.costs[neighbor][i] = rcvdpkt->mincost[i];
  }

  // copy node dv to a temporary vector
  int node2_dv_copy[4];
  for (unsigned int dest = 0; dest < 4; dest++) {
    node2_dv_copy[dest] = vt2.costs[2][dest];
  }

  for (unsigned int dest = 0; dest < 4; dest++) {
    int min_to_dest = node2_dv_copy[dest];
    for (unsigned int neighbor = 0; neighbor < 4; neighbor++) {
      int new_cost = node2_dv_copy[neighbor] + vt2.costs[neighbor][dest];
      if (new_cost < min_to_dest) {
        node2_dv_copy[dest] = new_cost;
        vt2.next_hops[dest] = vt2.next_hops[neighbor];;
      }
    }
  }

  int equal = compare_vectors(node2_dv_copy, vt2.costs[2], 4);
  if (equal == 0) {
    // update vector table
    copy_vector(vt2.costs[2], node2_dv_copy, 4);

    // send updated dv to all neighbors
    struct rtpkt pkt;
    pkt.sourceid = 2;
    copy_vector(pkt.mincost, vt2.costs[2], 4);

    // send to each neighbor
    for (unsigned int i = 0; i < 4; i++) {
      if (i != 2) {
        pkt.destid = i;
        tolayer2(pkt);
      } 
    }
  }

  printf("NODE2 VECTOR TABLE (AFTER UPDATE):\n");
  printvt(&vt2);
  printf("---------------------\n");
}


printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}






