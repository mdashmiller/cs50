#include <stdio.h>
#include <cs50.h>

int main(void)
{
  // Prompt user for int 1 to 8 inclusive
  int n;
  do
  {
    n = get_int("Height: ");
  } while (n < 1 || n > 8);

  // Print a Mario pyramid to screen where
  // the number of rows corresponds to n
  for (int row = 1; row <= n; row++)
  {
    // Leading spaces for each
    // bottom (nth) row has 0 spaces
    for (int l_spaces = n - row; l_spaces > 0; l_spaces--)
    {
      printf(" ");
    }
    // Bricks for the left side
    for (int col_l = 0; col_l < row; col_l++)
    {
      printf("#");
    }
    // The center gap
    printf("  ");
    // Bricks for the right side
    for (int col_r = 0; col_r < row; col_r++)
    {
      printf("#");
    }
    printf("\n");
  }
}