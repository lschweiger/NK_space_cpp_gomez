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
//#include <thread>
int n=pow(2,20);
const int nbgh=100;
const int agentcount=100;
std::uniform_real_distribution<double> uni(0.0,.0005);
std::random_device rd;

void open_space_scores(int j, vector<double>& input_vec){
	std::fstream in_scores;
	in_scores.open("NK_space_scores_"+to_string(j)+".txt");
	for (int i =0; i<::n;++i)
		{
			double i_score;
			in_scores>>i_score;
			//cout<<i_score<<"test"<<'\n';
			input_vec[i]=i_score;
			//cout<<input_vec[i]<<'\n';
		}
	in_scores.close();
};
void open_space_string(vector<string>& string_vec){
	std::fstream strings;
	strings.open("NK_space_strings.txt");
	for (int i =0; i<::n;++i)
		{
			string i_str;
			strings>>i_str;
			string_vec[i]=i_str;
		}
	strings.close();
};

class Agent // class for agents; includes variable and functions.
{
public:
	int id;
	string binarystring;	//stores main string
	string tempstring;		//stores temp string if agent exploits	
	double score;			//stores primary score
	double tempscore;		//stores temp score if agent exploits
	int connections[4];		//array of connections will be filled by agentconnections 
	int flag=0;				//flag is use in swap and exploit

	void agentconnections(int num,Agent& input_agent){ 
		input_agent.id=num; //assigns id from 0 to 99,
		//sets the default conections to -2,-1,1,2 of the current agents id, includes a check for negatives id
		if((num-2)%(::nbgh)<0){
			input_agent.connections[0]=((num-2)%(::nbgh))+::nbgh;
		}
		else{
			input_agent.connections[0]=num-2;
		}
		if((num-1)%(::nbgh)<0){
			input_agent.connections[1]=((num-1)%(::nbgh))+::nbgh;
		}
		else{
			input_agent.connections[1]=num-1;
		}
		input_agent.connections[2]=(num+1)%(::nbgh);
		input_agent.connections[3]=(num+2)%(::nbgh);
		};

	void agentchange(int num,Agent& input_agent, std::pair <vector<string>,vector<double>> pair){ 
		// changes sets score and binarystring by random value.
		input_agent.score=pair.second[num];
		input_agent.binarystring=pair.first[num];
	};

	void agentexploit(Agent& input_agent,Agent a,Agent b,Agent c,Agent d){
		//checks connections and assigns new score to main agent if needed otherwise sets flag to -1
		double temscore=input_agent.score;
		string temstring=input_agent.binarystring;
		if(temscore<a.score){ 
			temscore=a.score;
			temstring=a.binarystring;
			input_agent.flag=1;//
		}
		if(temscore<b.score){ 
			temscore=b.score;
			temstring=b.binarystring;
			input_agent.flag=1;
		}
		if(temscore<c.score){ 
			temscore=c.score;
			temstring=c.binarystring;
			input_agent.flag=1;
		}
		if(temscore<d.score){ 
			temscore=d.score;
			temstring=d.binarystring;
			input_agent.flag=1;
		}
		input_agent.tempscore=temscore;
		input_agent.tempstring=temstring;
		if(input_agent.flag==0) input_agent.flag=-1;
	};

	void agentswap(Agent& input_agent){
		//checks flag and swaps scores and string for next round if 1, and resets back to 0 for next round
		if(input_agent.flag==1)
		{;
			input_agent.score=input_agent.tempscore;
			input_agent.binarystring=input_agent.tempstring;
			input_agent.flag=0;
		}
	};

};


int main(){
vector<Agent> agent_array(100);
cout.setf(std::ios::fixed);
cout.setf(std::ios::showpoint);
cout.precision(15);
vector<string> NKspacevals (n);
vector<double> NKspacescore (n);
open_space_scores(0,NKspacescore);
open_space_string(NKspacevals);
// placing both vectors into a pair
std::pair <vector<string>,vector<double>> NKspace(NKspacevals,NKspacescore);
cout<<NKspace.first[0]<<endl;
cout<<NKspace.second[0]<<endl;
//srand (5); start from 1

for (int i = 0; i < ::agentcount; ++i)
{
	agent_array[i].agentconnections(i,agent_array[i]);
	agent_array[i].agentchange(rand()%(::n),agent_array[i],NKspace);
	
}
//agent_array[0].agentswap(agent_array[0]);
for (int i = 0; i < ::agentcount; i++)
{
	agent_array[i].agentexploit(agent_array[i],agent_array[agent_array[i].connections[0]],agent_array[agent_array[i].connections[1]],agent_array[agent_array[i].connections[2]],agent_array[agent_array[i].connections[3]]);
}
cout<<agent_array[99].id<<"\t id \t"<<agent_array[99].score<<" score \t"<<agent_array[99].binarystring<<"  before \n";
cout<<agent_array[99].binarystring<<" string "<<"\n";
cout<<agent_array[98].id<<"\t id \t"<<agent_array[98].score<<" score \t"<<agent_array[98].binarystring<<" before \n";
cout<<agent_array[98].binarystring<<" string "<<"\n";
cout<<agent_array[0].id<<"\t id \t"<<agent_array[0].score<<" score \t"<<agent_array[0].binarystring<<"  before \n";
cout<<agent_array[0].binarystring<<" string "<<"\n";
cout<<agent_array[1].id<<"\t id \t"<<agent_array[1].score<<" score \t"<<agent_array[1].binarystring<<" before \n";
cout<<agent_array[1].binarystring<<" string "<<"\n";
cout<<agent_array[2].id<<"\t id \t"<<agent_array[2].score<<" score \t"<<agent_array[2].binarystring<<" before \n";
cout<<agent_array[2].binarystring<<" string "<<"\n";
cout<<agent_array[3].id<<"\t id \t"<<agent_array[3].score<<" score \t"<<agent_array[3].binarystring<<" before \n";
cout<<agent_array[3].binarystring<<" string "<<"\n";
cout<<agent_array[4].id<<"\t id \t"<<agent_array[4].score<<" score \t"<<agent_array[4].binarystring<<" before \n";
cout<<agent_array[4].binarystring<<" string "<<"\n";
//
for (int i = 0; i < ::agentcount; i++)
{
	//agent_array[i].agentexploit(agent_array[i],agent_array[agent_array[i].connections[0]],agent_array[agent_array[i].connections[1]],agent_array[agent_array[i].connections[2]],agent_array[agent_array[i].connections[3]]);
	agent_array[i].agentswap(agent_array[i]);
}
cout<<agent_array[99].id<<"\t id \t"<<agent_array[99].score<<" score \t"<<agent_array[99].binarystring<<"  before \n";
cout<<agent_array[99].binarystring<<" string "<<"\n";
cout<<agent_array[98].id<<"\t id \t"<<agent_array[98].score<<" score \t"<<agent_array[98].binarystring<<" before \n";
cout<<agent_array[98].binarystring<<" string "<<"\n";
cout<<agent_array[0].id<<"\t id \t"<<agent_array[0].score<<" score \t"<<agent_array[0].binarystring<<"  before \n";
cout<<agent_array[0].binarystring<<" string "<<"\n";
cout<<agent_array[1].id<<"\t id \t"<<agent_array[1].score<<" score \t"<<agent_array[1].binarystring<<" before \n";
cout<<agent_array[1].binarystring<<" string "<<"\n";
cout<<agent_array[2].id<<"\t id \t"<<agent_array[2].score<<" score \t"<<agent_array[2].binarystring<<" before \n";
cout<<agent_array[2].binarystring<<" string "<<"\n";
cout<<agent_array[3].id<<"\t id \t"<<agent_array[3].score<<" score \t"<<agent_array[3].binarystring<<" before \n";
cout<<agent_array[3].binarystring<<" string "<<"\n";
cout<<agent_array[4].id<<"\t id \t"<<agent_array[4].score<<" score \t"<<agent_array[4].binarystring<<" before \n";
cout<<agent_array[4].binarystring<<" string "<<"\n";
cout<<rand()%(::n)<<endl;
cout<<rand()%(::n)<<endl;
return 0;
}