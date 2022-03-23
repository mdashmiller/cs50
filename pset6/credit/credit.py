'''
Use Luhn’s algorithm to determine if a number provided by the
user is a valid credit card number.
'''

from cs50 import get_string

# Get a number to check from the user
n = get_string('Number: ')

# First we multiply every other digit by 2, starting
# with the number’s second-to-last digit then add
# those products’ digits together

# For an even number of digits we can start at the
# first one to end at the second-to-last and for
# an odd number of digits we have to start
# at the second digit
start = 0 if len(n) % 2 == 0 else 1

products = ''
for i in range(start, len(n), 2):
    products += str(int(n[i]) * 2)

sum_of_digits = 0
for i in range(len(products)):
    sum_of_digits += int(products[i])

# Now add that sum to the sum of the digits that weren’t
# multiplied by 2, again starting from the end
start = 1 if len(n) % 2 == 0 else 0

sum_of_other = 0
for i in range(start, len(n), 2):
    sum_of_other += int(n[i])

check_sum = sum_of_digits + sum_of_other

# The last digit of the check sum should be
# zero for a valid credit card
if check_sum % 10 == 0:
    # Look at the first digit or two of n to determine the
    # brand of credit card
    if n[0] == '4':
        print('VISA')
    elif n[0] == '3':
        if n[1] == '4' or n[1] == '7':
            print('AMEX')
        else:
            print('INVALID')
    elif n[0] == '5':
        if n[1] == '1' or n[1] == '2' or n[1] == '3' or n[1] == '4' or n[1] == '5':
            print('MASTERCARD')
        else:
            print('INVALID')
else:
    print('INVALID')
