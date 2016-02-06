import sys
import commands
import re
import os
import time
import Gnuplot

file3=open('fun1','w')
file4=open('fun2','w')
file3.close()
file4.close()
file1=open('fun1','a')
file2=open('fun2','a')
def func():
	for i in range (1080):
		status,output=commands.getstatusoutput("perf stat -p %d sleep 1" %(int(sys.argv[1])))
		x = output.split('#')
		y = x[1].split(' ')
		val = float(y[4])
		file1.write('%d %f\n' %(i,val))

		status,output=commands.getstatusoutput("perf stat -p %d sleep 1" %(int(sys.argv[1])))
		x = output.split('\n')
		y = x[4].split(' ')
#		print y
		i1=0
		while y[i1]!="context-switches":
			i1+=1
#	print i1,type(y[i1-1])
		if (y[i1-1]=="counted>"):
			val=0
		else:
			val = int(y[i1-1])
		file2.write('%d %d\n' %(i,val))
func()
file1.close()
file2.close()
vee = Gnuplot.Gnuplot()
vee('set term png')
vee('set output "graph1.png"')
vee('set xrange [0:350]')
vee('set yrange [0:0.5]')
vee('plot "fun1" using 1:2 with lines')

vee1 = Gnuplot.Gnuplot()
vee1('set term png')
vee1('set output "graph2.png"')
vee1('set xrange [0:350]')
vee1('set yrange [0:160000]')
vee1('plot "fun2" using 1:2 with lines')
