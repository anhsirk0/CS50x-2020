# import csv into sqlite database
import sys, sqlite3

# name of house from terminal arguement
if len(sys.argv) != 2:
    sys.exit(" not enough arguements")
house = sys.argv[1]

#sqlite connection
conn = sqlite3.connect('students.db')
c = conn.cursor()

# function to print students info in desired format
def print_info(student):
    first = student[0]
    middle = student[1]
    last = student[2]
    birth = student[3]

    if middle: # if not None
        print("{} {} {}, born {}".format(first, middle, last, birth))
    else:  # if None
        print("{} {}, born {}".format(first, last, birth))

# query to select house
cursor = conn.execute("""SELECT first,middle,last, birth FROM students
                     WHERE house = '{}' ORDER BY last,first""".format(house))

# every row after query
for row in cursor:
    print_info(row)

# close connection
conn.close()

