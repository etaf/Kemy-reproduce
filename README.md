# Kemy : An AQM generator based on machine learning.

--------

We developed a program called Kemy
based on machine learning technologies. Comparing to some
representative artificial designed AQM schemes, we found that
Kemy-generated scheme performs better than other, and is more
effective against bufferbloat.

![](https://github.com/jkemy/Kemy-reproduce/blob/master/doc/images/dumbbell_result.png)

> ##Reproduce the Results

We produce the result showned in the above Figure on Ubuntu 14.04 LTS. Before running the evluation program, please make sure the following tools have been correctly installed in you running environment.

**Required tools:**

* C++ 11 compiler (we use gcc 4.8)
* [Google Protocol Buffers](https://code.google.com/p/protobuf/)
* [Boost C++ libraries](http://www.boost.org/)

After the required tools have been installed, please follow the step below to reproduce the result.
Download the source codes:

**Steps:**

1. ```git clone https://github.com/jkemy/Kemy-reproduce.git```
2. ```tar -xf kemy-reprodece```
3. ```cd kemy-reproduce```
4. ```./install```
5. ```cd ns-2.35/tcl/ex/congctrl```
6. ```./main.py```

if successful, you will see the following uncolorized image:
![](https://github.com/jkemy/Kemy-reproduce/blob/master/doc/images/origin-result.png)
--------

> ##Run training code

See on the repository of  [Kemy-training](https://github.com/jkemy/Kemy-training)
