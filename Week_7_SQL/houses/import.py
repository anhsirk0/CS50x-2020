# import csv into sqlite database
import sys, csv, sqlite3

if len(sys.argv) != 2:
    sys.exit("not enough arguements")
# csv file from terminal arguement
csvfile = sys.argv[1]

# create sqlite connection to database file
conn = sqlite3.connect('students.db')
c = conn.cursor()

#function to insert individual student at student id
def insert_student(student,student_id):
    full_name = student[0].split(" ")
    first = full_name[0]
    middle = full_name[1] if len(full_name)==3 else None
    last = full_name[-1]
    house = student[1]
    birth = int(student[2])

    with conn:
        if middle:  #if middle is not None
            c.execute("""INSERT INTO students
                     VALUES ({},'{}','{}','{}','{}',{})""".format(
                     student_id, first, middle, last, house, birth))
        else:  #if middle name is None insert NULL
            c.execute("""INSERT INTO students
                     VALUES ({},'{}',{},'{}','{}',{})""".format(
                     student_id, first, "NULL", last, house, birth))

# read csv file
with open(csvfile, 'r') as file:
    reader = csv.reader(file)
    next(reader) # first line (headers)
    i = 1 # initial id
    for row in reader:
        student = row # read students info from csvfile
        insert_student(student,i)
        i = i + 1 # increment id for every student
# print(i)

# close connection
conn.close()

