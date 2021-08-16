#include <stdio.h>

int main() {
  int v1[4] = {0,1,3,7};
  int v2[4] = {0,1,2,7};

  int equal = compare_vectors(v1, v2);
  if (equal) {
    printf("Equal\n");
  } else {
    printf("Not equal\n");
  }
}

int compare_vectors(const int* v1, const int* v2) {
  for (unsigned int i = 0; i < 4; i++) {
    if (*(v1 + i) != *(v2 + i)) {
      return 0;
    }
  }
  return 1;
}