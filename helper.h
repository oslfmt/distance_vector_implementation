struct vector_table
{
  int costs[4][4];
};

// prints full vector table
extern void printvt();

// copy v2 to v1
extern void copy_vector();

// compares vectors
extern int compare_vectors();
