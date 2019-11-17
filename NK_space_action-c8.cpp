#ifdef _OPENMP
# include <omp.h>
#endif

#include <iostream>

using std::cout;
using std::endl;
using std::to_string;
using std::string;

#include <fstream>
#include <utility>
#include <set>
#include <vector>  //vectors

using std::vector;
#include <iomanip>
#include <bitset> //binary to numbers
#include <cmath>
#include <random>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <string>
//#include <omp.h>
//#include <thread>
int n = pow(2, 20);
const int nbgh = 100;
const int agentcount = 100;
int matrix[100][100]={};

void open_space_scores(int j, vector<double> &input_vec) {
    std::ifstream in_scores;
    double i_score;
    in_scores.open("NK_space_scores_" + to_string(j) + ".txt",std::ios::in | std::ios::binary);
    //while (in_scores>>i_score) {input_vec[i] = i_score; ++i; cout<<input_vec[i]<<endl;}
    for (int i = 0; i < ::n; ++i) {
        in_scores >> i_score;
        //cout<<i_score<<"test"<<'\n';
        input_vec[i] = i_score;
        //cout<<input_vec[i]<<'\n';
    }
    in_scores.close();
}

void open_space_string(vector <string> &string_vec) {
    std::fstream strings;
    strings.open("NK_space_strings.txt");
    for (int i = 0; i < ::n; ++i) {
        string i_str;
        strings >> i_str;
        string_vec[i] = i_str;
    }
    strings.close();
}

void A_list(vector<char> &types){//creates list of A and B and  assigns one to an agent at the beginning of every new NK_Space
    std::vector<char> sp(100);
    std::fill(sp.begin(),sp.end(),'A');
    for (unsigned int i = 0; i < sp.size(); ++i)
    {
        types[i]=sp[i];
    }   
}
void AB_list(vector<char> &types){//creates list of A and B and  assigns one to an agent at the beginning of every new NK_Space
    std::vector<char> sp={'A','A','B','B','B','A','A','B','A','B','B','A','A','A','B','B','B','B','A','A','B','B','A','B','B','A','A','B','B','B','B','B','B','B','A','A','A','B','B','B','A','A','A','B','B','B','A','A','B','B','B','B','A','A','B','B','A','B','B','A','B','A','B','A','A','B','B','A','B','A','B','A','A','B','A','B','B','A','A','B','B','A','B','B','A','B','A','B','B','B','B','A','A','A','A','B','B','A','A','B'};

    for (unsigned int i = 0; i < sp.size(); ++i)
    {
        types[i]=sp[i];
    }   
}

void AB_random_list(vector<char> &types, int inksp){//Random assignment of A and B
	    int ac=0;
    	int bc=0;
    	int temp=0;
    	for (int i = 0; i < 100; ++i)
    	{
    	std::bernoulli_distribution d(0.5);
    	std::mt19937 gen;
    	gen.seed(inksp+i);
    	
    	temp=d(gen);
    	if(temp==1) {types[i]='A'; ac++;}
    	if(temp==0) {types[i]='B'; bc++;}
    	//cout<<temp<< ":V C:"<<types[i]<<" ";
    	d.reset();
    	}
    	//cout<<endl;
    	//cout<<ac<<":A B:"<<bc<<endl;
    	//std::cin.get();
}


void ABCDE_list(vector<char> &types){//creates list of A, B,C,D,E and randomly assigns one to an agent at the beginning of every new NK_Space
    vector<char> AB(::agentcount);
    for (int i = 0; i < ::agentcount; i++) {
        if (i%5==0) AB[i] = 'A';
        if (i%5==1) AB[i] = 'B';
        if (i%5==2) AB[i] = 'C';
        if (i%5==3) AB[i] = 'D';
        if (i%5==4) AB[i] = 'E';
    }
    int acount=0;
    int bcount=0;
    int ccount=0;
    int dcount=0;
    int ecount=0;
    int temp = 0;
    for (int i = 0; i < ::agentcount; ++i) {
        temp = rand() % (::agentcount - i);
        types[i] = AB[temp];
        AB.erase(AB.begin() + temp);
        if(types[i]=='A') acount++;
        //cout<<types[i]<<" ";
        if(types[i]=='B') bcount++;
        if(types[i]=='C') ccount++;
        if(types[i]=='D') dcount++;
        if(types[i]=='E') ecount++;
        
    }
}
void AB_list_deseg(vector<char> &types){//creates list of A and B and randomly assigns one to an agent at the beginning of every new NK_Space
    vector<char> AB(::agentcount);
    for (int i = 0; i < ::agentcount; ++i) {
        if (i%2==0) types[i] = 'A';
        if (i%2==1) types[i] = 'B';
    }
}

void AB_list_seg(vector<char> &types){//creates list of A and B and randomly assigns one to an agent at the beginning of every new NK_Space
    vector<char> AB(::agentcount);
    for (int i = 0; i < ::agentcount; ++i) {
        if (i < 50) types[i] = 'A';
        if (i >= 50) types[i] = 'B';
    }
}



class Agent // class for agents; includes variable and functions.
{
public:
    char species;            	// agent species type
    int minority=0;            	// is agent in minority by species compared to it connections
    int id=0;
    int flag = 0;				//flag is use in swap and exploit
    //int mutate_flag=0;          // flag for debugging mutate function 
    string binarystring;    	//stores main string
    string tempstring;        	//stores temp string if agent exploits
    double score;            	//stores primary score
    double tempscore;        	//stores temp score if agent exploits
    vector<int> connections=vector<int>(8);        //array of connections will be filled by agentconnections
    vector<int> tempconnections=vector<int>(8);
    

    void agent_connections(int num, Agent &input_agent) {
        input_agent.id = num; //assigns id from 0 to 99,
        //sets the default connections to -2,-1,1,2 of the current agents id, includes a check for negatives id
        if ((num - 4) % (::nbgh) < 0) {
            input_agent.connections[0] = ((num - 4) % (::nbgh)) + ::nbgh;
        } else {
            input_agent.connections[0] = num - 4;
        }
        if ((num - 3) % (::nbgh) < 0) {
            input_agent.connections[1] = ((num - 3) % (::nbgh)) + ::nbgh;
        } else {
            input_agent.connections[1] = num - 3;
        }
        if ((num - 2) % (::nbgh) < 0) {
            input_agent.connections[2] = ((num - 2) % (::nbgh)) + ::nbgh;
        } else {
            input_agent.connections[2] = num - 2;
        }
        if ((num - 1) % (::nbgh) < 0) {
            input_agent.connections[3] = ((num - 1) % (::nbgh)) + ::nbgh;
        } else {
            input_agent.connections[3] = num - 1;
        }
        input_agent.connections[4] = (num + 1) % (::nbgh);
        input_agent.connections[5] = (num + 2) % (::nbgh);
        input_agent.connections[6] = (num + 3) % (::nbgh);
        input_agent.connections[7] = (num + 4) % (::nbgh);
        std::sort(input_agent.connections.begin(),input_agent.connections.end());
    };

    void agent_change(int num, Agent &input_agent, vector<string> &istring, vector<double> &val) {
        // changes sets score and binarystring by random value.
        input_agent.score = val[num];
        //cout<<val[num]<<" vec test"<<endl;
        //cout<<input_agent.score<<" test" <<endl;
        input_agent.binarystring = istring[num];
    };

    void agent_minority_status(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
        int minorcalc = 0;
        int samecalc = 0;
        if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;
        if (input_agent.species != e.species) minorcalc++;
        if (input_agent.species != f.species) minorcalc++;
        if (input_agent.species != g.species) minorcalc++;
        if (input_agent.species != h.species) minorcalc++;

        if (input_agent.species == a.species) samecalc++;
        if (input_agent.species == b.species) samecalc++;
        if (input_agent.species == c.species) samecalc++;
        if (input_agent.species == d.species) samecalc++;
        if (input_agent.species == e.species) samecalc++;
        if (input_agent.species == f.species) samecalc++;
        if (input_agent.species == g.species) samecalc++;
        if (input_agent.species == h.species) samecalc++;

        //cout<<minorcalc<<"out"<<endl;
        if ((samecalc+1)<=4)
        	{
        		//cout<<minorcalc<<"in"<<endl;
        		//cout<<(samecalc+1)<<endl;
        		//cout<<input_agent.id<<" "<<input_agent.species<<" "<<a.species<<" "<<b.species<<" "<<c.species<<" "<<d.species<<" "<<e.species<<" "<<f.species<<" "<<g.species<<" "<<h.species<<endl;
        		input_agent.minority = 1; 
        	//cout<<input_agent.id<<" minor id "<<" funct "<<input_agent.species<<endl;
        	}
		else{input_agent.minority=0;}
    
    };

    void morph_agent(Agent &input,Agent target,double diffscore){
        std::random_device rd;
        std::binomial_distribution<int> binom (1,(1-std::exp(-diffscore*3.46)));
        int bi=binom(rd);
    if (bi==1)
        {
            input.species=target.species;
        }
    
    };


    void agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h,vector<double> &valscores) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
        // will mutate agents string and score if they exploit
        double temscore = input_agent.score;
        string temstring = input_agent.binarystring;
        // minority calc and flag
        /*
        int minorcalc = 0;
        if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;
        if (input_agent.species != e.species) minorcalc++;
        if (input_agent.species != f.species) minorcalc++;
        if (input_agent.species != g.species) minorcalc++;
        if (input_agent.species != h.species) minorcalc++;
		if (minorcalc >=5) {cout<<input_agent.id<<" "<<input_agent.species<<" "<<a.species<<" "<<b.species<<" "<<c.species<<" "<<d.species<<" "<<e.species<<" "<<f.species<<" "<<g.species<<" "<<h.species<<endl;
		input_agent.minority = 1; cout<<input_agent.id<<" minor id "<<" exploit "<<input_agent.species<<endl;}
    	*/
        
        agent_minority_status(input_agent,a,b,c,d,e,f,g,h);
        //cout<<"minority status "<<input_agent.minority<<endl;
        //score update
        int target_id=-1;
        double diffscore=0.0;
        if (temscore < a.score) {
            temscore = a.score;
            temstring = a.binarystring;
            input_agent.flag = 1;//
            target_id=0;
        }

        if (temscore < b.score) {
            temscore = b.score;
            temstring = b.binarystring;
            input_agent.flag = 1;
            target_id=1;
        }

        if (temscore < c.score) {
            temscore = c.score;
            temstring = c.binarystring;
            input_agent.flag = 1;
            target_id=2;
        }

        if (temscore < d.score) {
            temscore = d.score;
            temstring = d.binarystring;
            input_agent.flag = 1;
            target_id=3;
        }

        if (temscore < e.score) {
            temscore = e.score;
            temstring = e.binarystring;
            input_agent.flag = 1;
            target_id=4;
        }

        if (temscore < f.score) {
            temscore = f.score;
            temstring = f.binarystring;
            input_agent.flag = 1;
            target_id=5;
        }

        if (temscore < g.score) {
            temscore = g.score;
            temstring = g.binarystring;
            input_agent.flag = 1;
            target_id=6;
        }

        if (temscore < h.score) {
            temscore = h.score;
            temstring = h.binarystring;
            input_agent.flag = 1;
            target_id=7;
        }
        diffscore=std::abs(temscore-input_agent.tempscore);
        input_agent.tempscore = temscore;
        input_agent.tempstring = temstring;

        if(input_agent.flag==1){
            //char test =input_agent.species;
            //cout<<input_agent.id<<"\t"<<test<<endl;
            Agent target_agent;
            switch(target_id)
            {
                case 0: target_agent=a;break;
                case 1: target_agent=b;break;
                case 2: target_agent=c;break;
                case 3: target_agent=d;break;
                case 4: target_agent=e;break;
                case 5: target_agent=f;break;
                case 6: target_agent=g;break;
                case 7: target_agent=h;break;
            }
            morph_agent(input_agent,target_agent,diffscore);
            //char new_test=input_agent.species;
            //cout<<input_agent.id<<"\t"<<new_test<<endl;
        }
        if (input_agent.flag == 0) input_agent.flag = -1;
    };

    void agent_swap_interal_info(Agent &input_agent) {
        //checks flag and swaps scores and string, for next round if 1, and resets back to 0 for next round
        if (input_agent.flag == 1) {
            double temp = input_agent.tempscore;
            string stemp = input_agent.tempstring;
            input_agent.tempscore = input_agent.score;
            input_agent.tempstring = input_agent.binarystring;
            input_agent.score = temp;
            input_agent.binarystring = stemp;
            input_agent.flag = 0;
        }
    };
    void agent_explore_A_10(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 9);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            if (temstring[random] == '1') {
                temstring[random] = '0';
            } else {
                temstring[random] = '1';
            }
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };

    void agent_explore_B_10(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(10, 19);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            if (temstring[random] == '1') {
                temstring[random] = '0';
            } else {
                temstring[random] = '1';
            }
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };

    void agent_explore_A_even(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(1, 9);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen)*2;
        //cout<<random<<" A "<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            if (temstring[random] == '1') {
                temstring[random] = '0';
            } else {
                temstring[random] = '1';
            }
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };

    void agent_explore_B_odd(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(1, 9);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen)*2+1;
        //cout<<random<<" B "<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            if (temstring[random] == '1') {
                temstring[random] = '0';
            } else {
                temstring[random] = '1';
            }
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };

    void agent_explore_A_swap(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 18);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            int temp=temstring[random];
            temstring[random]=temstring[random+1];
            temstring[random+1]=temp;
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };
    void agent_explore_B_swap(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 19);
        std::uniform_int_distribution<> randm2(0, 19);
        std::random_device rdm;
        std::mt19937_64 gen(rdm());
        int random = randm(gen);
        int random2 = randm2(rdm);
        while(abs(random2-random)<=1) random2 = randm2(rdm); 
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            int temp=temstring[random];
            temstring[random]=temstring[random2];
            temstring[random2]=temp;
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };

    void agent_explore_A_flip(Agent &input_agent, vector<string> &istring, vector<double> &val){
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            for(int i=0;i<10;i++){
                if (temstring[i] == '1')
                {
                    temstring[i] = '0';
                } 
                else {
                    temstring[i] = '1';
                }
            }
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };

    void agent_explore_B_flip(Agent &input_agent, vector<string> &istring, vector<double> &val){
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            for(int i=10;i<20;i++){
            
                if (temstring[i] == '1')
                {
                    temstring[i] = '0';
                } 
                else {
                    temstring[i] = '1';
                }
            }
            std::bitset<20> newidbinary = std::bitset<20>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    };
    

    void agent_explore(Agent &input_agent, vector<string> &istring, vector<double> &val,int search) {
        // if agent with flag -1 will explore
        //cout<<search<<endl;
        if(input_agent.species=='A' && search==0) agent_explore_A_10(input_agent,istring,val);
        if(input_agent.species=='B' && search==0) agent_explore_B_10(input_agent,istring,val);
        if(input_agent.species=='A' && search==1) agent_explore_A_even(input_agent,istring,val);
        if(input_agent.species=='B' && search==1) agent_explore_B_odd(input_agent,istring,val);
        if(input_agent.species=='A' && search==2) agent_explore_A_swap(input_agent,istring,val);
        if(input_agent.species=='B' && search==2) agent_explore_B_swap(input_agent,istring,val);
        if(input_agent.species=='A' && search==3) agent_explore_A_flip(input_agent,istring,val);
        if(input_agent.species=='B' && search==3) agent_explore_B_flip(input_agent,istring,val);
        
    };

    void agent_minority_swap(int num,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
        // swaps agent that is in minority; by going through all connections i.e agent 98 will have the potential to get agents 90 through 6's connections
        // will randomly only get 8.
        
            srand(num);
            vector<int> temp=pool_con(input_agent,a,b,c,d,e,f,g,h);
            vector<int> newcons(8);
            for (int i =0; i<8;i++) {
               // cout<<i<<endl;
                std::uniform_int_distribution<> rands(0, (temp.size()-1));
                std::random_device rdm;
                std::mt19937 gen(rdm());
                int random = rands(gen);
                rands.reset();
                while(temp[random]==input_agent.id){// prevents the agent from getting itself as possible connections
                    std::uniform_int_distribution<> rands(0, (temp.size()-1));
                    random = rands(gen);
                    rands.reset();
                }
                newcons[i] = temp[random];
                //cout<<newcons[i]<<" ";
                temp.erase(temp.begin()+random);
                temp.resize(temp.size());
            }
            //cout<<endl;
            //cout<<"new"<<endl;
            for (int j = 0; j <8 ; ++j) {

                //cout<<newcons[j]<<"new con"<<endl;
                input_agent.tempconnections[j]=newcons[j];
            }
            sort(input_agent.tempconnections.begin(), input_agent.tempconnections.end());
        //when down resets flag back to 0, regardless of status
        connection_swap(input_agent);
        input_agent.minority=0;
        temp.clear();
        temp.resize(0);
    };

    void connection_swap(Agent &input_agent){// set connections equal to temp connection and clears, resize temp for future uses; for testing only
        input_agent.connections=input_agent.tempconnections;
        input_agent.tempconnections.clear();
        input_agent.tempconnections.resize(8);
    };

    std::vector<int> pool_con(Agent input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
        vector<int> temp(64);
        for(int i=0;i<8;i++){// fills temp with all possible connections including duplicates, will also fill temp up simultaneously from the other 8 agents
            temp[i]=a.connections[i];
                //cout<<a.id<<"id "<<a.connections[i]<<" a con"<<endl;
            temp[8+i]=b.connections[i];
                //cout<<b.id<<"id "<<b.connections[i]<<" b con"<<endl;
            temp[16+i]=c.connections[i];
                //cout<<c.id<<"id "<<c.connections[i]<<" c con"<<endl;
            temp[24+i]=d.connections[i];
                //cout<<d.id<<"id "<<d.connections[i]<<" d con"<<endl;
            temp[32+i]=e.connections[i];
                //cout<<e.id<<"id "<<e.connections[i]<<" e con"<<endl;
            temp[40+i]=f.connections[i];
                //cout<<f.id<<"id "<<f.connections[i]<<" f con"<<endl;
            temp[48+i]=g.connections[i];
                //cout<<g.id<<"id "<<g.connections[i]<<" g con"<<endl;
            temp[56+i]=h.connections[i];
                //cout<<h.id<<"id "<<h.connections[i]<<" h con"<<endl;
        }

            
            //sorts temp and removes duplicate values to create a vector
        temp.erase(std::remove(temp.begin(), temp.end(), input_agent.id), temp.end());
        sort(temp.begin(), temp.end());
        auto last = std::unique(temp.begin(), temp.end());
        temp.erase(last, temp.end());
        temp.resize(temp.size());
    return temp;
    };


    void agent_swap_con(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){ //swaps an agents connection using the adjacency matrix
        //matrix_fill_before(input_agent.id,input_agent.connections);
        //std::default_random_engine generator (2);
        int A=input_agent.id; //input agent named A 
        std::uniform_int_distribution<> selected(0, input_agent.connections.size()-1); //random selction for B
        std::random_device Srdm;
        std::mt19937 gen(Srdm());
        vector<int> pool=pool_con(input_agent,a,b,c,d,e,f,g,h); //set of all 1st degree connected agents that we pick C* from
        vector<int> old_cons=input_agent.connections;
        vector<int> C_potential;
        vector<int> D_potential;
        int B=old_cons[selected(gen)];
        for(unsigned int i=0;i<pool.size();i++){ //find and only adds agents for C* from the pool that are not connected to A
            bool found = false;
            for (auto & elem : Agents[pool[i]].connections)
                if (elem == A)
                {
                    found = true;
                    break;
                }
            if(found==true) continue;
            if(found==false)
                {
                	C_potential.push_back(pool[i]);
                }
        }
        //cout<<C_potential.size()<<" test 1"<<endl;
        std::uniform_int_distribution<> c_sel(0,C_potential.size()-1); //random selection of C* from C_potential.
        std::random_device Crdm;
        std::mt19937 genC(Crdm());
        int C=C_potential[c_sel(genC)];
        vector<int> C_cons=Agents[C].connections;
        //cout<<C_potential.size()<<" test 0"<<endl;
        for(unsigned int i=0;i<C_cons.size();i++){ // find and only adds agents for D* from the C* that are not connected to B
            bool found = false;
            for(auto & elem : Agents[C_cons[i]].connections)
            if (elem == B)
            	{
                	found = true;
                	break;
            	}
            if(found==true) continue;
            if(found==false)
                {
                    D_potential.push_back(C_cons[i]);
                }
        }
        while(D_potential.size()==0)
        {
            C=C_potential[c_sel(Crdm)];
            vector<int> C_cons=Agents[C].connections;
            //cout<<C_potential.size()<<" test 1"<<endl;
            for(unsigned int i=0;i<C_cons.size();i++){ // find and only adds agents for D* from the C* that are not connected to B
                bool found = false;
                for (auto & elem : Agents[C_cons[i]].connections)
                    if (elem == B || elem==A)
                    {
                        found = true;
                        break;
                    }
                    if(found==true) continue;
                    if(found==false)
                        {
                            D_potential.push_back(C_cons[i]);
                        }
            }
        }
        //cout<<D_potential.size()<<" test 3"<<endl;
        std::uniform_int_distribution<> d_sel(0,D_potential.size()-1);
        std::random_device Drdm;
        std::mt19937 genD(Drdm());
        int D=D_potential[d_sel(genD)];
        while(D==C||D==B) D=D_potential[d_sel(Drdm)];

        /*
        cout<<"before\n";
        cout<<Agents[A].id<<endl;
        for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[B].id<<endl;
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[B].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[C].id<<endl;
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[C].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[D].id<<endl;
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[D].connections[i]<<" ";
        }
        cout<<endl;
        */
        std::vector<int>::iterator ABit = std::find(Agents[A].connections.begin(), Agents[A].connections.end(), B);
        int ABindex = std::distance(Agents[A].connections.begin(), ABit);
        //cout<<Agents[A].connections[ABindex];
        std::vector<int>::iterator BAit = std::find(Agents[B].connections.begin(), Agents[B].connections.end(), A);
        int BAindex = std::distance(Agents[B].connections.begin(), BAit);
        //cout<<Agents[B].connections[BAindex];
        std::vector<int>::iterator CDit = std::find(Agents[C].connections.begin(), Agents[C].connections.end(), D);
        int CDindex = std::distance(Agents[C].connections.begin(), CDit); 
        //cout<<Agents[C].connections[CDindex];
        std::vector<int>::iterator DCit = std::find(Agents[D].connections.begin(), Agents[D].connections.end(), C);
        int DCindex = std::distance(Agents[D].connections.begin(), DCit);
        //cout<<Agents[D].connections[DCindex];
        Agents[A].connections[ABindex]=C;
        Agents[B].connections[BAindex]=D;
        Agents[C].connections[CDindex]=A;
        Agents[D].connections[DCindex]=B;
        /*
        cout<<"\n after\n";
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[B].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[C].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 8; ++i)
        {
            cout<<Agents[D].connections[i]<<" ";
        }
        cout<<endl;
        */
        std::sort(Agents[A].connections.begin(),Agents[A].connections.end());
        std::sort(Agents[B].connections.begin(),Agents[B].connections.end());
        std::sort(Agents[C].connections.begin(),Agents[C].connections.end());
        std::sort(Agents[D].connections.begin(),Agents[D].connections.end());
        input_agent.minority=0;
        //cout<<"info \n";
            //matrix_fill_after(input_agent.id,new_cons);            
    };

    void matrix_fill_before(int id,vector<int> filler)
        {//fills matrix with old connections
            for (unsigned int i = 0; i < filler.size(); i++)
            {
                ::matrix[id][filler[i]]=1;
                ::matrix[filler[i]][id]=1;
            }
        };

    void matrix_fill_after(int id,vector<int> filler)
        {// fill matrix with new connection
            for (unsigned int i = 0; i < filler.size(); i++)
            {
                ::matrix[id][filler[i]]=5;
                ::matrix[filler[i]][id]=5;
            }
        };
    void matrix_print(vector<Agent> agent_array)
    {//prints and counts new connections,
        memset(::matrix,0,sizeof(::matrix));
        for(int i=0;i<100;i++)
        {
            for (int j = 0; j < 100; j++)
            {
                for (unsigned int k = 0; k < agent_array[i].connections.size(); k++)
                {
                    if(agent_array[i].connections[k]==j)
                    {
                        ::matrix[i][agent_array[i].connections[k]]=5;
                    }
                }
                
            }
            
        }
        for (int i = 0; i < 100; ++i)
        {
            int count=0;
            for (int j = 0; j < 100; ++j)
            {
                if(::matrix[i][j]==5) count++;
                cout<<::matrix[i][j];
            }
            cout<<" num "<<count<<" "<<i<<endl;
        }
    };

	void swap_species(Agent &input_agent,double& epsilon)
	{//swaps agent species with given probability, to species type that is in majority 
		std::random_device rdm;
		std::mt19937_64 gen(rdm());
		std::binomial_distribution<int> binom (1,epsilon);
		int flag=0;
		int probchange=binom(gen);
		/*
		int minorcalc = 0;
		if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;
        if (input_agent.species != e.species) minorcalc++;
        if (input_agent.species != f.species) minorcalc++;
        if (input_agent.species != g.species) minorcalc++;
        if (input_agent.species != h.species) minorcalc++;
        */
    		if(probchange==1)
    		{
    			cout<<input_agent.id<<" agent species before "<<input_agent.species<<endl;
    			if(input_agent.species=='A')
    			{
    				input_agent.species='B';
    				flag=1;
    				cout<<"change "<<input_agent.species<<endl;
    			}
    			if(flag==0 && input_agent.species=='B')
    			{
    				input_agent.species='A';
    				cout<<"change "<<input_agent.species<<endl;
    			}
    		}
		//cout<<"agent species after "<<input_agent.species<<endl;
	}

};


void output_connections(int NKspace,vector<Agent> &Agents,int rounds){ //outputs all agents connections as a csv file; includes minority status
	std::ofstream out;
	out.open("agent connections "+to_string(NKspace)+" "+to_string(rounds)+".txt");
	out<<"Agent id #"<<","<<"species"<<","<<"connection 0"<<","<<"connection 1"<<","<<"connection 2"<<","<<"connection 3"<<","<<"connection 4"<<","<<"connection 5"<<","<<"connection 6"<<","<<"connection 7"<<","<<"minority status"<<"\n";
	for (vector<Agent>::iterator i = Agents.begin(); i != Agents.end(); i++)
	{
		out<<i->id<<","<<i->species<<","<<i->connections[0]<<","<<i->connections[1]<<","<<i->connections[2]<<","<<i->connections[3]<<","<<i->connections[4]<<","<<i->connections[5]<<","<<i->connections[6]<<","<<i->connections[7];
		if(i->minority==1) 
			{out<<","<<"M"<<"\n";} 
		else
			{out<<"\n";}
	}
}

void output_scores(int NKspace,vector<Agent> &Agents,int rounds){ //outputs all agents connections as a csv file; includes minority status
	std::ofstream out;
	out.open("agent scores round "+to_string(NKspace)+" "+to_string(rounds)+".txt");
	out<<"Agent id #"<<","<<"scores\n";
	for (vector<Agent>::iterator i = Agents.begin(); i != Agents.end(); i++)
	{
		out<<std::setprecision(15)<<i->id<<","<<i->score<<"\n";
	}
}

void output_round(int NKspace,int round,vector<int> rounds,vector<double> scr,vector<double> ag,vector<int> mc,vector<int> us,vector<double> pu,int search,int typeout,vector<double> same,vector<double> diff,char method){
	string outtype="";
    string outmethod;
	if(typeout==-9)  outtype ="baseline";
	if(typeout==-1)  outtype="minority";
	if(typeout==1)  outtype ="majority";
	if(typeout==0)  outtype ="Swap";
    if(method=='m') outmethod="matrix";
    if(method=='c') outmethod="connections";
    if(method=='s') outmethod="info_swap";
    if(method=='b') outmethod="basline";
    if(method=='z') {outtype="morph"; outmethod="morph";}
	std::fstream out("8c-NK_space_"+to_string(NKspace)+"_"+outtype+"_SH_"+to_string(search)+"_"+outmethod+".txt",std::ios::out | std::ios::binary);
	out<<"round,"<<"max score,"<<"avg score,"<<"Number of unique solutions,"<<"percent with max score,"<<"avg similiar species,"<<"avg different species,"<<"minority count"<<"\n";
	for (int i=0;i<=round; i++){
		out<<std::setprecision(15)<<rounds[i]<<","<<scr[i]<<","<<ag[i]<<","<<us[i]<<","<<pu[i]<<","<<same[i]<<","<<diff[i]<<","<<mc[i]<<"\n";
	}
}
void swapper(vector<Agent> &v,vector<int> &a,vector<int> &b){ //swaping function
        for(unsigned int i=0;i<a.size();i++){
            v[b[i]].species=v[a[i]].species;
            v[b[i]].tempstring=v[a[i]].binarystring;
            v[b[i]].tempscore=v[a[i]].score;
        }
    }
void swap_agents(vector<Agent> &v,int mode){//swaps minority agents by swaping information
        vector<int> minor;
        vector<int> permute;
        for (int i = 0; i < 100; ++i)
        {
            if(mode==1)
            {
                if(v[i].minority==1)
                {
                    minor.push_back(v[i].id);
                    v[i].minority=0; 
                }
            }
            if(mode==-1)
            {
                if(v[i].minority!=1)
                {
                    minor.push_back(v[i].id);
                    v[i].minority=0; 
                }
            }
        }
        permute=minor;
        std::random_shuffle ( permute.begin(), permute.end() );
        swapper(v,minor,permute);
        for(unsigned int i=0;i<minor.size();++i)
        {
        	v[permute[i]].agent_swap_interal_info(v[permute[i]]);
        }
    }
    

int main(int argc, char *argv[]) {
std::ios::sync_with_stdio(false); 
    std::stringstream convert1(argv[1]);
    std::stringstream convert2(argv[2]);
    std::string convert3(argv[3]);
    //std::string convert4(argv[4]);
    int start;
    convert1 >> start;
    int end;
    convert2 >> end;
    int searchm;
    double prob=0.0;
    int condition;
    int mode;
    char method='z';
    //cout<<argc<<" argc \n"<<endl;
    //for (int i=0;i<argc;++i) cout<<"argv"<<i<<" "<<argv[i]<<endl;
    if(convert3=="-S" && atoi(argv[4])<=3){
        searchm=atoi(argv[4]);
    }
    else{
        searchm=0;
    }

    if(argc>=6 && atof(argv[5])>0)
    {
    	if(atof(argv[5])>=0.0) 
    	{
    		prob=atof(argv[5]);
    		condition=0;
    		cout<<"using "<<prob<<endl;
    	} 
    	else
    	{
    		prob=0.05; 
    		condition=0;
    		cout<<"using default probability of 0.05 for species switch\n";
    	}
    }
    else
    {
    	cout<<"skipping species swap \n";
    	prob=-1;
    	if(argc==8)
            {
            mode=atoi(argv[7]);
            //mode ==1 uses agent_swap_con / matrix method
            //mode ==-1 uses agent_minority_swap / connection swap.
            condition=atoi(argv[6]);
            //coniditon selects majority seeking 1 or minority seeking -1 or uses swapping 0
            //or -9 for baseline
            }
    }
    int NKspace_num=start;
    vector <Agent> agent_array(::agentcount);
    cout.setf(std::ios::fixed);
    cout.setf(std::ios::showpoint);
    cout.precision(15);
    vector <string> NKspacevals(::n);
    vector<double> NKspacescore(::n);
    open_space_string(NKspacevals);

    double max = 0.0;
    double avgscore = 0.0;
    int percuni=0;
    vector<double> maxscore(100);
    vector<int> maxround(100);
    vector<int> minoritycount(100);
    vector<double> avgscores(100);
    vector<char> type(100);
    vector<int> uniquesize(100);
    vector<double> percentuni(100);
    vector<double> elts(100);
    vector<double> same(100);
    vector<double> diff(100);
    //AB_list(type);
    //AB_random_list(type,0);

	int rounds = 0;
    int nums = 0;
    int mcount=0;

#pragma omp parallel for default(none) shared(end,searchm,::n,NKspacevals,NKspace_num,cout,prob,argc,condition,mode,method) firstprivate(max,avgscore,percuni,maxscore,maxround,minoritycount,avgscores,uniquesize,percentuni,NKspacescore,rounds,mcount,agent_array,type,nums,elts,same,diff)   schedule(dynamic,25)
	for(int inksp=NKspace_num;inksp<end;++inksp){
		//srand(inksp+1);
    	cout<<"NK_space #:"<<inksp<<endl;
    	#ifdef _OPENMP
    	cout<<"\t thread #: "<<omp_get_thread_num()<<endl;
    	#endif
		if(inksp>0)
		{
			agent_array.clear();
			NKspacescore.clear();
			maxscore.clear();
			maxround.clear();
			minoritycount.clear();
			avgscores.clear();
			type.clear();
	        uniquesize.clear();
	        percentuni.clear();
			agent_array.resize(100);
			NKspacescore.resize(100);
			elts.clear();
			elts.resize(100);
			maxscore.resize(100);
			maxround.resize(100);
			minoritycount.resize(100);
			avgscores.resize(100);
			type.resize(100);
	        uniquesize.resize(100);
	        percentuni.resize(100);
            same.clear();
            diff.clear();
            same.resize(100);
            diff.resize(100);
	        percuni=0;
			rounds = 0;
	    	nums = 0;
	    	mcount=0;
	    	max=0.0;
	        //::matrix[100][100]={};
	        //std::cin.get();
    	}
    //A_list(type);
	AB_random_list(type,inksp);
	open_space_scores(inksp, NKspacescore);
    for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i) 
    {// assignment of information to agents, changes for each NK_Space
    	std::uniform_int_distribution<> ids(0,::n); //r
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int rnums=ids(gen);
        //cout<<rnums<<"\n";
        i->agent_connections(nums, *i);
        i->species = type[nums];
        //cout<<i->id<<endl;
        //cout<<i->species<<endl;
        i->agent_change(rnums, *i, NKspacevals, NKspacescore);

        nums++;
    }

    for (; rounds < 100; ++rounds) {
    	elts.clear();
		elts.resize(100);
    	/*
		intial data collection before every round starts its run
		runs through all agents to find number of unique solutions, max score and percent unique
    	*/
    	if(rounds==0){
    		        	for (int i=0; i<::agentcount;++i)
        		{// number of unqiue scores
        			elts[i]=agent_array[i].score;
        		}
        	std::sort(elts.begin(),elts.end());
        	auto ip=std::unique(elts.begin(),elts.end());
        	elts.erase(ip,elts.end());
        	uniquesize[rounds]=elts.size();

        max=0;
        for (int i = 0; i < ::agentcount; ++i) 
        {
            //cout<<agent_array[i].score<<endl;
            if (max < agent_array[i].score) 
            {
                max = agent_array[i].score;
            }

        }
        percuni=0;
        
        for (int i = 0; i < 100; ++i)
        {// percent of agents with max score
            if(max==agent_array[i].score)
            {
                percuni++;
                //cout<<percuni<<" ";
            }

        }
        percentuni[rounds]=(percuni);
        }
        //cout<<"round #"<<rounds<<endl;


        //cout<<percentuni[rounds]<<endl;
        mcount = 0;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i) 
        {
            i->agent_minority_status(*i, agent_array[i->connections[0]], agent_array[i->connections[1]],
                            agent_array[i->connections[2]], agent_array[i->connections[3]],
                            agent_array[i->connections[4]], agent_array[i->connections[5]],
                            agent_array[i->connections[6]], agent_array[i->connections[7]]);
            if (i->minority == 1) 
            {
                mcount++;
            }
            i->minority=0;
        }
        
        /*
		end of data collection
        */
        //cout<<mcount<<endl;
        minoritycount[rounds] = mcount;
       
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i)
        {
            i->agent_exploit(*i, agent_array[i->connections[0]], agent_array[i->connections[1]],
                            agent_array[i->connections[2]], agent_array[i->connections[3]],
                            agent_array[i->connections[4]], agent_array[i->connections[5]],
                            agent_array[i->connections[6]], agent_array[i->connections[7]],NKspacescore);
            //cout<<i->id<<" \033[1;31mid\033[0m "<<i->species<<" \033[1;32mspecies\033[0m "<<" "<<i->mutate_flag<< "\n";
            //cout<<i->minority<<" minority"<<" \033[1;34mnew_string\033[0m "<<endl;
            
        }


        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i) {
            i->agent_swap_interal_info(*i);
            //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
            //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
        }


        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i) 
        {
            if (i->flag == -1) 
            {
                i->agent_explore(*i, NKspacevals, NKspacescore,searchm);
                //agent_array[i].agentexplore(agent_array[i],NKspace);
                //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
                //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
            }
        }
        if(condition==-9)
        {
            method='b';
            //runs baseline so nothing here
        }
        else
        {
            if(prob==-1)
            { //if argv>=6 and probability is not given all connection manipulations are skipped
    	        if(condition==0)
                {
                    swap_agents(agent_array,mode);
                    method='s';
                }
                
    	        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i) 
    	        {
    	           if (condition==1)
                   {

                        if (i->minority == 1) 
    	               {//major seeking
    	                   //cout<<i->id<<" id \n";
    	              
    	                   if(mode==1)
                            {//matrix
                                
                                method='m';
    	                       i->agent_swap_con(agent_array,*i,
    	                                       agent_array[i->connections[0]],agent_array[i->connections[1]],
    	                                       agent_array[i->connections[2]], agent_array[i->connections[3]],
    	                                       agent_array[i->connections[4]], agent_array[i->connections[5]],
    	                                       agent_array[i->connections[6]], agent_array[i->connections[7]]);
    	                   }
                            if(mode==-1)
                            {// connection reassignment
                                
                                method='c';
    	                       i->agent_minority_swap(i->id,*i,
    	                                       agent_array[i->connections[0]],agent_array[i->connections[1]],
    	                                       agent_array[i->connections[2]], agent_array[i->connections[3]],
    	                                       agent_array[i->connections[4]], agent_array[i->connections[5]],
    	                                       agent_array[i->connections[6]], agent_array[i->connections[7]]);
    	                   }
                        }
    	            }
                    if (condition==-1)
                   {
                    
                        if (i->minority != 1) 
                       {//minor seeking
                           //cout<<i->id<<" id \n";
                      
                           if(mode==1)
                            {
                                
                                method='m';
                               i->agent_swap_con(agent_array,*i,
                                               agent_array[i->connections[0]],agent_array[i->connections[1]],
                                               agent_array[i->connections[2]], agent_array[i->connections[3]],
                                               agent_array[i->connections[4]], agent_array[i->connections[5]],
                                               agent_array[i->connections[6]], agent_array[i->connections[7]]);
                           }
                            if(mode==-1)
                            {
                            
                                method='c';
                               i->agent_minority_swap(i->id,*i,
                                               agent_array[i->connections[0]],agent_array[i->connections[1]],
                                               agent_array[i->connections[2]], agent_array[i->connections[3]],
                                               agent_array[i->connections[4]], agent_array[i->connections[5]],
                                               agent_array[i->connections[6]], agent_array[i->connections[7]]);
                           }
                        }
                    }
    	        }
        	}
        }
        if(argc>=6 && prob!=-1)
        {//if argv>=6 and probability is given agents will start swapping species type.
        	//for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i) 
        		//i->swap_species(*i,prob);
    	}
		
        //memset(::matrix,0,sizeof(::matrix));
        //agent_array[99].matrix_print(agent_array);
	
	/* 
	 *
	 *
	 * start of end round data collection
	 *
	 *
	 */
    int mincount=0;
    for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i)
    {
        if(i->minority==1)
        mincount++;
    }
    minoritycount[rounds] = mincount;
        double samespecies=0;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); ++i)
        {
            for (int j = 0; j < i->connections.size(); ++j)
               {
                  if(i->species==agent_array[i->connections[j]].species) samespecies++;
               }   
        }
        same[rounds]=(samespecies/100);
        diff[rounds]=8-(samespecies/100);
	if(rounds>0)
	{
        	for (int i=0; i<::agentcount;++i)
        		{// number of unqiue scores
        			elts[i]=agent_array[i].score;
        		}
        	std::sort(elts.begin(),elts.end());
        	auto ip=std::unique(elts.begin(),elts.end());
        	elts.erase(ip,elts.end());
        	uniquesize[rounds]=elts.size();

        max=0;
        for (int i = 0; i < ::agentcount; ++i) 
        {
            //cout<<agent_array[i].score<<endl;
            if (max < agent_array[i].score) 
            {
                max = agent_array[i].score;
            }

        }
        percuni=0;
        
        for (int i = 0; i < 100; ++i)
        {// percent of agents with max score
            if(max==agent_array[i].score)
            {
                percuni++;
                //cout<<percuni<<" ";
            }

        }
        percentuni[rounds]=(percuni);
    	}
        //cout << minoritycount[rounds] << " \033[1;34mminority count for round\033[0m " << rounds << "\n";
        maxscore[rounds] = max;
        maxround[rounds] = rounds;
        avgscore = 0.0;
       
        for (int i = 0; i < ::agentcount; ++i) 
        {
            avgscore += agent_array[i].score;
        }

        avgscores[rounds] = (avgscore / (::agentcount));
        int eqflag=0;
        //output_connections(inksp,agent_array,rounds);
        //output_scores(inksp,agent_array,rounds);
	/*
	 *
	 *
	 * end of data collection
	 *
	 *
	 *
	 */
    	if((uniquesize[rounds]==1) || rounds>=70)
    	{// checks for break conditions
    		//cout<<fabs((avgscore/100.0)-eq)<<"fabs"<<endl;
    		eqflag=1;
    	}
        
    	if(eqflag==1){break;}
    	
    }
    //for (int i = 0; i < rounds; i++) {
        //cout << maxscore[i] << " \033[1;31mmax score for round\033[0m " << i << endl;
        //cout << avgscores[i] << " \033[1;32mavg score for round\033[0m " << i << "\n";
    
    //
    output_round(inksp,rounds,maxround,maxscore,avgscores,minoritycount,uniquesize,percentuni,searchm,condition,same,diff,method);
	}
    return 0;
}
