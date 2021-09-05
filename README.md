# Implementation of the Distance Vector Algorithm
The distance-vector algorithm is one of two primary algorithms used in
the Internet Protocol to find the shortest path between a given node
and every other node in the network. The other algorithm is Dijkstra's
shortest-path algorithm.

The given scenario is this: given a set of routers, the links between those
routers, and the costs of those links, we now have a graph, where the routers
are the nodes, and the links are the edges. We wish to compute, for each node,
the shortest path to every other node in the network.

The importance of this problem should be obvious. In the Internet, when sending
packets from one node to another, a packet must be routed amonst routers to
reach the ultimate destination. To make this process efficient, networks must
use a routing algorithm to not just make sure the packet arrives at the
destination, but in the most optimal manner.

An important thing to note is that the Internet is a network of networks.
Thus, the DV-algorithm may be deployed for a particular subnet, which
may be in a particular AS. Thus, the nodes in that subnet only have to
consider the other nodes in the subnet, that is, in the node's view, the
entire network is just the subnet. This is for efficiency, because
otherwise the routing algorithm would have to consider millions of nodes.
Thus, the job in one subnet may be to route a packet in the most optimal
manner from one router to an edge router, which can then route the packet
into another subnet, which carries out its own "version" of the algorithm,
and is responsible for finding the optimal path in *it's* subnet. This is
how I believe it works.

## The DV Routing Algorithm
The DV algorithm is a *decentralized* routing algorithm, meaning that no
global knowledge of the network is needed. Three properties of the algorithm
are that it is:

1. Iterative - algorithm runs until no new information (vectors) are
exchanged between neighbors, implying that no new paths or link costs have
been computed or discovered.
2. Asynchronous - the algorithm executes whenever it is called for, ie, when
there is a new vector received, or a link update--it does not operate in lockstep.
3. Distributed - no global knowledge of the network is needed. Only neighbors
exchange information with each other. Non-neighbors never communicate.

The DV algorithm relies on two fundamental intuitions. First is that of the
Bellman-Ford equation, which states that the least-cost path from node x to
node y is given by the equation:

dx(y) = min_v { c(x, v) + dv(y) }, where v is all of x's neighbors

Intuitively, this makes sense. The equation states that the cost from x to y
is simply the cost from x to v, a neighbor, and then the *least-cost* path
from v to y. If we compute this over *all* of x's neighbors, then we get
multiple least-costs, and we take the minimum. The important thing to note
here is that a node *only* needs to know the cost to each of its direct
neighbors (which it can easily obtain), and we assume that the rest of the
path relies on the neighbor v--v must somehow know the least-cost path from
itself to the destination y. I will come back to this because this is an
important point.



Adapted from Kurose