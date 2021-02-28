# Agent based model

Research for prof Charles Gomez.

# WIP

Currently NK space generation is complete and,
action_space; mostly complete, mostly adding new functions and options in functions

**Warning: running `./NK_space_gen` will create 10000 spaces with a total the size of about 24GB**
# To compile and Run(on Linux and mac)

`g++ NK_space_gen.cpp -o NK_space_gen` and<br /> 
`g++  Agents.cpp NK_space_action-c8.cpp -o NK_space_action-c8 -O2 -std=c++17`<br />
If available OpenMP can be used to take advantage of multiple cores using the following <br /> 
`g++ -fopenmp Agents.cpp NK_space_action-c8.cpp -o NK_space_action-c8 -O2 -std=c++17`
#Run

`./NK_space_gen` to run and Generate NK spaces. If one would like to increase or decrease the Number of NK spaces generated, change the value the variable named loop (NK_space_gen will need to be recompiled).<br />
and Run the simulation with
`./NK_space_action-c8 -s # -e # -S # -c # -m # -C #-M ltr -P prob# `<br />
-s Start NKspace, requires a number greater than or equal to 0 <br />
-e End NKspace, requires a number greater than -s 
**Warning right now there is not safe check if the input number is greater than the largest NK space generated**<br />
-S Search heuristics either 0,1,2 or 3 (No longer used)<br />
-c Selects either Majority, Minority, or Merit seeking where needed, either 1=Majority, -1=Minority , -9 for baseline, or 9 for Merit swapping <br />
-m Selects either symmetric method or asymmetric method, 1=Matrix, -1=Connections <br />
-C Minority Criterion, can have the value of 4,5,6. <br />
-M If passed morphing will be used in agent exploit phase, active if the lowercase letter z is present <br />
-P If passed either exponential score difference is use or raw number, -1=Exponential score difference, 0<=P<=1 raw probability is used <br />

# Important Functions and Their details<br />
**NK space generation**:<br />
A list of 2^15 of randomly generated numbers between 0 and 1, is created up to 10000 times, each file is given an ID that is the NK space ID number. The NK spaces also may have a complexity value $K$ that changes the values. There are currently 4 values for K: 1,3,5,10.<br />

**AB random list**:<br />
For every NK space, a random generated vector(list) of A's and B's is used to fill all agents species variable. To start a vector of size 50, is filled with either **A** or **B** chosen at random by a bernoulli distribution with success equal to 0.5, where 1 is **A** and 0 is **B**. A seed is used to file the vector, the seed is based on  the NK space ID number plus 1 i.e NK spaces 10 will be seeded using the number 11. This allows the different complexity values to be used and still have the same assignment of A, B for each Nk spaces with the same ID.<br />

**Agent minority status**:<br /> 
The value of -m will invoke either agent_minority_status_asymmetric agent_minority_status_symmetric, Both of which will count and set an internal flag(0 or 1) that indicates whether the agent is in the Minority where 0 is no, and 1 is yes. 
For symmetric, Agents will always be flagged when the number of similar species is less or equal to half the total number of connections (currently 6) plus 1 (6/2 +1 =4).
For asymmetric -C (Criterion) will be used as the cut of number but strictly less than Criterion.<br />

**Agent exploit**:<br />
All agents will attempt to exploit by checking all connections and comparing scores, if a neighbour is better, it will take both the string and score from that neighbour. A flag is set if an agent is able to exploit if not said agent will then explore.<br />

**Agent explore**:<br />
Any agent that did not exploit, meaning of all it's neighbours the agent itself has the best score, the agent will now attempt to explore. There are 4  sub-functions that get called every round randomly.<br />
- random_0: randomly selects a bit in bits of the string and flips it to 0/1.
- random_1: randomly selects a bit in bits of the string and flips it to 1.
- explore_flip: randomly selects a even/odd bit in the string and flips it.
- explore_shuffle randomly shuffles the string by permutation.<br />

**Agent swap symmetric**:<br />
With symmetric all agents must beware if a connection is broken and will need to have the spot filled, and all connections must be bidirectional. In addition all agents must have 6 connection. If/When an agent needs to swap ties and flagged in the minority, the agent will swap a connection while maintaining every requirement for the other agents that was used in the swap. Details below of how the Algorithm works:<br />
- Quick explanation:<br /> From the flagged Agent (labeled **V<sub>1</sub>**) for swapping, we select a neighbour(**V<sub>2</sub>**) and from this neighbour select another agent (that is it is a second degree connection **V<sub>3</sub>**) that is not connected to the flagged agent. From this second degree agent we randomly select a new agent(**V<sub>4</sub>**). Finally to swap, in place of the selected neighbour(**V<sub>2</sub>**) in its place the second degree agent(**V<sub>3</sub>**) and also replace **V<sub>3</sub>**  with **V<sub>4</sub>** in **V<sub>2</sub>**.<br />
- **Detailed explanation**:<br />
The selected Agent is label **V<sub>1</sub>** and the selected neighbour labeled **V<sub>2</sub>**
We collect all  second degree from **V<sub>1</sub>** by way of all the **V<sub>2</sub>** s. We label them **V<sub>3<sub>j</sub></sub>**, we filter out all the **V<sub>3<sub>j</sub></sub>** that are also connected to **A**.
From the remaining **V<sub>3<sub>j</sub></sub>** a randomly selected **V<sub>3</sub>** is chosen, we pick from this **V<sub>3</sub>** a connection labeled **V<sub>4</sub>** that cannot be connected to either **V<sub>1</sub>** or **V<sub>2</sub>**. Finally we swap the **V<sub>1</sub>**&#x2194;**V<sub>2</sub>** connection to become **V<sub>1</sub>**&#x2194;**V<sub>3</sub>** and **V<sub>2</sub>**&#x2194;**V<sub>3</sub>** to become **V<sub>2</sub>**&#x2194;**V<sub>4</sub>**.
![Alt text](images/algo_start.jpg?raw=true "Before Tie swapping")
![Alt text](images/algo_end.jpg?raw=true "After Tie swapping")

**Agent swap Asymmetric**:<br />
Similar to Symmetric, but the only requirement is that Agents maintain 6 connection.  Agents do not have the requirement that the connection be bidirectional.Meaning when agent **V<sub>1</sub>** loses a connection to **V<sub>2</sub>** and connects to **V<sub>3</sub>**, **V<sub>1</sub>**&#8594;**V<sub>2</sub>** to **V<sub>1</sub>**&#8594;**V<sub>3</sub>**. However the connection  **V<sub>2</sub>**&#8594;**V<sub>1</sub>** is still present in **V<sub>2</sub>**'s list of connection, and **V<sub>3</sub>** is not aware that **V<sub>1</sub>** is connected to **V<sub>3</sub>**. Furthermore **V<sub>4</sub>** has no role in Asymmetric.

**Agent search heuristics**<br/>
When agents cannot exploit, they will move on to explore.
There are 4 search heuristics that can be performed that are randomly assigned for all agents once per round. 
- agent_explore_random_0: a bit randomly chosen in the string will be flipped to a Zero
- agent_explore_random_1: a bit randomly chosen in the string will be flipped to a One
- agent_explore_flip: a bit randomly chosen in the string will be flipped from what is was to the other 0 to 1 or 1 to 0.
- agent_explore_shuffle: The current string will be sampled from, with at least 2 index locations to create a substring. The agent then rotates the values of the selected index locations to the right with looping around. Once complete the substring is inserted back into the in the original index locations with the new rotated values.
i.e if 3 ,5, 8 chosen then the value at 3 is moved to 5, 5 to 8 and 8 to 3. once place back into the full string 3 will have the value of 8, 5 will have 3's value'and , 8 will have 5's value.