# <div id="top"></div>

# Thread Package on Edsim51

![](https://badgen.net/github/last-commit/bob1113/thread-pkgs-on-edsim51)
![](https://badgen.net/github/license/bob1113/thread-pkgs-on-edsim51)

## Table of contents

  - [About the project](#about-the-project)
  - [How to run this project](#how-to-run-this-project)
    - [Prerequisites](#prerequisites)
    - [Installation and simulation](#installation-and-simulation)
  - [Reference](#reference)
  - [Contact](#contact)


## About the project

This is an implementation of a multi-threading package in C language which targets at Edsim51 simulator. The whole project consists of five checkpoints, each with a different testcase

+ checkpoint 1: cooperative thread-switching 
+ checkpoint 2: preemptive thread-switching 
+ checkpoint 3: semaphore + bounded-buffer testcase
+ checkpoint 4: two producers & one consumer testcase
+ checkpoint 5: delay function + parking lot testcase

<p align="right">(<a href="#top">back to top</a>)</p>


## How to run this project

### Prerequisites

One needs Edsim51 to simulate 8051 processor and SDCC to generate Edsim51-targeted files

- [SDCC](http://sdcc.sourceforge.net/)
	- Small device C compiler
	- Open source, free, cross-platform
	- Default ISA is Intel mds51

- [Edsim51](http://www.edsim51.com/)
	- An 8051 microcontroller simulator
	- Runs as a Java app 
	- DI version - simulates a complete embedded system
	- SH version - customizable 

### Installation and simulation

To get a local copy up and running follow these simple steps.

1. clone the repo

   ```sh
   git clone https://github.com/bob1113/thread-pkgs-on-edsim51.git
   ```

2. Change into checkpoint directory and compile via run

    ```sh
    make
    ```

3. Choose the generated `.hex` file and click `Load` to load it into Edsim51

4. Click `Assm` to convert `.hex` file into assembly file

5. Click `Run` to execute

6. After finishing the simulation, one may clean up the grnerated files via run

	```sh
	make clean
	```


<p align="right">(<a href="#top">back to top</a>)</p>


## Reference

+ [MCS® 51 Microcontroller Family User's Manual](http://web.mit.edu/6.115/www/document/8051.pdf)

<p align="right">(<a href="#top">back to top</a>)</p>



## Contact

Bob Lai - [bobolai1113@gmail.com](bobolai1113@gmail.com) \
Project Link - [https://github.com/bob1113/pyMoney](https://github.com/bob1113/thread-pkgs-on-edsim51)

<p align="right">(<a href="#top">back to top</a>)</p>