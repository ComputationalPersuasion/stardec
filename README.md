# stardec

## Installation
1. Clone the repository
2. ```git submodule init```
3. ```git submodule update --remote```
4. ```cmake .```
5. ```make```

## Running the program
It can be a good idea to run ```./stardecbin -h```

The folder ```resources``` contains examples of graphs to have an idea of the syntax for the input file.
Basically it is:
* ```arg(arg).``` to define an argument ```arg``` (it can be any alphanum label)
* ```att(arg1, arg2).``` to define an attack from ```arg1``` to ```arg2``` (they need to be defined first)
* ```goal(arg)``` to define the goal for the persuader in the dialogue (the argument needs to be defined first)

## Choose you functions
Some functions (decision, evaluation, updating, etc.) are already defined. If you want to define your own, just follow the stubs that are given in the different headers included (most of them are in ```tree.h```).
You then just need to replace the function in the main.
