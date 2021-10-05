#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Return an error if user has not entered
    // exactly 1 command line argument
    if (argc != 2)
    {
        printf("Hey buddy. Please enter ONE key. No more no less.\n");
        return 1;
    }

    // Check if argv[1] is a valid key
    string err_msg = "Your key must be exactly and only 26 unique letters. :(";

    // Return an error if key is not proper length
    if (strlen(argv[1]) != 26)
    {
        printf("%s\n", err_msg);
        return 1;
    }

    // Check that argv[1] has proper syntax
    char c; // Check one char each loop through argv[1]
    int count; // Accumulator for occurances of a char in argv[1]
    char key[27]; // Place to store a validated key

    // Check that argv[1] is letters only
    for (int i = 0; i < 26; i++)
    {
        // Capitalize each char for ease-of-comparison
        c = toupper(argv[1][i]);

        // Return an error if any chars aren't letters
        if (c < 'A' || c > 'Z')
        {
            printf("%s\n", err_msg);
            return 1;
        }

        // Count how many times c occurs in argv[1]
        count = 0; // Reset count to 0 at the start of each loop
        for (int j = i; j < 26; j++)
        {
            char d = toupper(argv[1][j]);
            if (d == c)
            {
                count++;
            }

            // Return an error if c appears more than
            // once in argv[1]
            if (count > 1)
            {
                printf("%s\n", err_msg);
                return 1;
            }
        }

        // As long as c is valid, continue building
        // an all-uppercase key
        key[i] = c;
    }

    // Tell C where the end of the key string is located
    key[26] = 0;

    // Get plain text from user
    string ptxt = get_string("plaintext: ");

    // Encrypt ptxt according to key
    char ctxt[strlen(ptxt) + 1];
    for (int i = 0, n = strlen(ptxt); i < n; i++)
    {
        // Keep non-alphabetical chars unchanged
        if (ptxt[i] < 'A' || (ptxt[i] > 'Z' && ptxt[i] < 'a') || ptxt[i] > 'z')
        {
            ctxt[i] = ptxt[i];
        }
        // Substitute alphabetic chars in ptext for their ctxt equivalent
        else
        {
            // Preserve case
            if (islower(ptxt[i]))
            {
                ctxt[i] = tolower(key[ptxt[i] - 97]);
            }
            else
            {
                ctxt[i] = key[ptxt[i] - 65];
            }
        }
    }

    // Tell C where the end of the ctxt string is located
    ctxt[strlen(ptxt)] = 0;

    printf("ciphertext: %s\n", ctxt);
    return 0;
}
