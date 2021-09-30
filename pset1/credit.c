#include <cs50.h>
#include <stdio.h>

int main(void)
{
  // Prompt user to enter a CC number
  long n = get_long("Number: ");
  // loop_n will be overwritten in the loop below but we'll still
  // need the original value of n to use later
  long loop_n = n;

  // Use Luhn’s Algorithm to check if n
  // is a syntactically valid CC number
  long rem;
  long power = 1;         // Allows us to divide by a power of 10 higher each time through the loop
  int i = 1;              // Tracks whether we are on an odd or even numbered pass and is used to id the first digits of n
  int dbl_digit;          // On odd passes through n the current digit will be doubled
  int sum_of_dbls = 0;    // Sum of all the dbl_digit products’ digits...whew
  int sum_of_singles = 0; // Sum of all the digits from the odd numbered passes

  do
  {
    // Store the remainder
    rem = loop_n % (10 * power);
    // if this is an even-numbered pass through n
    // then we'll want to double the digit
    if (i % 2 == 0)
    {
      dbl_digit = rem / power * 2;
      // When doubling results in 2 digits,
      // split and add them before adding them
      // to sum_of_dbls
      if ((dbl_digit / 10) < 1)
      {
        sum_of_dbls += dbl_digit;
      }
      else
      {
        sum_of_dbls += dbl_digit % 10 + 1;
      }
    }
    // On an odd numbered pass through n we
    // can just add the current digit to sum_of_singles
    else
    {
      sum_of_singles += rem / power;
    }
    loop_n = loop_n - rem;
    power *= 10;
    i++;
  } while (power <= loop_n);

  // We'll use the sum of the doubled and single
  // digits to see if n is a syntactically valid CC number
  if ((sum_of_dbls + sum_of_singles) % 10 == 0)
  {
    // If n is valid, id its first and second digits
    int len_n = i - 1;
    int loop_len_n = len_n; // Like n before, we need a copy of len_n to overwrite in a loop
    // printf("len: %i\n", len_n);

    // Build up a divisor by powers of ten according to the len of n
    long divisor = 1;
    while (loop_len_n > 1)
    {
      divisor *= 10;
      loop_len_n--;
    }
    // printf("divisor: %li\n", divisor);

    // Divide n to get a float which we can truncate to get the 1st digit
    long first_digit = n / divisor;
    long second_digit = (n / (divisor / 10)) % 10;
    printf("first digit: %li\n", first_digit);
    printf("second digit: %li\n", second_digit);

    // Check for VISA
    if (first_digit == 4)
    {
      if (len_n == 13 || len_n == 16)
      {
        printf("VISA\n");
      }
      else
      {
        printf("INVALID\n");
      }
    }
    // Check for AMEX
    else if (first_digit == 3)
    {
      if (second_digit == 4 || second_digit == 7)
      {
        if (len_n == 15)
        {
          printf("AMEX\n");
        }
        else
        {
          printf("INVALID\n");
        }
      }
      else
      {
        printf("INVALID\n");
      }
    }
    // Check for MC
    else if (first_digit == 5)
    {
      if (second_digit == 1 || second_digit == 2 || second_digit == 3 || second_digit == 4 || second_digit == 5)
      {
        if (len_n == 16)
        {
          printf("MASTERCARD\n");
        }
        else
        {
          printf("INVALID\n");
        }
      }
      else
      {
        printf("INVALID\n");
      }
    }
    else
    {
      printf("INVALID\n");
    }
  }
  else
  {
    printf("INVALID\n");
  }
}