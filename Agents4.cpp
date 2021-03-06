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
#include <iomanip> //set precision
#include <bitset> //binary to numbers
#include <cmath> // pow function
#include <random> // probability distribution
#include <algorithm>
#include <sstream>
#include <cstring>

#include "Agents4.h"
int n = pow(2, 15);
const int nbgh = 20;
//const int agentcount = 100;
int matrix[100][100]={};   

    void Agent::agent_connections(int num, Agent &input_agent) {
        //if(num>=20) return;
        int size=4;
        int esize=0;
        input_agent.id = num; //assigns id from 0 to 99,
        //cout<<input_agent.id<<" id"<<endl;
        //sets the default connections to -2,-1,1,2 of the current agents id, includes a check for negatives id
        for(int i=size/2;i>0;--i)
        {
            if((num-i)<0) input_agent.connections[(size/2-i)]=(num-i)%(::nbgh)+(nbgh);
            else input_agent.connections[size/2-i]=(num-i)%(::nbgh);
        }
        if(size%2==0) esize=size/2;
        else esize=(size/2)+1;
        for(int i=0; i<esize;++i)
        {
            if(size%2==0)
                input_agent.connections[size/2+i]=(num+i+1)%(::nbgh);
            else
                input_agent.connections[size/2+i]=(num+i+1)%(::nbgh);
        }
        std::sort(input_agent.connections.begin(),input_agent.connections.end());
    }

    void Agent::agent_change(int num, Agent &input_agent, vector<string> &istring, vector<double> &val) {
        // changes sets score and binarystring by random value.
        input_agent.score = val[num];
        //cout<<val[num]<<" vec test"<<endl;
        //cout<<input_agent.score<<" test" <<endl;
        input_agent.binarystring = istring[num];
    }

    void Agent::agent_minority_status(Agent &input_agent, Agent a, Agent b, Agent c, Agent d){
        int minorcalc = 0;
        int samecalc = 0;
        if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;

        if (input_agent.species == a.species) samecalc++;
        if (input_agent.species == b.species) samecalc++;
        if (input_agent.species == c.species) samecalc++;
        if (input_agent.species == d.species) samecalc++;


        //cout<<minorcalc<<"out"<<endl;
        if ((samecalc+1)<=2)
        	{
        		//cout<<minorcalc<<"in"<<endl;
        		//cout<<(samecalc+1)<<endl;
        		//cout<<input_agent.id<<" "<<input_agent.species<<" "<<a.species<<" "<<b.species<<" "<<c.species<<" "<<d.species<<" "<<e.species<<" "<<f.species<<" "<<g.species<<" "<<h.species<<endl;
        		input_agent.minority = 1; 
        	//cout<<input_agent.id<<" minor id "<<" funct "<<input_agent.species<<endl;
        	}
		else{input_agent.minority=0;}
    
    }

    void Agent::morph_agent_exp(Agent &input,Agent target,double diffscore){
        std::random_device rd;
        std::binomial_distribution<int> binom (1,(1-std::exp(-diffscore*3.46)));
        int bi=binom(rd);
    if (bi==1)
        {
            input.species=target.species;
        }
    
    }

    void Agent::morph_agent_100(Agent &input,Agent target,double prob){
        std::random_device rd;
        std::binomial_distribution<int> binom (1,prob);
        int bi=binom(rd);
    if (bi==1)
        {
            input.species=target.species;
        }
    
    }


    void Agent::agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,double prob,char method) {
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
        
        agent_minority_status(input_agent,a,b,c,d);
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
            }
            if(method=='z')
            {
            if(prob==-1){
                //cout<<"morphing\n";
            
            morph_agent_exp(input_agent,target_agent,diffscore);
            }
            else
                //cout<<"morphing raw\n";
            morph_agent_100(input_agent,target_agent,prob);
            }
            //char new_test=input_agent.species;
            //cout<<input_agent.id<<"\t"<<new_test<<endl;
        }
        if (input_agent.flag == 0) input_agent.flag = -1;
    }

    void Agent::agent_swap_interal_info(Agent &input_agent) {
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
    }
    void Agent::agent_explore_A_10(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 6);
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
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }

    void Agent::agent_explore_B_10(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(7, 14);
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
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }

    void Agent::agent_explore_A_even(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(1, 7);
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
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }

    void Agent::agent_explore_B_odd(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 6);
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
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }

    void Agent::agent_explore_A_swap(Agent &input_agent,  vector<double> &val){
        std::uniform_int_distribution<> randm(0, 14);
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
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }
    void Agent::agent_explore_B_swap(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 14);
        std::uniform_int_distribution<> randm2(0, 14);
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
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }

    void Agent::agent_explore_A_flip(Agent &input_agent, vector<double> &val){
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            for(int i=0;i<7;i++){
                if (temstring[i] == '1')
                {
                    temstring[i] = '0';
                } 
                else {
                    temstring[i] = '1';
                }
            }
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }

    void Agent::agent_explore_B_flip(Agent &input_agent, vector<double> &val){
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            for(int i=7;i<14;i++){
            
                if (temstring[i] == '1')
                {
                    temstring[i] = '0';
                } 
                else {
                    temstring[i] = '1';
                }
            }
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.binarystring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
    }
    

    void Agent::agent_explore(Agent &input_agent, vector<double> &val,int search) {
        // if agent with flag -1 will explore
        //cout<<search<<endl;
        if(input_agent.species=='A' && search==0) agent_explore_A_10(input_agent,val);
        if(input_agent.species=='B' && search==0) agent_explore_B_10(input_agent,val);
        if(input_agent.species=='A' && search==1) agent_explore_A_even(input_agent,val);
        if(input_agent.species=='B' && search==1) agent_explore_B_odd(input_agent,val);
        if(input_agent.species=='A' && search==2) agent_explore_A_swap(input_agent,val);
        if(input_agent.species=='B' && search==2) agent_explore_B_swap(input_agent,val);
        if(input_agent.species=='A' && search==3) agent_explore_A_flip(input_agent,val);
        if(input_agent.species=='B' && search==3) agent_explore_B_flip(input_agent,val);
        
    }

    std::vector<int> pool_con(Agent input_agent,Agent a, Agent b, Agent c, Agent d){
        vector<int> temp(16);
        for(int i=0;i<4;i++){// fills temp with all possible connections including duplicates, will also fill temp up simultaneously from the other 8 agents
            temp[i]=a.connections[i];
                //cout<<a.id<<"id "<<a.connections[i]<<" a con"<<endl;
            temp[4+i]=b.connections[i];
                //cout<<b.id<<"id "<<b.connections[i]<<" b con"<<endl;
            temp[8+i]=c.connections[i];
                //cout<<c.id<<"id "<<c.connections[i]<<" c con"<<endl;
            temp[12+i]=d.connections[i];
                //cout<<d.id<<"id "<<d.connections[i]<<" d con"<<endl;
        }

            
            //sorts temp and removes duplicate values to create a vector
        temp.erase(std::remove(temp.begin(), temp.end(), input_agent.id), temp.end());
        sort(temp.begin(), temp.end());
        auto last = std::unique(temp.begin(), temp.end());
        temp.erase(last, temp.end());
        temp.resize(temp.size());
    return temp;
    }

    void Agent::connection_swap(Agent &input_agent){// set connections equal to temp connection and clears, resize temp for future uses; for testing only
        input_agent.connections=input_agent.tempconnections;
        input_agent.tempconnections.clear();
        input_agent.tempconnections.resize(4);
    }

    void Agent::agent_minority_swap(int num,Agent &input_agent,Agent a, Agent b, Agent c, Agent d){
        // swaps agent that is in minority; by going through all connections i.e agent 98 will have the potential to get agents 90 through 6's connections
        // will randomly only get 8.
        
            //srand(num);
            int stop=input_agent.connection_replace;
            vector<int> temp=pool_con(input_agent,a,b,c,d);
            vector<int> newcons=input_agent.connections;
            for (int i =0; i<stop;++i) {
               // cout<<i<<endl;
                std::uniform_int_distribution<> rands(0, (temp.size()-1));
                std::random_device rdm;
                std::mt19937 gen(rdm());
                int random = rands(gen);
                rands.reset();
                newcons[i] = temp[random];
                sort(newcons.begin(), newcons.end());
                auto last = std::unique(newcons.begin(), newcons.end());
                newcons.erase(last, newcons.end());
                newcons.resize(newcons.size());
                while(newcons.size()!=4){
                random = rands(gen);
                rands.reset();
                newcons.push_back(temp[random]);
                sort(newcons.begin(), newcons.end());
                auto last = std::unique(newcons.begin(), newcons.end());
                newcons.erase(last, newcons.end());
                newcons.resize(newcons.size());
                cout<<temp.size()<<" here\n";
                }
                /*while(temp[random]==input_agent.id){// prevents the agent from getting itself as possible connections
                    std::uniform_int_distribution<> rands(0, (temp.size()-1));
                    random = rands(gen);
                    rands.reset();
                }*/
                
                //cout<<newcons[i]<<" ";
                temp.erase(temp.begin()+random);
                temp.resize(temp.size());
            }
            //cout<<newcons.size()<<' ';
            //cout<<endl;
            //cout<<"new"<<endl;
            for (int j = 0; j <4 ; ++j) {

                //cout<<newcons[j]<<"new con"<<endl;
                input_agent.tempconnections[j]=newcons[j];
            }
            sort(input_agent.tempconnections.begin(), input_agent.tempconnections.end());
        //when done resets flag back to 0, regardless of status
        connection_swap(input_agent);
        input_agent.minority=0;
        temp.clear();
        temp.resize(0);
    }

    

    


    void Agent::agent_swap_con(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, int loop){ //swaps an agents connection using the adjacency matrix
        //matrix_fill_before(input_agent.id,input_agent.connections);
        //std::default_random_engine generator (2);
        try{
        int A=input_agent.id; //input agent named A 
        //std::uniform_int_distribution<> selected(0, input_agent.connections.size()-1); //random selction for B
        //std::random_device Srdm;
        //std::mt19937 gen(Srdm());
        vector<int> pool=pool_con(input_agent,a,b,c,d); //set of all 1st degree connected agents that we pick C* from
        vector<int> old_cons=input_agent.connections;
        vector<int> C_potential;
        vector<int> D_potential;
        int B=old_cons[loop];
        //cout<<"here\n";
        //cout<<input_agent.id<<endl;
        for(unsigned int i=0;i<pool.size();i++){ //find and only adds agents for C* from the pool that are not connected to A
            bool found = false;
            int loc=0;
            for (auto & elem : Agents[pool[i]].connections)
                if (elem == A)
                {
                    found = true;
                    break;
                }
            if(found==true){++loc; continue;}
            if(found==false)
                {
                    C_potential.push_back(pool[i]);
                }
        }
        if(C_potential.size()==0) throw(static_cast<int>(C_potential.size()));
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
                for(auto & elem : Agents[C_cons[i]].connections)
                {
                    if (elem == B || elem==A)
                    {
                        found = true;
                        break;
                    }
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
        for (int i = 0; i < 4; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[B].id<<endl;
                for (int i = 0; i < 4; ++i)
        {
            cout<<Agents[B].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[C].id<<endl;
                for (int i = 0; i < 4; ++i)
        {
            cout<<Agents[C].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[D].id<<endl;
                for (int i = 0; i < 4; ++i)
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
                for (int i = 0; i < 4; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 4; ++i)
        {
            cout<<Agents[B].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 4; ++i)
        {
            cout<<Agents[C].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 4; ++i)
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
        }
        catch(int size){
            cout<<input_agent.id<<" size is 0\n";
            for (int i = 0; i < 4; ++i)
            {
                cout<<input_agent.connections[i]<<" ";
            }
            cout<<endl;
            return;
        }           
    }
    void Agent::agent_swap_hack(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d){
        for (int i = 0; i < input_agent.connection_replace; ++i)
        {
            agent_swap_con(Agents, input_agent, a, b, c, d,i);
        }
    }
    void Agent::matrix_fill_before(int id,vector<int> filler)
        {//fills matrix with old connections
            for (unsigned int i = 0; i < filler.size(); i++)
            {
                ::matrix[id][filler[i]]=1;
                ::matrix[filler[i]][id]=1;
            }
        }

    void Agent::matrix_fill_after(int id,vector<int> filler)
        {// fill matrix with new connection
            for (unsigned int i = 0; i < filler.size(); i++)
            {
                ::matrix[id][filler[i]]=5;
                ::matrix[filler[i]][id]=5;
            }
        }
    void Agent::matrix_print(vector<Agent> agent_array)
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
    }

	void Agent::swap_species(Agent &input_agent,double& epsilon)
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
    void Agent::agent_exploit_weighted(Agent &input_agent, Agent a, Agent b, Agent c, Agent d) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
        // will mutate agents string and score if they exploit
        double temscore = input_agent.score;
        string temstring = input_agent.binarystring;
        double ascore;
        double bscore;
        double cscore;
        double dscore;

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
        
        Agent::agent_minority_status(input_agent,a,b,c,d);
        //cout<<"minority status "<<input_agent.minority<<endl;
        //score update
        int target_id=-1;
        double multiplier=0.25;
        if(input_agent.species==a.species) ascore=a.score+a.score*multiplier;
        else ascore=a.score-a.score*multiplier;
        if(input_agent.species==b.species) bscore=b.score+b.score*multiplier;
        else bscore=b.score-b.score*multiplier;
        if(input_agent.species==c.species) cscore=c.score+c.score*multiplier;
        else cscore=c.score-c.score*multiplier;
        if(input_agent.species==d.species) dscore=d.score+d.score*multiplier;
        else dscore=d.score-d.score*multiplier;

        if (temscore < ascore) 
        {
            temscore = a.score;
            temstring = a.binarystring;
            input_agent.flag = 1;//
            target_id=0;
        }

        if (temscore < bscore) {
            temscore = b.score;
            temstring = b.binarystring;
            input_agent.flag = 1;
            target_id=1;
        }

        if (temscore < cscore) {
            temscore = c.score;
            temstring = c.binarystring;
            input_agent.flag = 1;
            target_id=2;
        }

        if (temscore < dscore) {
            temscore = d.score;
            temstring = d.binarystring;
            input_agent.flag = 1;
            target_id=3;
        }

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
            }
            //morph_agent(input_agent,target_agent);
            //char new_test=input_agent.species;
            //cout<<input_agent.id<<"\t"<<new_test<<endl;
        }
        if (input_agent.flag == 0) input_agent.flag = -1;
    }
    void Agent::agent_exploit_weighted_inverse(Agent &input_agent, Agent a, Agent b, Agent c, Agent d) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
        // will mutate agents string and score if they exploit
        double temscore = input_agent.score;
        string temstring = input_agent.binarystring;
        double ascore;
        double bscore;
        double cscore;
        double dscore;
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
        
        Agent::agent_minority_status(input_agent,a,b,c,d);
        //cout<<"minority status "<<input_agent.minority<<endl;
        //score update
        int target_id=-1;
        double multiplier=0.25;
        if(input_agent.species!=a.species) ascore=a.score-a.score*multiplier;
        else ascore=a.score+a.score*multiplier;
        if(input_agent.species!=b.species) bscore=b.score-b.score*multiplier;
        else bscore=b.score+b.score*multiplier;
        if(input_agent.species!=c.species) cscore=c.score-c.score*multiplier;
        else cscore=c.score+c.score*multiplier;
        if(input_agent.species!=d.species) dscore=d.score-d.score*multiplier;
        else dscore=d.score+d.score*multiplier;

        if (temscore < ascore) 
        {
            temscore = a.score;
            temstring = a.binarystring;
            input_agent.flag = 1;//
            target_id=0;
        }

        if (temscore < bscore) {
            temscore = b.score;
            temstring = b.binarystring;
            input_agent.flag = 1;
            target_id=1;
        }

        if (temscore < cscore) {
            temscore = c.score;
            temstring = c.binarystring;
            input_agent.flag = 1;
            target_id=2;
        }

        if (temscore < dscore) {
            temscore = d.score;
            temstring = d.binarystring;
            input_agent.flag = 1;
            target_id=3;
        }



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
            }
            //morph_agent(input_agent,target_agent);
            //char new_test=input_agent.species;
            //cout<<input_agent.id<<"\t"<<new_test<<endl;
        }
        if (input_agent.flag == 0) input_agent.flag = -1;
    }