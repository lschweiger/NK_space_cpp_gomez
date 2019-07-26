# Agent based model

Research for prof Charles Gomez.

# WIP

Currently NK space generation is mostly complete and,
action_space; the basics are done, will need to add to the class structure and add more functions.

**Warning: running `./NK_space_gen` will create 1000 spaces with a total the size of about 19GB**
# To compile just run (on Linux and mac)

`g++ NK_space_gen.cpp -o NK_space_gen` and<br /> 
`g++ NK_space_action.cpp -o NK_space_action`<br />
and run with <br />
`./NK_space_gen` and<br />
`./NK_space_action start end -S (search heuristic )` ; where start is a number and end is larger number and search heuristic is either 0,1,2,3;
i.e `./NK_space_action 0 10 -S 1`