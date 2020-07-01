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

#include "Agents6.h"
int n = (1<<15);
const int agentcounta = 50;
const int connection_number=6;
//const int agentcount = 100;
int matrix[agentcounta][agentcounta]={};   

    void Agent::agent_connections(int num, Agent &input_agent) {
        //if(num>=20) return;
        int size=connection_number;
        int esize=0;
        input_agent.id = num; //assigns id from 0 to 99,
        //cout<<input_agent.id<<" id"<<endl;
        //sets the default connections to -2,-1,1,2 of the current agents id, includes a check for negatives id
        for(int i=size/2;i>0;--i)
        {
            if((num-i)<0) input_agent.connections[(size/2-i)]=(num-i)%(::agentcounta)+(::agentcounta);
            else input_agent.connections[size/2-i]=(num-i)%(::agentcounta);
        }
        if(size%2==0) esize=size/2;
        else esize=(size/2)+1;
        for(int i=0; i<esize;++i)
        {
                input_agent.connections[size/2+i]=(num+i+1)%(::agentcounta);
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

    void Agent::agent_minority_status_symmetric(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f){
        int minorcalc = 0;
        int samecalc = 0;
        if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;
        if (input_agent.species != e.species) minorcalc++;
        if (input_agent.species != f.species) minorcalc++;

        if (input_agent.species == a.species) samecalc++;
        if (input_agent.species == b.species) samecalc++;
        if (input_agent.species == c.species) samecalc++;
        if (input_agent.species == d.species) samecalc++;
        if (input_agent.species == e.species) samecalc++;
        if (input_agent.species == f.species) samecalc++;

        //cout<<minorcalc<<"out"<<endl;
        if ((samecalc+1)<=(1+connection_number/2))
        	{
        	   //cout<<minorcalc<<"in"<<endl;
        	   //cout<<(samecalc+1)<<endl;
        	   //cout<<input_agent.id<<" "<<input_agent.species<<" "<<a.species<<" "<<b.species<<" "<<c.species<<" "<<d.species<<" "<<e.species<<" "<<f.species<<endl;
        	   input_agent.minority = 1; 
        	   //cout<<input_agent.id<<" minor id "<<" funct "<<input_agent.species<<endl;
        	}
		else{input_agent.minority=0;}
    
    }

    void Agent::agent_minority_status_asymmetric(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int Criterion){ //minority calculation for connections method
        int minorcalc = 0;
        int samecalc = 0;
        if (input_agent.species != a.species) minorcalc++;
        if (input_agent.species != b.species) minorcalc++;
        if (input_agent.species != c.species) minorcalc++;
        if (input_agent.species != d.species) minorcalc++;
        if (input_agent.species != e.species) minorcalc++;
        if (input_agent.species != f.species) minorcalc++;

        if (input_agent.species == a.species) samecalc++;
        if (input_agent.species == b.species) samecalc++;
        if (input_agent.species == c.species) samecalc++;
        if (input_agent.species == d.species) samecalc++;
        if (input_agent.species == e.species) samecalc++;
        if (input_agent.species == f.species) samecalc++;

        //cout<<minorcalc<<"out"<<endl;
        if (samecalc<Criterion)
            {
               //cout<<minorcalc<<"in"<<endl;
               //cout<<(samecalc+1)<<endl;
               //cout<<input_agent.id<<" "<<input_agent.species<<" "<<a.species<<" "<<b.species<<" "<<c.species<<" "<<d.species<<" "<<e.species<<" "<<f.species<<endl;
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
        binom.reset();
    }

    void Agent::morph_agent_100(Agent &input,Agent target,double prob){
        std::random_device rd;
        std::binomial_distribution<int> binom (1,prob);
        int bi=binom(rd);
    if (bi==1)
        {
            input.species=target.species;
        }
        binom.reset();
    }


    void Agent::agent_exploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,double prob,char method,int mode, int Criterion) {
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
        
        if(mode==1)agent_minority_status_symmetric(input_agent,a,b,c,d,e,f);
        if(mode==-1)agent_minority_status_asymmetric(input_agent,a,b,c,d,e,f,Criterion);
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
        randm.reset();
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
        randm.reset();
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
        randm.reset();
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
        randm.reset();
    }

    void Agent::agent_explore_A_swap(Agent &input_agent,  vector<double> &val){
        std::uniform_int_distribution<> randm(0, 13);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            char temp=temstring[random];
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
        randm.reset();
    }
    void Agent::agent_explore_B_swap(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 13);
        std::uniform_int_distribution<> randm2(0, 13);
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
            char temp=temstring[random];
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
        randm.reset();
        randm2.reset();
    }

    void Agent::agent_explore_A_flip(Agent &input_agent, vector<double> &val){
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            for(int i=0;i<1;i++){
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
            for(int i=7;i<8;i++){
            
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
    
    void Agent::agent_explore_random_1(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 13);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {

            string temstring = input_agent.binarystring;
            //cout<<"1 old string "<<temstring<<endl;
            temstring[random] ='1';
            //cout<<"\t new string "<<tempstring<<endl;
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            //cout<<"\t new score "<<val[newid]<<endl;
            
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.tempstring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
        randm.reset();
    }

    void Agent::agent_explore_random_0(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 13);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //cout<<random<<endl;
        //checks if flag is really -1
        if (input_agent.flag == -1) {

            string temstring = input_agent.binarystring;
            //cout<<"0 old string "<<temstring<<endl;
            temstring[random] ='0';
            //cout<<"\t new string "<<temstring<<endl;
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.tempstring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
        randm.reset();
    }

    void Agent::agent_explore_flip(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> randm(0, 13);
        std::random_device rdm;
        std::mt19937 gen(rdm());
        int random = randm(gen);
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            string temstring = input_agent.binarystring;
            //cout<<"flip old string "<<temstring<<endl;
                if (temstring[random] == '1')
                {
                    temstring[random] = '0';
                } 
                else {
                    temstring[random] = '1';
                }
            //cout<<"\t new string "<<temstring<<endl;
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.tempstring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
        randm.reset();
    }

    void Agent::agent_explore_shuffle(Agent &input_agent, vector<double> &val){
        std::uniform_int_distribution<> rands(0, 6);
        std::uniform_int_distribution<> rande(7, 13);
        std::random_device rds;
        std::random_device rde;
        std::random_device rdshuf;
        std::mt19937 gens(rds());
        std::mt19937 gene(rde());
        std::mt19937 genshuf(rdshuf());
        int random_start = rands(gens);
        int random_end = rande(gene);
        unsigned int seed=0;
        //checks if flag is really -1
        if (input_agent.flag == -1) {
            //cout<<"shuffle "<<input_agent.binarystring<<endl;
            string temstring = input_agent.binarystring;
            //cout<<"old string "<<temstring<<endl;
            std::shuffle(temstring.begin()+random_start,temstring.begin()+random_end,genshuf);
            //cout<<"\t new string "<<temstring<<endl;
            std::bitset<15> newidbinary = std::bitset<15>(temstring);
            int newid=newidbinary.to_ulong();
            // if new string and score are better assigns them to agent
            if (input_agent.score < val[newid]) {
                input_agent.score = val[newid];
                input_agent.tempstring = temstring;
            }
            // sets to 0 when done, for testing use another number, after testing is done set to 0
            input_agent.flag = 0;
        }
        rands.reset();
        rande.reset();
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

    void Agent::agent_explore_new(Agent &input_agent, vector<double> &val,int Asearch,int Bsearch) {
        // if agent with flag -1 will explore
        //cout<<search<<endl;
        if(input_agent.species=='A' && Asearch==0) agent_explore_random_0(input_agent,val);
        if(input_agent.species=='B' && Bsearch==0) agent_explore_random_0(input_agent,val);
        if(input_agent.species=='A' && Asearch==1) agent_explore_random_1(input_agent,val);
        if(input_agent.species=='B' && Bsearch==1) agent_explore_random_1(input_agent,val);
        if(input_agent.species=='A' && Asearch==2) agent_explore_flip(input_agent,val);
        if(input_agent.species=='B' && Bsearch==2) agent_explore_flip(input_agent,val);
        if(input_agent.species=='A' && Asearch==3) agent_explore_shuffle(input_agent,val);
        if(input_agent.species=='B' && Bsearch==3) agent_explore_shuffle(input_agent,val);
        
    }

    std::vector<int> pool_con(Agent input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f){
        vector<int> temp(36);
        for(int i=0;i<4;i++){// fills temp with all possible connections including duplicates, will also fill temp up simultaneously from the other 6 agents
            temp[i]=a.connections[i];
                //cout<<a.id<<"id "<<a.connections[i]<<" a con"<<endl;
            temp[6+i]=b.connections[i];
                //cout<<b.id<<"id "<<b.connections[i]<<" b con"<<endl;
            temp[12+i]=c.connections[i];
                //cout<<c.id<<"id "<<c.connections[i]<<" c con"<<endl;
            temp[18+i]=d.connections[i];
                //cout<<d.id<<"id "<<d.connections[i]<<" d con"<<endl;
            temp[24+i]=e.connections[i];
            temp[30+i]=f.connections[i];
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
        input_agent.tempconnections.resize(6);
    }

    void Agent::agent_asymmetric_swap(int num,vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int loop){
        // swaps agent that is in minority; by going through all connections i.e agent 98 will have the potential to get agents 90 through 6's connections
        // will randomly only get 6.
        // loops internally through the value of input_agent.connection_replace
        
        int A=input_agent.id; //input agent named A 
        vector<int> pool=pool_con(input_agent,a,b,c,d,e,f); //set of all 1st degree connected agents that we pick C* from
        vector<int> old_cons=input_agent.connections;
        vector<int> C_potential;
        std::vector<int>::iterator loc = std::find(old_cons.begin(), old_cons.end(), loop);
        int index = std::distance(old_cons.begin(), loc);
        int B=old_cons[index];

            //cout<<input_agent.id<<endl;
            for(unsigned int i=0;i<pool.size();i++){ //find and only adds agents for C* from the pool that are not connected to A
                bool found = false;
                for (auto & elem : Agents[pool[i]].connections)
                    if (elem == A)
                    {
                        found = true;
                        break;
                    }
                if(found==true){continue;}
                if(found==false)
                    {
                        C_potential.push_back(pool[i]);
                    }
            }

            if(C_potential.size()==0){
                C_potential.push_back(B);
            }
            
            std::uniform_int_distribution<> c_sel(0,C_potential.size()-1); //random selection of C* from C_potential.
            std::random_device Crdm;
            std::mt19937 genC(Crdm());
            int C=C_potential[c_sel(genC)];
            c_sel.reset();
            /*cout<<"\n before\n";
            cout<<Agents[A].id<<endl;
            for (int i = 0; i < 6; ++i)
            {
                cout<<Agents[A].connections[i]<<" ";
            }*/
            std::vector<int>::iterator ABit = std::find(Agents[A].connections.begin(), Agents[A].connections.end(), B);
            int ABindex = std::distance(Agents[A].connections.begin(), ABit);
            Agents[A].connections[ABindex]=C;


        /*cout<<"\n after\n";
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }*/
        input_agent.minority=0;
    }

    
    vector<int> Agent::agent_connections_score_match(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f){// creates vector filled with agents that their score is less than Primary Agent requirement
        std::vector<int> match;
        {
            //cout<<input_agent.connections.size()<<" cinnections size\n";
            for (int i=0;i<input_agent.connections.size(); ++i)
            {
                //cout<<Agents[input_agent.connections[i]].id<<" "<<"match size"<<match.size()<<endl;
                if(input_agent.score>=Agents[input_agent.connections[i]].score){
                    //cout<<input_agent.connections[i]<<"match\n";
                    match.push_back(input_agent.connections[i]);
                }
            }
        }
        return match;
    }


    void Agent::agent_swap_symmetric(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f,int loop){ //swaps an agents connection using the adjacency matrix
        //loops from the outside using the function agent_swap_hack
        
        int A=input_agent.id; //input agent named A 
        //std::uniform_int_distribution<> selected(0, input_agent.connections.size()-1); //random selction for B
        //std::random_device Srdm;
        //std::mt19937 gen(Srdm());
        vector<int> pool=pool_con(input_agent,a,b,c,d,e,f); //set of all 1st degree connected agents that we pick C* from
        vector<int> old_cons=input_agent.connections;
        vector<int> C_potential;
        vector<int> D_potential;
        std::vector<int>::iterator loc = std::find(old_cons.begin(), old_cons.end(), loop);
        int index = std::distance(old_cons.begin(), loc);
        int B=old_cons[index];
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

        if(C_potential.size()==0) return;
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
        c_sel.reset();
        while(D_potential.size()==0)
        {
            C=C_potential[c_sel(Crdm)];
            C_cons=Agents[C].connections;
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
        d_sel.reset();
        while(D==C||D==B) {
            D=D_potential[d_sel(Drdm)];
            d_sel.reset();
        }

        /*
        cout<<"before\n";
        cout<<Agents[A].id<<endl;
        for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[B].id<<endl;
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[B].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[C].id<<endl;
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[C].connections[i]<<" ";
        }
        cout<<endl;
        cout<<Agents[D].id<<endl;
                for (int i = 0; i < 6; ++i)
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
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[B].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[C].connections[i]<<" ";
        }
        cout<<endl;
                for (int i = 0; i < 6; ++i)
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
        
        /*catch(int size){
            cout<<input_agent.id<<" size is 0\n";
            for (int i = 0; i < 4; ++i)
            {
                cout<<input_agent.connections[i]<<" ";
            }
            cout<<endl;
            return;
        }
        */           
    }
    void Agent::agent_swap_hack_symmetric(vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,vector<int> agent_list){ //used for the matrix method to loop through connection_replace and replace
        for (int i=0;i<agent_list.size(); ++i)
        {
            if(agent_list[i]==-1||agent_list.size()==0)break;
            //cout<<"func sy\n";
           //cout<<agent_list[i]<<endl;
           agent_swap_symmetric(Agents, input_agent, a, b, c, d, e, f,agent_list[i]);
        }
    }

    void Agent::agent_swap_hack_asymmetric(int num,vector<Agent> &Agents,Agent &input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int Criterion,vector<int> agent_list){ //used for the connection method to loop through connection_replace and replace connections
        for (int i=0;i<agent_list.size(); ++i)
        {
            if(agent_list[i]==-1||agent_list.size()==0)break;
            //cout<<"func asy\n";
            //cout<<agent_list[i]<<endl;
            agent_asymmetric_swap(num,Agents, input_agent, a, b, c, d, e, f,agent_list[i]);
        }
    }

    vector<int> Agent::agent_connections_species_match(vector<Agent> Agents,Agent input_agent,Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int condition){// creates vector filled with agents that match conditions requirement
        std::vector<int> match;
        if(condition==1)
        {
            for (int i=0;i<input_agent.connections.size(); ++i)
            {
                //cout<<Agents[input_agent.connections[i]].id<<" "<<Agents[input_agent.connections[i]].species<<endl;
                if(input_agent.species!=Agents[input_agent.connections[i]].species){
                    //cout<<input_agent.connections[i]<<"match\n";
                    match.push_back(input_agent.connections[i]);
                }
            }
        }
        if(condition==-1)
        {
            for (int i=0;i<input_agent.connections.size(); ++i)
            {
                //cout<<Agents[input_agent.connections[i]].id<<" "<<Agents[input_agent.connections[i]].species<<endl;
                if(input_agent.species==Agents[input_agent.connections[i]].species){
                    //cout<<input_agent.connections[i]<<"match\n";
                    match.push_back(input_agent.connections[i]);
                }
            }
        }
        //cout<<"size "<<match.size()<<endl;
        if(match.size()==0)match.push_back(-1);
        //cout<<"elem\n";
        return match;
    }


    void Agent::matrix_fill_before(int m_id,vector<int> filler)
        {//fills matrix with old connections
            for (unsigned int i = 0; i < filler.size(); i++)
            {
                ::matrix[m_id][filler[i]]=1;
                ::matrix[filler[i]][m_id]=1;
            }
        }

    void Agent::matrix_fill_after(int m_id,vector<int> filler)
        {// fill matrix with new connection
            for (unsigned int i = 0; i < filler.size(); i++)
            {
                ::matrix[m_id][filler[i]]=5;
                ::matrix[filler[i]][m_id]=5;
            }
        }
    void Agent::matrix_print(vector<Agent> agent_array)
    {//prints and counts new connections,
        memset(::matrix,0,sizeof(::matrix));
        for(int i=0;i<100;i++)
        {
            for (int j = 0; j < agentcounta; j++)
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
        for (int i = 0; i < agentcounta; ++i)
        {
            int count=0;
            for (int j = 0; j < agentcounta; ++j)
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
		int f_flag=0;
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
    				f_flag=1;
    				cout<<"change "<<input_agent.species<<endl;
    			}
    			if(f_flag==0 && input_agent.species=='B')
    			{
    				input_agent.species='A';
    				cout<<"change "<<input_agent.species<<endl;
    			}
                binom.reset();
    		}
		//cout<<"agent species after "<<input_agent.species<<endl;
	}
    void Agent::agent_exploit_weighted(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int mode,int Criterion) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
        // will mutate agents string and score if they exploit
        double temscore = input_agent.score;
        string temstring = input_agent.binarystring;
        double ascore;
        double bscore;
        double cscore;
        double dscore;
        double escore;
        double fscore;
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
        
        if(mode==1)agent_minority_status_symmetric(input_agent,a,b,c,d,e,f);
        if(mode==-1)agent_minority_status_asymmetric(input_agent,a,b,c,d,e,f,Criterion);
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
        if(input_agent.species==e.species) escore=e.score+e.score*multiplier;
        else escore=e.score-e.score*multiplier;
        if(input_agent.species==f.species) fscore=f.score+f.score*multiplier;
        else fscore=f.score-f.score*multiplier;

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

        if (temscore < escore) {
            temscore = e.score;
            temstring = e.binarystring;
            input_agent.flag = 1;
            target_id=4;
        }
        if (temscore < fscore) {
            temscore = f.score;
            temstring = f.binarystring;
            input_agent.flag = 1;
            target_id=5;
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
                case 4: target_agent=e;break;
                case 5: target_agent=f;break;
            }
            //morph_agent(input_agent,target_agent);
            //char new_test=input_agent.species;
            //cout<<input_agent.id<<"\t"<<new_test<<endl;
        }
        if (input_agent.flag == 0) input_agent.flag = -1;
    }

    void Agent::agent_exploit_weighted_inverse(Agent &input_agent, Agent a, Agent b, Agent c, Agent d,Agent e, Agent f,int mode,int Criterion) {
        //checks connections and assigns new score to main agent if needed otherwise sets flag to -1
        // also checks if it is in the minority and sets the minority flag to 1
        // will mutate agents string and score if they exploit
        double temscore = input_agent.score;
        string temstring = input_agent.binarystring;
        double ascore;
        double bscore;
        double cscore;
        double dscore;
        double escore;
        double fscore;
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
        
        if(mode==1)agent_minority_status_symmetric(input_agent,a,b,c,d,e,f);
        if(mode==-1)agent_minority_status_asymmetric(input_agent,a,b,c,d,e,f,Criterion);
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
        if(input_agent.species!=e.species) escore=e.score-e.score*multiplier;
        else escore=e.score+e.score*multiplier;
        if(input_agent.species!=f.species) fscore=f.score-f.score*multiplier;
        else fscore=f.score+f.score*multiplier;

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

        if (temscore < escore) {
            temscore = e.score;
            temstring = e.binarystring;
            input_agent.flag = 1;
            target_id=4;
        }

        if (temscore < fscore) {
            temscore = f.score;
            temstring = f.binarystring;
            input_agent.flag = 1;
            target_id=5;
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
                case 4: target_agent=e;break;
                case 5: target_agent=f;break;
            }
            //morph_agent(input_agent,target_agent);
            //char new_test=input_agent.species;
            //cout<<input_agent.id<<"\t"<<new_test<<endl;
        }
        if (input_agent.flag == 0) input_agent.flag = -1;
    }



void Agent::agent_asymmetric_swap_global(int num,vector<Agent> &Agents,Agent &input_agent,int loop,int Criterion){
        int A=input_agent.id; //input agent named A 
        vector<int> old_cons=input_agent.connections;
        vector<int> C_potential;
        std::vector<int>::iterator loc = std::find(old_cons.begin(), old_cons.end(), loop);
        int index = std::distance(old_cons.begin(), loc);
        std::uniform_int_distribution<> global(0,agentcounta); //random selection of C* from C_potential.
        std::random_device globalrdm;
        std::mt19937 genGlobal(globalrdm());
        int B=old_cons[loop];

            //cout<<input_agent.id<<endl;
            for(unsigned int i=0;i<Criterion;i++){ //find and only adds agents for C* from the pool that are not connected to A
                int choice=global(genGlobal);
                while( choice==A) choice=global(genGlobal);
                global.reset();
                bool found = false;
                for (auto & elem : Agents[choice].connections)
                    if (elem == A)
                    {
                        found = true;
                        break;
                    }
                if(found==true){continue;}
                if(found==false)
                    {
                        C_potential.push_back(choice);
                    }
            }

            if(C_potential.size()==0){
                C_potential.push_back(B);
            }
            
            std::uniform_int_distribution<> c_sel(0,C_potential.size()-1); //random selection of C* from C_potential.
            std::random_device Crdm;
            std::mt19937 genC(Crdm());
            int C=C_potential[c_sel(genC)];
            for (int i = 0; i <6; ++i)
            {
            	bool found = false;
                for (auto & elem : input_agent.connections)
                    if (elem == C)
                    {
                        found = true;
                        C=global(genGlobal);
                        int count=0;
                        while(elem==C){
                          C=C_potential[c_sel(genC)];
                          ++count;
                          if(count==6){
                           C=global(genGlobal);
                           break;
                       		}
                      	}
                    }

            }

        	c_sel.reset();
            /*cout<<"\n before\n";
            cout<<Agents[A].id<<endl;
            for (int i = 0; i < 6; ++i)
            {
                cout<<Agents[A].connections[i]<<" ";
            }*/
			std::vector<int>::iterator ABit = std::find(Agents[A].connections.begin(), Agents[A].connections.end(), B);
            int ABindex = std::distance(Agents[A].connections.begin(), ABit);
            Agents[A].connections[ABindex]=C;            
            std::sort(input_agent.connections.begin(),input_agent.connections.end());

        /*cout<<"\n after\n";
                for (int i = 0; i < 6; ++i)
        {
            cout<<Agents[A].connections[i]<<" ";
        }*/
        input_agent.minority=0;
	}


void Agent::agent_swap_hack_asymmetric_global(int num,vector<Agent> &Agents,Agent &input_agent, int Criterion){ //used for the connection method to loop through connection_replace and replace connections
        for (int i=0;i<Criterion; ++i)
        {
            //if(agent_list[i]==-1||agent_list.size()==0)break;
            //cout<<"func asy\n";
            //cout<<agent_list[i]<<endl;
            agent_asymmetric_swap_global(num,Agents, input_agent,i,Criterion);
        }
    }