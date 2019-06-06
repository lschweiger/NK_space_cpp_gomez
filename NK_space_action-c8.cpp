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
int matrix[100][100]={};
std::uniform_real_distribution<double> uni(0.0, .0005);
std::random_device rd;

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
    vector<char> AB(::agentcount);
    for (int i = 0; i < ::agentcount; ++i) {
        if (i < 50) AB[i] = 'A';
        if (i >= 50) AB[i] = 'B';
    }
    //int acount=0;
    //int bcount=0;
    int temp = 0;
    for (int i = 0; i < ::agentcount; ++i) {
        temp = rand() % (::agentcount - i);
        types[i] = AB[temp];
        AB.erase(AB.begin() + temp);
        //if(types[i]=='A') acount++;
        //if(types[i]=='B') bcount++;
        //cout<<types[i]<<endl;
    }
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

    void agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
        double temscore = input_agent.score;
        string temstring = input_agent.binarystring;
        // minority calc and flag
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
    void agent_explore_A(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 3);
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

    void agent_explore_B(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(4, 7);
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

    void agent_explore_C(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(8, 11);
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

    void agent_explore_D(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(12, 15);
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

    void agent_explore_E(Agent &input_agent, vector<string> &istring, vector<double> &val){
        std::uniform_int_distribution<> randm(16, 19);
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

    void agent_explore(Agent &input_agent, vector<string> &istring, vector<double> &val) {
        // if agent with flag -1 will explore

        if(input_agent.species=='A') agent_explore_A(input_agent,istring,val);
        if(input_agent.species=='B') agent_explore_B(input_agent,istring,val);
        if(input_agent.species=='C') agent_explore_C(input_agent,istring,val);
        if(input_agent.species=='D') agent_explore_D(input_agent,istring,val);
        if(input_agent.species=='E') agent_explore_E(input_agent,istring,val);
        
    };

    void agent_minority_swap(int num,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
    	// swaps agent that is in minority; by going one connection through i.e agent 98 will have the potential to get agents 90 through 6's connections
    	// will randomly only get 8 from the 17.
        
            srand(num);
            vector<int> temp(64);
            vector<int> newcons(8);
            int rdm=0;
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
            sort(temp.begin(), temp.end());
            auto last = std::unique(temp.begin(), temp.end());
            temp.erase(last, temp.end());
            // cout << input_agent.id << " id " << endl;
            for (int i =0; i<8;i++) {
               // cout<<i<<endl;
                std::uniform_int_distribution<> rands(0, (temp.size()-i-1));
       			std::random_device rdm;
      			int random = rands(rdm);
      			
      			while(temp[random]==input_agent.id){// prevents the agent from getting itself as possible connections
      				std::uniform_int_distribution<> rands(0, (temp.size()-i-1));
      				random = rands(rdm);
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
    void connection_swap(Agent &input_agent){// set connections equal to temp connection and clears, resize temp for future uses
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
    return temp;
    };


    void agent_swap_con(vector<Agent> Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
        matrix_fill_before(input_agent.id,input_agent.connections);
        vector<int> new_cons;
        cout<<input_agent.id<<" here \n";
        for(int i=0;new_cons.size()<8;i++)
        {

            //vector<int> pool=pool_con(input_agent,a,b,c,d,e,f,g,h);
            vector<int> old_cons=input_agent.connections;
            vector<int> potential;
            bool found=false;
            std::uniform_int_distribution<> Brands(0, old_cons.size()-1);
            std::random_device Brdm;
            std::uniform_int_distribution<> Crands(0, old_cons.size()-1);
            std::random_device Crdm;
            int B=old_cons[i];
            int C=old_cons[Crands(Crdm)];
            while(C==B&&found==true) {// check if C==B are equal and that C is not connected to B
                for (auto & elem : Agents[B].connections)
                    {
                    if (elem == C)
                        {
                            found = true;
                            break;
                        }
                    }
                C=old_cons[Crands(Crdm)];
            }
            vector<int> C_cons=Agents[C].connections; // assigns the picked agent's connections to C_cons and filters and removes common elements from C_con and potential
            std::set_difference(C_cons.begin(), C_cons.end(), old_cons.begin(), old_cons.end(), std::inserter(potential, potential.begin()));
            potential.erase(std::remove(potential.begin(), potential.end(), input_agent.id), potential.end());
            //cout<<"potential"<<" size "<<potential.size()<<endl;
            /*for (int i = 0; i < potential.size(); ++i)
            {
               cout<<potential[i]<<"\t";
            }
            cout<<endl;
            */
            std::uniform_int_distribution<> Drands(0, potential.size()-1);
            std::random_device Drdm;
            int D=Drands(Drdm);
            //cout<<"random pick"<<D<<" value of D in potential "<<potential[D]<<endl;

            new_cons.push_back(potential[D]); //adds new connection to new_cons and checks that it is unique
            sort(new_cons.begin(), new_cons.end());
            auto last1 = std::unique(new_cons.begin(), new_cons.end());
            new_cons.erase(last1, new_cons.end());

        }
        /*cout<<" new cons"<<endl;
        for (int i = 0; i < new_cons.size(); ++i)
        {
            cout<<new_cons[i]<<"\t";
        }
        cout<<endl;
        cout<<" size of new "<<new_cons.size()<<endl;
        */
        cout<<"info \n";
            matrix_fill_after(input_agent.id,new_cons);
            if (input_agent.id==0||input_agent.id==1||input_agent.id==2||input_agent.id==84||input_agent.id==99)
            {
                matrix_print();
            }
            cout<<"here\n";
            for (int i = 0; i < new_cons.size(); ++i)
            {// assigns new connection to agent's temp and call connection swap after.
                input_agent.tempconnections[i]=new_cons[i];
            }
            //connection_swap(input_agent);
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
    void matrix_print(void)
    {//prints and counts new connections,
        for (int i = 0; i < 100; ++i)
        {
            int count=0;
            for (int j = 0; j < 100; ++j)
            {
                if (::matrix[i][j]==5) count++;
                cout<<::matrix[i][j];

            }
            cout<<" num "<<count<<endl;
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

void output_round(int NKspace,int round,vector<int> rounds,vector<double> scr,vector<double> ag,vector<double> mc,vector<int> us,vector<double> pu){
	std::ofstream out("8c-NK_space_"+to_string(NKspace)+".txt");
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
    int start;
    convert1 >> start;
    int end;
    convert2 >> end;
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
        ::matrix[100][100]={};
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

    for (rounds; rounds < 1; rounds++) {
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
        mcount = 0;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {

            i->agent_exploit(*i, agent_array[i->connections[0]], agent_array[i->connections[1]],
                            agent_array[i->connections[2]], agent_array[i->connections[3]],
                            agent_array[i->connections[4]], agent_array[i->connections[5]],
                            agent_array[i->connections[6]], agent_array[i->connections[7]]);
            //cout<<i->id<<" \033[1;31mid\033[0m "<<i->species<<" \033[1;32mspecies\033[0m "<<"\n";
            //cout<<i->minority<<" minority"<<" \033[1;34mnew_string\033[0m "<<endl;
            if (i->minority == 1) {
                mcount++;
            }
        }
        /*output_connections(agent_array,rounds);*/
        mcount = mcount / 100;
        //cout<<mcount<<endl;
        minoritycount[rounds] = mcount;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            i->agent_swap_info(*i);
            //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
            //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
        }

        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            if (i->flag == -1) {
                i->agent_explore(*i, NKspacevals, NKspacescore);
                //agent_array[i].agentexplore(agent_array[i],NKspace);
                //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
                //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
            }
        }
        //swap_agents(agent_array);
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            //if (i->minority == 1) {
                cout<<i->id<<" id \n";
                //if(i->id==0)i->connections={(1+i->id)%(100),(3+i->id)%(100),(5+i->id)%(100),(7+i->id)%(100),(11+i->id)%(100),(13+i->id)%(100),(17+i->id)%(100),(19+i->id)%(100)};
                i->agent_swap_con(agent_array,*i,
                                       agent_array[i->connections[0]],agent_array[i->connections[1]],
                                       agent_array[i->connections[2]], agent_array[i->connections[3]],
                                       agent_array[i->connections[4]], agent_array[i->connections[5]],
                                       agent_array[i->connections[6]], agent_array[i->connections[7]]);
                /*i->agent_minority_swap(i->id,*i,
                                       agent_array[i->connections[0]],agent_array[i->connections[1]],
                                       agent_array[i->connections[2]], agent_array[i->connections[3]],
                                       agent_array[i->connections[4]], agent_array[i->connections[5]],
                                       agent_array[i->connections[6]], agent_array[i->connections[7]]);
                //cout<<i->connections[0]<<" ";
                //cout<<i->connections[1]<<" ";
                //cout<<i->connections[2]<<" ";
                //cout<<i->connections[3]<<" ";
                //cout<<i->connections[4]<<" ";
                //cout<<i->connections[5]<<" ";
                //cout<<i->connections[6]<<" ";
                //cout<<i->connections[7]<<endl;
            }*/
            //}
        }
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            i->connection_swap(*i);
        }
        memset(::matrix,0,sizeof(::matrix));
        for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 100; ++j)
            {
                ::matrix[i][j]=0;
            }
        }
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
    output_round(NKspace_num,rounds,maxround,maxscore,avgscores,minoritycount,uniquesize,percentuni);
}
    return 0;
}