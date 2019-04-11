#include <iostream>

using std::cout;
using std::endl;
using std::to_string;
using std::string;

#include <fstream>
#include <utility>
#include <vector>  //vectors

using std::vector;

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

class Agent // class for agents; includes variable and functions.
{
public:
    char species;            // agent species type
    int minority;            // is agent in minority by species compared to it connections
    int id;
    int flag = 0;
    string binarystring;    //stores main string
    string tempstring;        //stores temp string if agent exploits
    double score;            //stores primary score
    double tempscore;        //stores temp score if agent exploits
    int connections[8];        //array of connections will be filled by agentconnections
    //flag is use in swap and exploit

    void agentconnections(int num, Agent &input_agent) {
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

    void agentchange(int num, Agent &input_agent, std::pair <vector<string>, vector<double>> pair) {
        // changes sets score and binarystring by random value.
        input_agent.score = pair.second[num];
        input_agent.binarystring = pair.first[num];
    };

    void agentexploit(Agent &input_agent, Agent a, Agent b, Agent c, Agent d, Agent e, Agent f, Agent g, Agent h) {
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

    void agentswap(Agent &input_agent) {
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

    void agentexplore(Agent &input_agent, std::pair <vector<string>, vector<double>> pair) {
        // if agent with flag -1 will explore
        //checks if flag is really -1
        //int random=rand()%20;
        std::uniform_int_distribution<> randm(0, 19);
        std::random_device rdm;
        int random = randm(rdm);
        //cout<<random<<endl;
        if (input_agent.flag == -1) {
            //keeps running until flag is not -1
            string temstring = input_agent.binarystring;
            if (temstring[random] == '1') {
                temstring[random] = '0';
            } else {
                temstring[random] = '1';
            }
            std::bitset<20> newid = std::bitset<20>(temstring);
            // if new string and score are better assigns them to agent
            if (input_agent.score < pair.second[newid.to_ulong()]) {
                input_agent.score = pair.second[newid.to_ulong()];
                input_agent.binarystring = temstring;
            }
            // should be set to 0, after testing is done
            input_agent.flag = 0;
        }
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

int main(int argc, char *argv[]) {
    srand(1); //start from 1
    std::stringstream convert1(argv[1]);
    std::stringstream convert2(argv[2]);
    int start;
    convert1 >> start;
    int end;
    convert2 >> end;

    vector <Agent> agent_array(::agentcount);
    cout.setf(std::ios::fixed);
    cout.setf(std::ios::showpoint);
    cout.precision(15);
    vector <string> NKspacevals(n);
    vector<double> NKspacescore(n);
    double max = 0;
//int round=0;
    vector<double> maxscore(100);
    vector<int> maxround(100);
    vector<double> minoritycount(100);
    vector<double> avgscores(100);
    vector<char> AB(::agentcount);
    vector<char> type(::agentcount);
    for (int i = 0; i < ::agentcount; ++i) {
        if (i < 50) AB[i] = 'A';
        if (i >= 50) AB[i] = 'B';
    }
    //int acount=0;
    //int bcount=0;
    int temp = 0;
    for (int i = 0; i < ::agentcount; ++i) {
        temp = rand() % (::agentcount - i);
        type[i] = AB[temp];
        AB.erase(AB.begin() + temp);
        //if(type[i]=='A') acount++;
        //if(type[i]=='B') bcount++;
        //cout<<type[i]<<" As "<<acount<<" Bs"<<bcount<<endl;
    }
    open_space_scores(0, NKspacescore);
    open_space_string(NKspacevals);
// placing both vectors into a pair
    std::pair <vector<string>, vector<double>> NKspace(NKspacevals, NKspacescore);

    int nums = 0;
    double mcount;
    for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {

        i->agentconnections(nums, *i);
        i->species = type[nums];
        //cout<<i->id<<endl;
        //cout<<i->species<<endl;
        i->agentchange(rand() % (::n), *i, NKspace);
        nums++;
    }
    for (int rounds = 0; rounds < 100; rounds++) {

        mcount = 0;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {

            i->agentexploit(*i, agent_array[i->connections[0]], agent_array[i->connections[1]],
                            agent_array[i->connections[2]], agent_array[i->connections[3]],
                            agent_array[i->connections[4]], agent_array[i->connections[5]],
                            agent_array[i->connections[6]], agent_array[i->connections[7]]);
            //cout<<i->id<<" \033[1;31mid\033[0m "<<i->species<<" \033[1;32mspecies\033[0m "<<"\n";
            //cout<<i->minority<<" minority"<<" \033[1;34mnew_string\033[0m "<<endl;
            if (i->minority == 1)
                mcount++;

        }
        mcount = mcount / 100;
        //cout<<mcount<<endl;
        minoritycount[rounds] = mcount;
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            i->agentswap(*i);
            //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
            //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
        }

        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            if (i->flag == -1) {
                i->agentexplore(*i, NKspace);
                //agent_array[i].agentexplore(agent_array[i],NKspace);
                //cout<<i->id<<" id \t"<<i->score<<" \033[1;31mnew_score\033[0m "<<i->tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
                //cout<<i->binarystring<<" \033[1;34mnew_string\033[0m "<<i->tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
            }
        }
        for (vector<Agent>::iterator i = agent_array.begin(); i != agent_array.end(); i++) {
            if (i->minority == 1) {

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

        double avgscore = 0;
        for (int i = 0; i < ::agentcount; i++) {
            avgscore += agent_array[i].score;
        }
        avgscores[rounds] = (avgscore / (::agentcount));

    }
    for (int i = 0; i < maxscore.size(); i++) {
        cout << maxscore[i] << " \033[1;31mmax score for round\033[0m " << i << endl;
        cout << avgscores[i] << " \033[1;32mavg score for round\033[0m " << i << "\n";
        cout << minoritycount[i] << " \033[1;34mminority count for round\033[0m " << i << "\n";
    }

    return 0;
}