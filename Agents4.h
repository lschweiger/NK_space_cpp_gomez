//Header file for Agent class
//
#ifndef AGENTS4_H
#define AGENTS4_H
class Agent // class for agents; includes variable and functions.
{
public:
    char species='1';            	// agent species type
    int minority=0;            	// is agent in minority by species compared to it connections
    int id=0;
    int flag = 0;				//flag is use in swap and exploit
    //int mutate_flag=0;          // flag for debugging mutate function 
    string binarystring="0000000000000000";    	//stores main string
    string tempstring="000000000000000";        	//stores temp string if agent exploits
    double score=0.0;            	//stores primary score
    double tempscore=0.0;        	//stores temp score if agent exploits
    vector<int> connections=vector<int>(4);        //array of connections will be filled by agentconnections
    vector<int> tempconnections=vector<int>(4);
    unsigned int connection_replace=1; // number of connections to replace, default is 8 and may be changed.


void agent_connections(int num, Agent &input_agent);
void agent_change(int num, Agent &input_agent, vector<string> &istring, vector<double> &val);
void agent_minority_status(Agent &input_agent, Agent a, Agent b, Agent c, Agent d);
void morph_agent_exp(Agent &input,Agent target,double diffscore);
void morph_agent_100(Agent &input,Agent target,double prob);
void agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,double prob,char method);
void agent_swap_interal_info(Agent &input_agent);
void agent_explore_A_10(Agent &input_agent, vector<double> &val);
void agent_explore_B_10(Agent &input_agent, vector<double> &val);
void agent_explore_A_even(Agent &input_agent, vector<double> &val);
void agent_explore_B_odd(Agent &input_agent, vector<double> &val);
void agent_explore_A_swap(Agent &input_agent, vector<double> &val);
void agent_explore_B_swap(Agent &input_agent, vector<double> &val);
void agent_explore_A_flip(Agent &input_agent, vector<double> &val);
void agent_explore_B_flip(Agent &input_agent, vector<double> &val);
void agent_explore(Agent &input_agent, vector<double> &val,int search);
void connection_swap(Agent &input_agent);
void agent_minority_swap(int num,Agent &input_agent,Agent a, Agent b, Agent c, Agent d);
void agent_swap_con(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, int loop);
void agent_swap_hack(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d);
void matrix_fill_before(int id,vector<int> filler);
void matrix_fill_after(int id,vector<int> filler);
void matrix_print(vector<Agent> agent_array);
void swap_species(Agent &input_agent,double& epsilon);
void agent_exploit_weighted(Agent &input_agent, Agent a, Agent b, Agent c, Agent d);
void agent_exploit_weighted_inverse(Agent &input_agent, Agent a, Agent b, Agent c, Agent d);
};



#endif