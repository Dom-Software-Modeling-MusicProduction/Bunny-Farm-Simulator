# Bunny-Farm-Simulator

This program serves as a console-based simulation of a bunny farm written in C++. The program models a growing bunny population over years, with randomized genetics, births, deaths, rare mutations, and special “vampire” bunnies that can spread their curse. The simulation prints out events to the console and writes a full log of everything that happens to a text file.

# Techniques That Were Incorporated

Object-oriented design using Bunny and Farm classes



- Clean use of C++ classes, encapsulation, and state
- Data structure usage: std::vector, iteration patterns, filtering into survivor lists
- Probability-based logic and simulations using rand()
- Separation of concerns (generation vs yearly updates vs logging)
- File I/O via std::ofstream
- System-style thinking: tuning difficulty, scaling effects, balancing survival vs reproduction

# How It Works 

The farm starts with 5 randomly generated bunnies. Each year:
- All bunnies age by 1
- A season event is chosen (winter/spring/predators/eclipse/none)
- Extra disasters may occur (winter deaths, predators, drought, plague)
- Each bunny may die based on age + overcrowding + events
- Eligible females produce litters (size influenced by spring + difficulty scaling)
- Vampire bunnies may convert others
- A summary prints births, deaths, and total population
- The simulation ends when the bunny population reaches 0.

Some additional notes:

Randomized bunny traits:

- Sex assignment
- Color inheritance + mutation chance (rare random colors)
- Rare “vampire” trait (low probability)
- Year-by-year simulation loop:

Aging

- Natural deaths based on age (separate mortality curves for normal vs vampire bunnies)
- Overcrowding increases mortality
- Seasonal / world events that affect survival and growth:
- Harsh Winter (higher mortality)
- Abundant Spring (bigger litters)
- Predator Season (random kills)
- Eclipse (boosts vampire conversions)
- Plague system with randomized disease names and mortality rates
- An optional drought penalty affecting fertility

Vampire mechanics:

- Vampires can “curse” non-vampire bunnies each year
- During an Eclipse, conversions are stronger
- Vampires have partial resistance to plague
- Outputs a full simulation log to Bunny_Farm_Simulation.txt

# Execution

To run and compile this program, use the command  g++ BUNNYFARM.CPP, and it will print text to the console window, informing the user about what is currently happening in the simulation each year. Further reference regarding what happens can also be found in the text file "Bunny_Farm_Simulation.txt", as the program also outputs all events that occur to that file. 

To run the following program, clone the repository to your computer and then compile it. This project was written in C++ and was tested on Windows using MSYS2 (MinGW64).

### Requirements
- g++ (C++17)

### Compile
g++ BUNNYFARM.CPP -o program

### Run
./program

# Purpose

This program was designed to be an entertaining way to integrate all the knowledge I gained regarding fundamental C++ concepts in the first semester of computer science, with a special emphasis on using classes and vectors. This was not assigned in any way, shape, or form; it was completely optional. Hence, I present this functional program as a demonstration of what I have learned. 


