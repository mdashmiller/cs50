'''
Take a sequence of DNA and a CSV file containing STR counts for a list of individuals
and then output to whom the DNA (most likely) belongs.
'''

from sys import argv, exit
from csv import DictReader, reader


def main():
    # Check for proper usage
    if len(argv) != 3:
        print('Usage: python dna.py data.csv sequence.txt')
        exit()

    # Store the data about STR counts for individuals in a list
    db = []
    with open(argv[1]) as fcsv:
        reader = DictReader(fcsv)
        for row in reader:
            db.append(row)

    # Store the provided sequence in a string
    seq = ''
    with open(argv[2]) as ftxt:
        seq = ftxt.readlines()[0].rstrip()

    # Make a dict to store the longest consecutive repeat of each STR in seq
    lngst_rpts = {}

    # Find the longest consecutive repeat in seq of each STR listed in db
    STRs = list(db[0].keys())
    STRs.remove('name')
    for i in range(len(STRs)):
        lngst_rpts[STRs[i]] = most_cnsctv_rpts(STRs[i], seq)

    # Find the perp!
    for person in db:
        matches = 0
        for i in range(len(STRs)):
            if person[STRs[i]] == str(lngst_rpts[STRs[i]]):
                matches += 1
                if matches == len(STRs):
                    print(person['name'])
                    exit()
    print('No match')


# Return the number of repeats in the longest run of consecutive repeats
# of a substring
def most_cnsctv_rpts(pattern, seq):
    i = current = longest = 0
    while True:
        if i > len(seq) - len(pattern):
            return longest
        if seq[i] == pattern[0]:
            if seq[i:i + len(pattern)] == pattern:
                current += 1
                i += len(pattern)
                if current > longest:
                    longest = current
            else:
                current = 0
                i += 1
        else:
            current = 0
            i += 1


main()
