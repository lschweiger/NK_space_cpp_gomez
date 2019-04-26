#include <iostream>

using std::cout;
using std::endl;
using std::to_string;
using std::string;

#include <fstream>
#include <utility>
#include <vector>  //vectors

using std::vector;
#include <iomanip>
#include <bitset> //binary to numbers
#include <cmath>
#include <random>
#include <algorithm>
#include <sstream>

//#include <thread>
int n = pow(2, 20);
const int nbgh = 100;
const int agentcount = 100;
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
        //if(type[i]=='A') acount++;
        //if(type[i]=='B') bcount++;
        //cout<<types[i]<<endl;
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

    void agent_swap(Agent &input_agent) {
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

    void agent_explore(Agent &input_agent, vector<string> &istring, vector<double> &val) {
        // if agent with flag -1 will explore

        //int random=rand()%20;
        std::uniform_int_distribution<> randm(0, 19);
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
    void agent_minority_swap(int num,Agent &input_agent,Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h){
    	// swaps agent that is in minority; by go one connection through i.e agent 98 will have the potential to get agents 90 through 6's connections
    	// will randomly only get 8 from the 16.
        
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
                temp[24+i]=d.connections[i];
                temp[32+i]=e.connections[i];
                temp[40+i]=f.connections[i];
                temp[48+i]=g.connections[i];
                temp[56+i]=h.connections[i];
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
                input_agent.connections[j]=newcons[j];
            }
            sort(input_agent.connections.begin(), input_agent.connections.end());
        //when down resets flag back to 0, regardless of status
        input_agent.minority=0;
    };
};

/*
// added threading to speed up the agent explore
void threads(int a,int b,vector<Agent>& agent_array,std::pair<vector<string>,vector<double>> NKspace){
	for (int i = a; i < b; i++)
	{
		if(agent_array[i].flag==-1){
		agent_array[i].agentexplore(agent_array[i],NKspace);
		//agent_array[i].agentexplore(agent_array[i],NKspace);
		//cout<<agent_array[i].id<<" id \t"<<agent_array[i].score<<" \033[1;31mnew_score\033[0m "<<agent_array[i].tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
		//cout<<agent_array[i].binarystring<<" \033[1;34mnew_string\033[0m "<<agent_array[i].tempstring<<" \033[1;33mold_string\033[0m "<<"\n";		
		}
	}
};
*/

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

void output_round(int NKspace,int round,vector<int> &rounds,vector<double> &scr,vector<double> &ag,vector<double> mc){
	std::ofstream out("8c-NK_space_"+to_string(NKspace)+".txt");
	out<<"round,"<<"max score,"<<"avg score,"<<"minority count"<<"\n";
	for (int i=0;i<round; i++){
		out<<std::setprecision(15)<<rounds[i]<<","<<scr[i]<<","<<ag[i]<<","<<std::setprecision(15)<<mc[i]<<"\n";
	}
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
    vector<double> maxscore(100);
    vector<int> maxround(100);
    vector<double> minoritycount(100);
    vector<double> avgscores(100);
    vector<char> type(::agentcount);
    AB_list_seg(type);

// placing both vectors into a pair
    //std::pair <vector<string>, vector<double>> NKspace(NKspacevals, NKspacescore);
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
		agent_array.resize(100);
		NKspacescore.resize(100);
		maxscore.resize(100);
		maxround.resize(100);
		minoritycount.resize(100);
		avgscores.resize(100);
		type.resize(100);
		rounds = 0;
    	nums = 0;
    	mcount=0;
    	eq=0.0;
    	max=0.0;
	}
	AB_list_seg(type);
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
    	cout<<rounds<<endl;
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
            i->agent_swap(*i);
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
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            if (i->minority == 1) {
                i->agent_minority_swap(i->id,*i,
                                       agent_array[i->connections[0]],agent_array[i->connections[1]],
                                       agent_array[i->connections[2]], agent_array[i->connections[3]],
                                       agent_array[i->connections[4]], agent_array[i->connections[5]],
                                       agent_array[i->connections[6]], agent_array[i->connections[7]]);
                //cout<<i->id<<"id"<<endl;
                //cout<<i->connections[0]<<" ";
                //cout<<i->connections[1]<<" ";
                //cout<<i->connections[2]<<" ";
                //cout<<i->connections[3]<<" ";
                //cout<<i->connections[4]<<" ";
                //cout<<i->connections[5]<<" ";
                //cout<<i->connections[6]<<" ";
                //cout<<i->connections[7]<<endl;
            }
        }
        for (int i = 0; i < ::agentcount; i++) {
            //cout<<agent_array[i].score<<endl;
            if (max < agent_array[i].score) {
                max = agent_array[i].score;
            }

        }
        //round++;
        //maxscore.push_back(max);
        //maxround.push_back(rounds);
        maxscore[rounds] = max;
        maxround[rounds] = rounds;
		eq=maxscore[rounds];
        avgscore = 0;
        for (int i = 0; i < ::agentcount; i++) {
            avgscore += agent_array[i].score;
        }
        avgscores[rounds] = (avgscore / (::agentcount));
        int eqflag=0;
    	if(fabs((avgscore/100.0)-eq)<pow(10,-14) && rounds>=15){
    		//cout<<fabs((avgscore/100.0)-eq)<<"fabs"<<endl;
    		eqflag=1;
    	}
    	else{
    		//cout<<pow(10,-14)<<endl;
    		//cout<<eq<<"eq"<<endl;
    		//cout<<fabs((avgscore/100.0)-eq)<<endl;
    		//eq=avgscore/100.0;
    		//cout<<avgscore/100.0<<endl;
    		
    	}	
    	if(eqflag==1){cout<<"ending"<<endl;break;}
    }
    /*for (int i = 0; i < rounds; i++) {
        cout << maxscore[i] << " \033[1;31mmax score for round\033[0m " << i << endl;
        cout << avgscores[i] << " \033[1;32mavg score for round\033[0m " << i << "\n";
        cout << minoritycount[i] << " \033[1;34mminority count for round\033[0m " << i << "\n";
    }*/
    output_round(NKspace_num,rounds,maxround,maxscore,avgscores,minoritycount);
}
    return 0;
}