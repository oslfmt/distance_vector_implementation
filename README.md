# Implementation of the Distance Vector Algorithm
The distance-vector algorithm is one of two primary algorithms used
to find the shortest path between a given node and every other node 
in the network.

The given scenario is this: given a set of routers, the links between those
routers, and the costs of those links, we now have a graph, where the routers
are the nodes, and the links are the edges. We wish to compute, for each node,
the shortest path to every other node in the network.

The importance of this problem is obvious. In the Internet, when sending
packets from one node to another, a packet must be routed amongst routers to
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
and is responsible for finding the optimal path in *it's* subnet.

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
path relies on the neighbor v. v must somehow know the least-cost path from
itself to the destination y. I will come back to this because this is an
important point.

So, if each node can find this vector, then it knows the least cost to every
single other node in the network! Now, the question becomes, how can each node
obtain, or compute, this vector?

### Algorithm Breakdown
Well, each node starts out with a vector Dx(y) (note the capital D), which is
a vector of estimates to all nodes y in the network. For example, suppose we
have a network of 5 nodes, a, b, c, d, e. From a's POV, it maintains a vector:

    b   c   d   e
a   x   x   x   x

where, each of the x's are a's current known costs to b, c, d, e. If a is not
connected to a node, say node d, then the cost will be infinity. In other words,
each node starts out with a vector that contains the costs of the links to its
direct neighbors. Since we are just starting out, this is the minimum known
cost to each node.

Node a then sends it's vector to all of its neighbors, and all neighbors of a
send their vectors to a. Node a thus must maintain a table of vectors, where
each row is the distance vector of a directly connected neighbor. Thus, if
a's neighbors are b, c, and e, its table would look like:

    b   c   d   e
a   x   x   x   x
b   x   x   x   x
c   x   x   x   x
d   inf inf inf inf
e   x   x   x   x

Node d's vector would always be a vector of infinities, since node d is not
connected to a, thus the node's will never communicate.

Let's say node a receives a new vector from node b. Node b's vector contains
its currently known least-cost paths to its neighbors. Node a then updates
its own vector by computing the Bellman-Ford equation. That is, for each
destination y in N, for each neighbor v, the cost from a to v, and then the
cost from v to y. Since b's vector is now not just a row of infinities, some
updates may occur, ie, a new minimum cost is found, and a can update its own
vector. Node a then send's out this new updated vector to its neighbors.

And now we arrive at a second fundamental intuition. It has been proven, that
as nodes continue to exchange DVs in this asynchronous, distributed manner,
then eventually the estimate vectors converge to the actual least cost
paths for all nodes!

However, the node's are not simply interested in the least cost of the least
cost paths to all destinations. This doesn't help them at all--they must know
the next hop *on the least cost path* in order to forward packets to the
next node on this least cost path, thus fulfilling its role.

Let's think about how we can figure out the next hop router on the least cost
path. [explain and reason about why my method works]

Consider this scenario. Let's say we received a new vector from neighbor c,
and while performing the update computation, we find out that we have found
a new minimum cost to another destination, say, node d, ie, this new
minimum cost is better than the current one we have in our vector. Now, we
know that the distance from c to d is minimal, and we don't have to worry
about that leg of the path--that is c's responsibility. We also know we
must reach c, but what is the minimal path to c? Since c is a direct
neighbor we could of course use c as the next hop. But there could be
a shorter path to c. 

[reason about how the algorithm eventually converges]

Adapted from Kurose