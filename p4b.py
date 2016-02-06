import commands
import os
import sys
min_size = 10


def merge(file1, file2, main_file):
    f = open(main_file, 'w')
    f.close()
    f = open(main_file, 'a')
    f1 = open(file1, 'r')
    f2 = open(file2, 'r')
    arr1 = [line for line in f1.read().split('\n')]
    arr2 = [line for line in f2.read().split('\n')]
    print(arr1,arr2)
    i1 = len(arr1)
    i2 = len(arr2)
    l1 = 0
    l2 = 0
    while l1 < i1 and l2 < i2:
        if int(arr1[l1]) >= int(arr2[l2]):
            f.write(arr2[l2])
            f.write("\n")
            l2 += 1
        elif int(arr1[l1]) <= int(arr2[l2]):
            f.write(arr1[l1])
            f.write("\n")
            l1 += 1
    while l1 < i1:
        f.write(arr1[l1])
        f.write("\n")
        l1 += 1
    while l2 < i2:
        f.write(arr2[l2])
        f.write("\n")
        l2 += 1
    f.close()


def divide(filename):
    status, size = commands.getstatusoutput("ls -l %s | awk '{ print $5 }'" % filename)
    status, lines = commands.getstatusoutput("wc -l %s | awk '{print $1}'" % filename)
    if int(size) < min_size:
        status, output = commands.getstatusoutput("sort -n %s" % filename)
        f = open(filename, 'w')
        f.close()
        f = open(filename, 'a')
        output = output + "\n"
        f.write(output)
        f.close()
    else:
        line = int(lines)
        if line % 2 == 1:
            line += 1
        line /= 2
        status, output = commands.getstatusoutput("split --lines=%d --numeric-suffixes --suffix-length=1 %s %s" % (line, filename, filename))
        temp_file1 = str(filename + "0")
        temp_file2 = str(filename + "1")
        divide(temp_file1)
        divide(temp_file2)
        merge(temp_file1, temp_file2, filename)

divide(sys.argv[1])
