# Agent based model

Research for prof Charles Gomez.

# WIP

Currently NK space generation is complete and,
action_space; mostly complete and just adding new function and options in functions

**Warning: running `./NK_space_gen` will create 1000 spaces with a total the size of about 19GB**
# To compile and Run (on Linux and mac)

`g++ NK_space_gen.cpp -o NK_space_gen` and<br /> 
`g++ NK_space_action.cpp -o NK_space_action`<br />
and run with <br />
`./NK_space_gen` and<br />
`./NK_space_action -s # -e # -S # -c # -m # -M ltr -P prob# `<br />
-s Start NKspace, requires a number greater than 0 <br />
-e End NKspace requires a number greater than -s <br />
-S Search heuristics either 0,1,2 or 3 <br />
-c Selects Majority or minority seeking where needed, either 1=Majority, -1=Minority , -9 for baseline, or 0 for Permutation swapping <br />
-m Selects either Matrix method or connection method, 1=Matrix, -1=Connections <br />
-M If passed morphing will be used in agent exploit phase, active if the lowercase letter z is present <br />
-P If passed either exponential score difference is use or raw number, -1=Exponential score difference, 0<=P<=1 raw probability is used <br />
If available OpenMP can be used to take advantage of multiple cores using the following <br /> 
`g++ -fopenmp NK_space_action.cpp -o NK_space_action`