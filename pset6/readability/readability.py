'''
A program to determine the grade level of text provided by the user
using the Coleman-Liau formula
'''

from cs50 import get_string
import re


# Get text from user
t = get_string('Text: ')

# Variables to track the number of letters, words and sentences
letters = words = sentences = 0

for c in range(len(t)):
    if t[c] == '.' or t[c] == '!' or t[c] == '?':
        if c == len(t) - 1 or c == len(t) - 2:
            words += 1
            sentences += 1
        else:
            if t[c + 1] != '\"':
                sentences += 1
    elif t[c] == ' ':
        words += 1
    elif re.match('[A-Z | a-z]', t[c]):
        letters += 1


#  Calculate and print the grade level of t
index = (0.0588 * (letters * 100 / words)) - (0.296 * (sentences * 100 / words)) - 15.8
if index > 15:
    print('Grade 16+')
elif index < 1:
    print('Before Grade 1')
else:
    print(f'Grade {round(index)}')
