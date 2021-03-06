//Header file for Agent class
//
#ifndef AGENTS6_H
#define AGENTS6_H
class Agent // class for agents; includes variable and functions.
{
public:
    char species='1';            	// agent species type A or B are valid characters 
    int minority=0;            	// is set if agent is in minority by species/scores compared to it connections. valid values are 0,1
    int id=0;                   //agent id number 0 to agentcounta that is set in space-action
    int flag = 0;				//flag is used in exploit and explore, valid values are -1, 0,1. -1 meanings agents failed exploit and will run explore, 0 is either nothing or has already explored
    //int mutate_flag=0;          // flag for debugging mutate function 
    string binarystring="000000000000000";    	//stores main string
    string tempstring=  "000000000000000";        	//stores temp string if agent exploits
    double score=0.0;            	//stores primary score
    double tempscore=0.0;        	//stores temp score if agent exploits
    double move_avg=0.0;
    vector<int> connections=vector<int>(6);        //array of connections will be filled by agentconnections
    vector<int> tempconnections=vector<int>(6);
    unsigned int connection_replace=1; // number of connections to replace, default is 1 and may be changed.


void agent_connections(int num, Agent &input_agent);
void agent_change(int num, Agent &input_agent, const vector<string> &istring, const vector<double> &val);
void agent_minority_status_symmetric(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,int condition);
void agent_minority_status_asymmetric(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f, int Criterion,int condition);
void agent_minority_status_merit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f, int Criterion);
void agent_minority_status_merit_avg(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f, int Criterion);
void morph_agent_exp(Agent &input,Agent target,double diffscore);
void morph_agent_100(Agent &input,Agent target,double prob);
void agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,double prob,char method,int mode,int Criterion,int condition, const std::vector<double> &val);
void agent_swap_interal_info(Agent &input_agent);
void agent_explore_A_10(Agent &input_agent, const vector<double> &val);
void agent_explore_B_10(Agent &input_agent, const vector<double> &val);
void agent_explore_A_even(Agent &input_agent, const vector<double> &val);
void agent_explore_B_odd(Agent &input_agent, const vector<double> &val);
void agent_explore_A_swap(Agent &input_agent, const vector<double> &val);
void agent_explore_B_swap(Agent &input_agent, const vector<double> &val);
void agent_explore_A_flip(Agent &input_agent, const vector<double> &val);
void agent_explore_B_flip(Agent &input_agent, const vector<double> &val);
void agent_explore_random_1(Agent &input_agent, const vector<double> &val,std::vector<int> seq);
void agent_explore_random_0(Agent &input_agent, const vector<double> &val,std::vector<int> seq);
void agent_explore_flip(Agent &input_agent, const vector<double> &val,std::vector<int> seq);
void agent_explore_shuffle(Agent &input_agent,  const vector<double> &val,std::vector<int> seq);
void agent_explore(Agent &input_agent, const vector<double> &val,int search);
void agent_explore_new(Agent &input_agent, const vector<double> &val,int Asearch,int Bsearch,std::vector<int> seq);
std::vector<int> pool_con(Agent input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f);
void connection_swap(Agent &input_agent); //swaps connections with tempconnections;
vector<int> agent_connections_score_match(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f); //used for merit
vector<int> agent_connections_score_match_avg(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f); //used for merit with average
void agent_asymmetric_swap(int num,vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, int loop);//connections method swap
void agent_swap_symmetric(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,  Agent e, Agent f,int loop);//matrix method swap
void agent_swap_hack_asymmetric(int num,vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int Criterion,vector<int> agent_list);//solution to solving more than one connection swapped
void agent_swap_hack_symmetric(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,vector<int> agent_list);//same as above
vector<int> agent_connections_species_match(vector<Agent> Agents,Agent input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int condition);// checks connected agents species based on condition and returns vector, used in swaping hack  
void matrix_fill_before(int id,vector<int> filler); //debugging
void matrix_fill_after(int id,vector<int> filler);//debugging
void matrix_print(vector<Agent> agent_array);//debugging
void swap_species(Agent &input_agent,double& epsilon);
void agent_exploit_weighted(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,int mode,int Criterion,int condition,std::vector<double> &val);
void agent_exploit_weighted_inverse(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,int mode,int Criterion,double prob,int condition);
void agent_string_bitflip(Agent &input_agent,double prob,std::vector<double> &val);

//Global connection swapping functions

void agent_asymmetric_swap_global(int num,vector<Agent> &Agents,Agent &input_agent,int loop,int Criterion);//connections method swap hack that calls , getting agents globally
void agent_swap_hack_asymmetric_global(int num,vector<Agent> &Agents,Agent &input_agent, int Criterion);
void agent_asymmetric_swap_global_metric(int num,vector<Agent> &Agents,Agent &input_agent,int loop,int Criterion);
void agent_swap_hack_asymmetric_global_metric(int num,vector<Agent> &Agents,Agent &input_agent, int Criterion,vector<int> list);

};



#endif