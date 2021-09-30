#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int calc_index(int letters, int words, int sentences);

int main(void)
{
  // Get text from user
  string text = get_string("Text: ");

  // Determine the Coleman-Liau index
  int index = calc_index(count_letters(text), count_words(text), count_sentences(text));
  // Print message for user depending on index value
  if (index < 1)
  {
    printf("Before Grade 1\n");
  }
  else if (index > 15)
  {
    printf("Grade 16+\n");
  }
  else
  {
    printf("Grade %i\n", index);
  }
}

int count_letters(string text)
{
  int count = 0; // Accumulator for number of letters
  for (int i = 0, n = strlen(text); i < n; i++)
  {
    if (toupper(text[i]) >= 'A' && toupper(text[i]) <= 'Z')
    {
      count++;
    }
  }

  return count;
}

int count_words(string text)
{
  int count = 0; // Accumulator for the number of words
  for (int i = 0, n = strlen(text); i < n; i++)
  {
    if (text[i] == ' ')
    {
      count++;
    }
  }
  // Increment count once for the last word
  count++;

  return count;
}

int count_sentences(string text)
{
  int count = 0; // Accumulator for number of sentences
  for (int i = 0, n = strlen(text); i < n; i++)
  {
    if (text[i] == '.' || text[i] == '?' || text[i] == '!')
    {
      count++;
    }
  }

  return count;
}

int calc_index(int letters, int words, int sentences)
{
  // Determine the values of L and S and use them in the Coleman-Liau equation
  return round(0.0588 * (100.0 / words * letters) - 0.296 * (100.0 / words * sentences) - 15.8);
}