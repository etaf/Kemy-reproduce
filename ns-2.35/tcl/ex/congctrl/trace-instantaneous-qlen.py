#!/usr/bin/env python
# encoding: utf-8
import os
import subprocess
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-w","--whiskers",  type="string",dest="whiskers", default="./jrats/delta1-alpha0.5-1d-gen.78")
(config, args) = parser.parse_args()
childs = []

#childs.append(subprocess.Popen("./kemy3.tcl ./kemyconf/dumbbell-buf1000-rtt150-bneck15.tcl -tcp TCP -sink TCPSink -gw DropTail -ontype bytes -onrand Exponential -avgbytes 100000 -offrand Exponential -offavg 0.5 -nsrc 8  -topo Dumbbell -tr",shell=True))
#childs.append(subprocess.Popen("./kemy3.tcl ./kemyconf/dumbbell-buf1000-rtt150-bneck15.tcl -tcp TCP -sink TCPSink -gw Blue -ontype bytes -onrand Exponential -avgbytes 100000 -offrand Exponential -offavg 0.5 -nsrc 8  -topo Dumbbell -tr",shell=True))
#childs.append(subprocess.Popen("./kemy3.tcl ./kemyconf/dumbbell-buf1000-rtt150-bneck15.tcl -tcp TCP -sink TCPSink -gw RED -ontype bytes -onrand Exponential -avgbytes 100000 -offrand Exponential -offavg 0.5 -nsrc 8  -topo Dumbbell -tr",shell=True))
#childs.append(subprocess.Popen("./kemy3.tcl ./kemyconf/dumbbell-buf1000-rtt150-bneck15.tcl -tcp TCP -sink TCPSink -gw CoDel -ontype bytes -onrand Exponential -avgbytes 100000 -offrand Exponential -offavg 0.5 -nsrc 8  -topo Dumbbell -tr",shell=True))

#os.environ['WHISKERS'] = config.whiskers
#childs.append(subprocess.Popen("./kemy3.tcl ./kemyconf/dumbbell-buf1000-rtt150-bneck15.tcl -tcp TCP -sink TCPSink -gw KEMY -ontype bytes -onrand Exponential -avgbytes 100000 -offrand Exponential -offavg 0.5 -nsrc 8  -topo Dumbbell -tr",shell=True))

childs.append(subprocess.Popen("./kemy4.tcl ./kemyconf/dumbbell-compare-buf.tcl -gw DropTail -topo Dumbbell -maxq 1000 -tr /home/lxa/jkemy-reproduce-1.0/ns-2.35/tcl/ex/congctrl/results/test/tr/DropTail.buffersize1000.topoDumbbell",shell=True))
childs.append(subprocess.Popen("./kemy4.tcl ./kemyconf/dumbbell-compare-buf.tcl -gw Blue -topo Dumbbell -maxq 1000 -tr /home/lxa/jkemy-reproduce-1.0/ns-2.35/tcl/ex/congctrl/results/test/tr/Blue.buffersize1000.topoDumbbell",shell=True))
childs.append(subprocess.Popen("./kemy4.tcl ./kemyconf/dumbbell-compare-buf.tcl -gw RED -topo Dumbbell -maxq 1000 -tr /home/lxa/jkemy-reproduce-1.0/ns-2.35/tcl/ex/congctrl/results/test/tr/RED.buffersize1000.topoDumbbell",shell=True))
childs.append(subprocess.Popen("./kemy4.tcl ./kemyconf/dumbbell-compare-buf.tcl -gw CoDel -topo Dumbbell -maxq 1000 -tr /home/lxa/jkemy-reproduce-1.0/ns-2.35/tcl/ex/congctrl/results/test/tr/CoDel.buffersize1000.topoDumbbell",shell=True))

os.environ['WHISKERS'] = config.whiskers
childs.append(subprocess.Popen("./kemy4.tcl ./kemyconf/dumbbell-compare-buf.tcl -gw KEMY -topo Dumbbell -maxq 1000 -tr /home/lxa/jkemy-reproduce-1.0/ns-2.35/tcl/ex/congctrl/results/test/tr/KEMY.buffersize1000.topoDumbbell",shell=True))

for child in childs:
    child.wait()

cwd = os.getcwd()
os.chdir("./qlens")
child = subprocess.Popen("gnuplot -p ./qlen.gnu",shell=True)
child.wait()
child = subprocess.Popen("display ./qlen.eps",shell=True)
#child.wait()
os.chdir(cwd)
