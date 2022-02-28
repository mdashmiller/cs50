#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
  int winner;
  int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
int calc_stren_of_vic(int pair);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
  // Check for invalid usage
  if (argc < 2)
  {
    printf("Usage: tideman [candidate ...]\n");
    return 1;
  }

  // Populate array of candidates
  candidate_count = argc - 1;
  if (candidate_count > MAX)
  {
    printf("Maximum number of candidates is %i\n", MAX);
    return 2;
  }
  for (int i = 0; i < candidate_count; i++)
  {
    candidates[i] = argv[i + 1];
  }

  // Clear graph of locked in pairs
  for (int i = 0; i < candidate_count; i++)
  {
    for (int j = 0; j < candidate_count; j++)
    {
      locked[i][j] = false;
    }
  }

  pair_count = 0;
  int voter_count = get_int("Number of voters: ");

  // Query for votes
  for (int i = 0; i < voter_count; i++)
  {
    // ranks[i] is voter's ith preference
    int ranks[candidate_count];

    // Query for each rank
    for (int j = 0; j < candidate_count; j++)
    {
      string name = get_string("Rank %i: ", j + 1);

      if (!vote(j, name, ranks))
      {
        printf("Invalid vote.\n");
        return 3;
      }
    }

    record_preferences(ranks);

    printf("\n");
  }

  add_pairs();
  sort_pairs();
  lock_pairs();
  print_winner();
  return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
  for (int i = 0; i < candidate_count; i++)
  {
    if (strcasecmp(name, candidates[i]) == 0)
    {
      // Add candidate to ranks
      ranks[rank] = i;
      return true;
    }
  }
  // User has not entered a valid candidate name
  return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
  for (int i = 0; i < candidate_count; i++)
  {
    for (int j = i + 1; j < candidate_count; j++)
    {
      preferences[ranks[i]][ranks[j]] += 1;
    }
  }
  return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
  pair_count = 0;

  for (int i = 0; i < candidate_count; i++)
  {
    for (int j = 0; j < candidate_count; j++)
    {
      if (preferences[i][j] > 0)
      {
        if (preferences[i][j] > preferences[j][i])
        {
          pairs[pair_count].winner = i;
          pairs[pair_count].loser = j;
          pair_count++;
        }
      }
    }
  }
  return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
  // Temporary storage for a pair to facilitate swapping
  int temp_storage_winner;
  int temp_storage_loser;
  // Track the current largest strength of victory
  int current_largest_sov;

  for (int i = 0; i < pair_count - 1; i++)
  {
    // Set current_largest_sov to the sov of the pair at the place in the list we are
    // currently working on
    current_largest_sov = calc_stren_of_vic(i);

    for (int j = i + 1; j < pair_count; j++)
    {
      if (calc_stren_of_vic(j) > current_largest_sov)
      {
        // Temporarily store the pair with the lower sov so its spot can be
        // overwritten by the pair with the larger sov
        temp_storage_winner = pairs[i].winner;
        temp_storage_loser = pairs[i].loser;
        // Overwrite pairs into their new relatively sorted positions
        pairs[i] = pairs[j];
        pairs[j].winner = temp_storage_winner;
        pairs[j].loser = temp_storage_loser;

        // Update value of current_largest_sov after the swap has occured
        current_largest_sov = calc_stren_of_vic(i);
      }
    }
  }

  return;
}

// Calculate the difference between the number of voters who preferred one
// candidate in a pair over the other
int calc_stren_of_vic(int pair)
{
  return preferences[pairs[pair].winner][pairs[pair].loser] - preferences[pairs[pair].loser][pairs[pair].winner];
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
  // Track which candidates have edges pointing at them
  bool undefeated[candidate_count];
  int counter;

  for (int i = 0; i < candidate_count; i++)
  {
    // Initialize all candidates as undefeated
    undefeated[i] = true;
  }

  for (int i = 0; i < pair_count; i++)
  {
    // Set a "test-edge" against the loser of the pair
    undefeated[pairs[i].loser] = false;
    counter = 0;

    for (int j = 0; j < candidate_count; j++)
    {
      if (undefeated[j])
      {
        // With this edge in place if there is still at
        // least 1 undefeated candidate then the edge can be locked
        locked[pairs[i].winner][pairs[i].loser] = true;
        break;
      }
      else
      {
        counter++;
      }
    }

    if (counter == candidate_count)
    {
      // Remove the "test-edge" if it can't be locked
      undefeated[pairs[i].loser] = true;
    }
  }

  return;
}

// Print the winner of the election
void print_winner(void)
{
  for (int i = 0; i < candidate_count; i++)
  {
    for (int j = 0; j < candidate_count; j++)
    {
      // Any candidate with another locked over them
      // can't be the source
      if (locked[i][j])
      {
        candidates[j] = "loser";
      }
    }
  }

  for (int i = 0; i < candidate_count; i++)
  {
    // Print the name of the only candidate who
    // isn't a total loser
    if (strcasecmp("loser", candidates[i]) != 0)
    {
      printf("%s\n", candidates[i]);
      break;
    }
  }

  return;
}
