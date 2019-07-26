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
#include <stdio.h>
#include <cstring>

//#include <thread>
int n = pow(2, 20);
const int nbgh = 100;
const int agentcount = 100;
int matrix[1][1]={};

void open_space_scores(int j, vector<double> &input_vec) {
    std::fstream in_scores;
    in_scores.open("NK_space_scores_" + to_string(j) + ".txt");
    for (int i = 0; i < ::n; ++i) {
        double i_score;
        in_scores >> i_score;
        //cout<<i_score<<"test"<<'\n';
        input_vec[i] = i_score;
        //cout<<input_vec[i]<<'\n';
    }
    in_scores.close();
};

void open_space_string(vector <string> &string_vec) {
    std::fstream strings;
    strings.open("NK_space_strings.txt");
    for (int i = 0; i < ::n; ++i) {
        string i_str;
        strings >> i_str;
        string_vec[i] = i_str;
    }
    strings.close();
};

void AB_list(vector<char> &types){//creates list of A and B and randomly assigns one to an agent at the beginning of every new NK_Space
    std::seed_seq seed{std::time(0)};
    std::default_random_engine generator(seed);
    std::bernoulli_distribution distribution(0.5);

    int acount=0;
    int bcount=0;
    int temp = 0;
    std::vector<char> sp={'A','A','B','B','B','A','A','B','A','B','B','A','A','A','B','B','B','B','A','A','B','B','A','B','B','A','A','B','B','B','B','B','B','B','A','A','A','B','B','B','A','A','A','B','B','B','A','A','B','B','B','B','A','A','B','B','A','B','B','A','B','A','B','A','A','B','B','A','B','A','B','A','A','B','A','B','B','A','A','B','B','A','B','B','A','B','A','B','B','B','B','A','A','A','A','B','B','A','A','B'};
    /*
    for (int i = 0; i < ::agentcount; ++i) {
        temp=distribution(generator);
        if(temp==1) types[i]='A';
        if(temp==0) types[i]='B';
        if(types[i]=='A') acount++;
        if(types[i]=='B') bcount++;
        cout<<types[i];

        distribution.reset();
    }
    */
    for (int i = 0; i < sp.size(); ++i)
    {
        types[i]=sp[i];
                if(types[i]=='A') acount++;
        if(types[i]=='B') bcount++;
    }
    //cout<<" ";
    //cout<<acount<<" "<<bcount<<endl;
    
};
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
};
void AB_list_deseg(vector<char> &types){//creates list of A and B and randomly assigns one to an agent at the beginning of every new NK_Space
    vector<char> AB(::agentcount);
    for (int i = 0; i < ::agentcount; ++i) {
        if (i%2==0) types[i] = 'A';
        if (i%2==1) types[i] = 'B';
    }
};

void AB_list_seg(vector<char> &types){//creates list of A and B and randomly assigns one to an agent at the beginning of every new NK_Space
    vector<char> AB(::agentcount);
    for (int i = 0; i < ::agentcount; ++i) {
        if (i < 50) types[i] = 'A';
        if (i >= 50) types[i] = 'B';
    }
};



class Agent // class for agents; includes variable and functions.
{
public:
    char species;            	// agent species type
    int minority;            	// is agent in minority by species compared to it connections
    int id;
    int flag = 0;				//flag is use in swap and exploit
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
        if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;
        if (input_agent.species != e.species) minorcalc++;
        if (input_agent.species != f.species) minorcalc++;
        if (input_agent.species != g.species) minorcalc++;
        if (input_agent.species != h.species) minorcalc++;
        if (minorcalc >4) input_agent.minority = 1;
    };
    void agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
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
        if (minorcalc > 4) input_agent.minority = 1;
        */
        agent_minority_status(input_agent,a,b,c,d,e,f,g,h);
        //cout<<"minority status "<<input_agent.minority<<endl;
        //score update
        if (temscore < a.score) {
            temscore = a.score;
            temstring = a.binarystring;
            input_agent.flag = 1;//
        }
        if (temscore < b.score) {
            temscore = b.score;
            temstring = b.binarystring;
            input_agent.flag = 1;
        }
        if (temscore < c.score) {
            temscore = c.score;
            temstring = c.binarystring;
            input_agent.flag = 1;
        }
        if (temscore < d.score) {
            temscore = d.score;
            temstring = d.binarystring;
            input_agent.flag = 1;
        }
        if (temscore < e.score) {
            temscore = e.score;
            temstring = e.binarystring;
            input_agent.flag = 1;
        }
        if (temscore < f.score) {
            temscore = f.score;
            temstring = f.binarystring;
            input_agent.flag = 1;
        }
        if (temscore < g.score) {
            temscore = g.score;
            temstring = g.binarystring;
            input_agent.flag = 1;
        }
        if (temscore < h.score) {
            temscore = h.score;
            temstring = h.binarystring;
            input_agent.flag = 1;
        }
        input_agent.tempscore = temscore;
        input_agent.tempstring = temstring;
        if (input_agent.flag == 0) input_agent.flag = -1;
    };

    void agent_swap_info(Agent &input_agent) {
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
        int random = randm(rdm);
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
        int random = randm(rdm)*2;
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
        int random = randm(rdm)*2+1;
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

    void agent_explore_B_10(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(10, 19);
        std::random_device rdm;
        int random = randm(rdm);
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

    void agent_explore_A_swap(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 18);
        std::random_device rdm;
        int random = randm(rdm);
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
        int random = randm(rdm);
        int random2 = randm2(rdm);
        while(abs(random2-random)<=2) random2 = randm2(rdm); 
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
        if(input_agent.species=='B' && search==1) agent_explore_A_even(input_agent,istring,val);
        if(input_agent.species=='B' && search==1) agent_explore_B_odd(input_agent,istring,val);
        if(input_agent.species=='B' && search==2) agent_explore_A_swap(input_agent,istring,val);
        if(input_agent.species=='B' && search==2) agent_explore_B_swap(input_agent,istring,val);
        if(input_agent.species=='B' && search==3) agent_explore_A_flip(input_agent,istring,val);
        if(input_agent.species=='B' && search==3) agent_explore_B_flip(input_agent,istring,val);
        
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


    void agent_swap_con(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
        //matrix_fill_before(input_agent.id,input_agent.connections);
        std::default_random_engine generator (2);
        int A=input_agent.id; //input agent named A 
        std::uniform_int_distribution<> selected(0, input_agent.connections.size()-1); //random selction for B
        std::random_device Srdm;
        vector<int> pool=pool_con(input_agent,a,b,c,d,e,f,g,h); //set of all 1st degree connected agents that we pick C* from
        vector<int> old_cons=input_agent.connections;
        vector<int> C_potential;
        vector<int> D_potential;
        int B=old_cons[selected(Srdm)];
        for(int i=0;i<pool.size();i++){ //find and only adds agents for C* from the pool that are not connected to A
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
        int C=C_potential[c_sel(Crdm)];
        vector<int> C_cons=Agents[C].connections;
        //cout<<C_potential.size()<<" test 0"<<endl;
        for(int i=0;i<C_cons.size();i++){ // find and only adds agents for D* from the C* that are not connected to B
            bool found = false;
            for (auto & elem : Agents[C_cons[i]].connections)
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
            for(int i=0;i<C_cons.size();i++){ // find and only adds agents for D* from the C* that are not connected to B
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
        int D=D_potential[d_sel(Drdm)];
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
            for (int i = 0; i < filler.size(); i++)
            {
                ::matrix[id][filler[i]]=1;
                ::matrix[filler[i]][id]=1;
            }
        };

    void matrix_fill_after(int id,vector<int> filler)
        {// fill matrix with new connection
            for (int i = 0; i < filler.size(); i++)
            {
                ::matrix[id][filler[i]]=5;
                ::matrix[filler[i]][id]=5;
            }
        };
    void matrix_print(vector<Agent> agent_array)
    {//prints and counts new connections,
        for(int i=0;i<100;i++)
        {
            for (int j = 0; j < 100; j++)
            {
                for (int k = 0; k < agent_array[i].connections.size(); k++)
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
};


void output_connections(vector<Agent> &Agents,int rounds){ //outputs all agents connections as a csv file; includes minority status
	std::ofstream out;
	out.open("agent connections "+to_string(rounds)+".txt");
	out<<"Agent id #"<<","<<"connection 0"<<","<<"connection 1"<<","<<"connection 2"<<","<<"connection 3"<<","<<"connection 4"<<","<<"connection 5"<<","<<"connection 6"<<","<<"connection 7"<<","<<"minority status"<<"\n";
	for (vector<Agent>::iterator i = Agents.begin(); i != Agents.end(); i++)
	{
		out<<i->id<<","<<i->connections[0]<<","<<i->connections[1]<<","<<i->connections[2]<<","<<i->connections[3]<<","<<i->connections[4]<<","<<i->connections[5]<<","<<i->connections[6]<<","<<i->connections[7];
		if(i->minority==1) 
			{out<<","<<"M"<<"\n";} 
		else
			{out<<"\n";}
	}
};

void output_round(int NKspace,int round,vector<int> rounds,vector<double> scr,vector<double> ag,vector<double> mc,vector<int> us,vector<double> pu,int search){
	std::ofstream out("8c-NK_space_"+to_string(NKspace)+"_SH_"+to_string(search)+".txt");
	out<<"round,"<<"max score,"<<"avg score,"<<"Number of unique solutions,"<<"percent with max score,"<<"minority count"<<"\n";
	for (int i=0;i<round; i++){
		out<<std::setprecision(15)<<rounds[i]<<","<<scr[i]<<","<<ag[i]<<","<<us[i]<<","<<pu[i]<<","<<std::setprecision(15)<<mc[i]<<"\n";
	}
};
void swapper(vector<Agent> &v,vector<int> &a,vector<int> &b){
        char s;
        string string;
        double binary;
        for(int i=0;i<a.size();i++){
            s=v[a[i]].species;
            string=v[a[i]].binarystring;
            binary=v[a[i]].score;
            v[a[i]].species=v[b[i]].species;
            v[a[i]].binarystring=v[b[i]].binarystring;
            v[a[i]].score=v[b[i]].score;
            v[b[i]].species=s;
            v[b[i]].binarystring=string;
            v[b[i]].score=binary;
        }
    };
    void swap_agents(vector<Agent> &v){
        vector<int> minor;
        vector<int> permute;
        for (int i = 0; i < 100; ++i)
        {
            if(v[i].minority==1) {
                minor.push_back(v[i].id);
                v[i].minority=0; 
            }
        }
        permute=minor;
        std::random_shuffle ( permute.begin(), permute.end() );
        swapper(v,minor,permute);

    };
    

int main(int argc, char *argv[]) { 
    std::stringstream convert1(argv[1]);
    std::stringstream convert2(argv[2]);
    std::string convert3(argv[3]);
    std::string convert4(argv[4]);
    int start;
    convert1 >> start;
    int end;
    convert2 >> end;
    int searchm;
    if(convert3=="-S" && atoi(argv[4])<=3){
        searchm=atoi(argv[4]);
    }
    else{
        searchm=0;
    }
    int NKspace_num=start;
    vector <Agent> agent_array(::agentcount);
    cout.setf(std::ios::fixed);
    cout.setf(std::ios::showpoint);
    cout.precision(15);
    vector <string> NKspacevals(::n);
    vector<double> NKspacescore(::n);
    open_space_scores(NKspace_num, NKspacescore);
    open_space_string(NKspacevals);

    double max = 0.0;
    double eq=0.0;
    double avgscore = 0.0;
    int unisize=0;
    int percuni=0;
    vector<double> maxscore(100);
    vector<int> maxround(100);
    vector<double> minoritycount(100);
    vector<double> avgscores(100);
    vector<char> type(::agentcount);
    vector<int> uniquesize(100);
    vector<double> percentuni(100);
    AB_list(type);

	int rounds = 0;
    int nums = 0;
    double mcount;
for(NKspace_num;NKspace_num<end;NKspace_num++){
	srand(NKspace_num+1);
    cout<<NKspace_num<<endl;
	if(NKspace_num>0){
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
		maxscore.resize(100);
		maxround.resize(100);
		minoritycount.resize(100);
		avgscores.resize(100);
		type.resize(100);
        uniquesize.resize(100);
        percentuni.resize(100);
        percuni=0;
        unisize=0;
		rounds = 0;
    	nums = 0;
    	mcount=0;
    	eq=0.0;
    	max=0.0;
        //::matrix[100][100]={};
    }
	AB_list(type);
	open_space_scores(NKspace_num, NKspacescore);
    for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {

        i->agent_connections(nums, *i);
        i->species = type[nums];
        //cout<<i->id<<endl;
        //cout<<i->species<<endl;
        i->agent_change(rand() % (::n), *i, NKspacevals, NKspacescore);

        nums++;
    }

    for (rounds; rounds < 100; rounds++) {
        unisize=1;
        string binstr=agent_array[0].binarystring;
        for (int i=1; i<::agentcount;i++){
            if(binstr!=agent_array[i].binarystring){
                unisize++;
                binstr=agent_array[i].binarystring;
            }
            //if(i==99 && unisize==0) unisize=1;
        }
        uniquesize[rounds]=unisize;
        max=0;
        for (int i = 0; i < ::agentcount; i++) {
            //cout<<agent_array[i].score<<endl;
            if (max < agent_array[i].score) {
                max = agent_array[i].score;
            }

        }
        cout<<"round #"<<rounds<<endl;
        percuni=0;
        for (int i = 0; i < 100; ++i)
        {
            if(max==agent_array[i].score){
                percuni++;
                //cout<<percuni<<" ";
            }

        }
        percentuni[rounds]=(percuni);

        //cout<<percentuni[rounds]<<endl;
        mcount = 0.0;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            i->agent_minority_status(*i, agent_array[i->connections[0]], agent_array[i->connections[1]],
                            agent_array[i->connections[2]], agent_array[i->connections[3]],
                            agent_array[i->connections[4]], agent_array[i->connections[5]],
                            agent_array[i->connections[6]], agent_array[i->connections[7]]);
            if (i->minority == 1) {
                mcount++;
                i->minority==0;
            }
        }
        mcount = mcount / 100.0;
        //cout<<mcount<<endl;
        minoritycount[rounds] = mcount;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {

            i->agent_exploit(*i, agent_array[i->connections[0]], agent_array[i->connections[1]],
                            agent_array[i->connections[2]], agent_array[i->connections[3]],
                            agent_array[i->connections[4]], agent_array[i->connections[5]],
                            agent_array[i->connections[6]], agent_array[i->connections[7]]);
            //cout<<i->id<<" \033[1;31mid\033[0m "<<i->species<<" \033[1;32mspecies\033[0m "<<"\n";
            //cout<<i->minority<<" minority"<<" \033[1;34mnew_string\033[0m "<<endl;
            
        }
        /*output_connections(agent_array,rounds);*/

        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            i->agent_swap_info(*i);
            //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
            //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
        }

        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            if (i->flag == -1) {
                i->agent_explore(*i, NKspacevals, NKspacescore,searchm);
                //agent_array[i].agentexplore(agent_array[i],NKspace);
                //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
                //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
            }
        }
        //swap_agents(agent_array);
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            if (i->minority == 1) {
               //cout<<i->id<<" id \n";
                //if(i->id==0)i->connections={(1+i->id)%(100),(3+i->id)%(100),(5+i->id)%(100),(7+i->id)%(100),(11+i->id)%(100),(13+i->id)%(100),(17+i->id)%(100),(19+i->id)%(100)};
                i->agent_swap_con(agent_array,*i,
                                       agent_array[i->connections[0]],agent_array[i->connections[1]],
                                       agent_array[i->connections[2]], agent_array[i->connections[3]],
                                       agent_array[i->connections[4]], agent_array[i->connections[5]],
                                       agent_array[i->connections[6]], agent_array[i->connections[7]]);
            }
        }
        //agent_array[99].matrix_print(agent_array);

        max=0;
        for (int i = 0; i < ::agentcount; i++) {
            //cout<<agent_array[i].score<<endl;
            if (max < agent_array[i].score) {
                max = agent_array[i].score;
            }

        }

        maxscore[rounds] = max;
        maxround[rounds] = rounds;
		eq=maxscore[rounds];
        avgscore = 0;
        for (int i = 0; i < ::agentcount; i++) {
            avgscore += agent_array[i].score;
        }

        avgscores[rounds] = (avgscore / (::agentcount));
        int eqflag=0;
    	if((uniquesize[rounds]==1 &&uniquesize[rounds-1]==1) || rounds>=50){
    		//cout<<fabs((avgscore/100.0)-eq)<<"fabs"<<endl;
    		eqflag=1;
    	}
        
    	if(eqflag==1){cout<<"ending"<<endl;break;}
    }
    /*for (int i = 0; i < rounds; i++) {
        cout << maxscore[i] << " \033[1;31mmax score for round\033[0m " << i << endl;
        cout << avgscores[i] << " \033[1;32mavg score for round\033[0m " << i << "\n";
        cout << minoritycount[i] << " \033[1;34mminority count for round\033[0m " << i << "\n";
    }*/
    output_round(NKspace_num,rounds,maxround,maxscore,avgscores,minoritycount,uniquesize,percentuni,searchm);
}
    return 0;
}