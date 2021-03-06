// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
  struct person *parents[2];
  char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
  // Seed random number generator
  srand(time(0));

  // Create a new family with three generations
  person *p = create_family(GENERATIONS);

  // Print family tree of blood types
  print_family(p, 0);

  // Free memory
  free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
  // Allocate memory for new person
  person *p = malloc(sizeof(person));
  if (p == NULL)
  {
    printf("Could not create a new person.\n");
    return NULL;
  }

  // Generation with parent data
  if (generations > 1)
  {
    // Recursively create blood type histories for parents
    person *parent_1 = create_family(generations - 1);
    person *parent_2 = create_family(generations - 1);

    p->parents[0] = parent_1;
    p->parents[1] = parent_2;

    // Randomly assign child alleles based on parents
    int r = rand() % 2;
    p->alleles[0] = p->parents[0]->alleles[r];
    r = rand() % 2;
    p->alleles[1] = p->parents[1]->alleles[r];
  }

  // Generation without parent data
  else
  {
    // Set parent pointers to NULL
    p->parents[0] = NULL;
    p->parents[1] = NULL;

    // Randomly assign alleles
    p->alleles[0] = random_allele();
    p->alleles[1] = random_allele();
  }

  // Return newly created person
  return p;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
  // Handle base case
  if (p == NULL)
  {
    return;
  }

  // Free parents
  free_family(p->parents[0]);
  free_family(p->parents[1]);

  // Free child
  free(p);
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
  // Handle base case
  if (p == NULL)
  {
    return;
  }

  // Print indentation
  for (int i = 0; i < generation * INDENT_LENGTH; i++)
  {
    printf(" ");
  }

  // Print person
  printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
  print_family(p->parents[0], generation + 1);
  print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
  int r = rand() % 3;
  if (r == 0)
  {
    return 'A';
  }
  else if (r == 1)
  {
    return 'B';
  }
  else
  {
    return 'O';
  }
}
