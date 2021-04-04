# match dna sequence

import sys
import csv

# required files from terminal arguements
csvfile = sys.argv[1]
dnafile = sys.argv[2]

# compare single test string to STR


def compare(test, STR):
    for i in range(len(STR)):
        if test[i] != STR[i]:
            return False
    return True

# check whole dna sequence for STR counts


def check(STR, dna_seq):
    i = 0
    l = len(STR)
    c_max = 0  # maximum STR counts
    c = 0  # initial STR count
    while i < len(dna_seq) - l:
        l = len(STR)
        test = dna_seq[i:i + l]
        result = compare(test, STR)
        if result:  # if current string match STR
            c += 1  # increase STR count
            i += l  # move to next piece of string
            if c > c_max:
                c_max = c  # update maximum STR count

        else:  # if current string don't match
            c = 0  # re - initialize STR count
            i += 1  # move to next
    return(c_max)


# reading dna sequence in memory
dna_seq = open(dnafile).read()

# read csv database file
with open(csvfile, 'r') as file:
    reader = csv.reader(file)
    names = next(reader)[1:]  # names of the STR to check
    result = []
    match = "No Match"
    for STR in names:  # check every STR and store STR count in result list
        result.append(check(STR, dna_seq))
    for row in reader:  # check every row for match
        case = [int(i) for i in row[1:]]  # STR counts in int
        if case == result:
            match = row[0]  # name of the match

print(match)
