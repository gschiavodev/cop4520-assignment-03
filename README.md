
# Programming Assignment 3

In this project we were given two problems to solve. When running the program you need to pass a command line argument, either `1` or `2` to run problem one and two respectively.

#### Testing
The program was tested on several different machines such as Windows, Linux, and MacOS. Since there was no time requirment to test for, Isimply checked that each problem had the correct output.

### Problem 1: The Birthday Presents Party

In this problem we were tasked to have four servants (threads) of the minotaur sort through and unordered bag of presents and place each present one by one in sorted order into a concurrent linked-list. Each servant (thread) would cycle through the task of adding a present to the sorted list, taking a present from the sorted list and writing a thank you card for the guest, or checking if a particular present was in the sorted list as per minotaurs request.

For the third task, as of my testing, the presents requested were never found in the sorted list. This is likely due to the sheer speed and efficiency of the servants taking out gifts and writing thank you cards. Since one servant was (likely) never doing the same task as another servant, anytime a gift was sorted, it was then quickly removed by another. Because the minotaur had 500,000 presents to pick from to check for, the chance that the 3-4 gifts in the sorted list at any given time was the one he chose is unlikely.

### Problem 2: Atmospheric Temperature Reading Module

For this problem, we had to have eight temperature sensors (threads) on a Mars rover collect samples every minute for an hour, then the rover would generate a report. To solve this, I had the sensors wait on condition variables for when the rover was ready to collect samples and to wait while the rover was collecting the samples taken. They would do this every minute until all the samples were collected, the report was generated, and then the rover was ready to collect samples for the next `n_operational_hours`, where after it would shut down along with the sensors.

## Authors

- [@gschiavodev](https://github.com/gschiavodev)

## Run Locally

To compile and run the program locally, either execute the makefile, open the visual studio project and compile in x64 release mode, or manually compile the current state of the project with the commands below:

```bash
$ mkdir -p int
$ mkdir -p bin
$ g++ -std=c++17 -O3 -I include  -c -o int/main.o source/main.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/mars_rover.o source/mars_rover.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/present.o source/present.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/problem_01.o source/problem_01.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/problem_02.o source/problem_02.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/servant.o source/servant.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/temperature_sensor.o source/temperature_sensor.cpp
$ g++ -std=c++17 -O3 -I include  -o bin/cop4520-assignment-03 int/main.o int/mars_rover.o int/present.o int/problem_01.o int/problem_02.o int/servant.o int/temperature_sensor.o
```

Then run the program from the root directory as with the following command or edit the command line arguments to send in visual studio:

```bash
$ ./bin/cop4520-assignment-03 1
$ ./bin/cop4520-assignment-03 2
```
## Authors

- [@gschiavodev](https://github.com/gschiavodev)

## Run Locally

To compile and run the program locally, either execute the makefile, open the visual studio project and compile in x64 release mode, or manually compile the current state of the project with the commands below:

```bash
$ mkdir -p int
$ mkdir -p bin
$ g++ -std=c++17 -O3 -I include  -c -o int/MCSLock.o src/MCSLock.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/MinotaurBirthdayParty.o src/MinotaurBirthdayParty.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/MinotaurCrystalVase.o src/MinotaurCrystalVase.cpp
$ g++ -std=c++17 -O3 -I include  -c -o int/main.o src/main.cpp
$ g++ -std=c++17 -O3 -I include  -o bin/minotaur int/MCSLock.o int/MinotaurBirthdayParty.o int/MinotaurCrystalVase.o int/main.o
```
