#include <cs50.h>
#include <stdio.h>

int main(void)
{
  // Ask user for starting population size
  // of 9 or greater
  int population;
  do
  {
    population = get_int("Start size: ");
  } while (population < 9);

  // Ask user for ending population size
  // greater than starting size
  int end;
  do
  {
    end = get_int("End size: ");
  } while (end < population);

  // calculate number of years until we reach threshold
  int years = 0;
  while (population < end)
  {
    population += population / 3 - population / 4;
    years++;
  }

  printf("Years: %i\n", years);
}