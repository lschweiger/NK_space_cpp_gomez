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
#include <thread>
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
		//checks flag and swaps scores and string, for next round if 1, and resets back to 0 for next round
		if(input_agent.flag==1)
		{
			double temp=input_agent.tempscore;
			string stemp=input_agent.tempstring;
			input_agent.tempscore=input_agent.score;
			input_agent.tempstring=input_agent.binarystring;
			input_agent.score=temp;
			input_agent.binarystring=stemp;
			input_agent.flag=0;
		}
	};
	void agentexplore(Agent& input_agent,std::pair <vector<string>,vector<double>> pair){
		// if agent whith flag -1 will explore
		std::vector<int> v(3);
		std::default_random_engine generator;
  		std::uniform_int_distribution<int> dist(0,19);
  		//checks if flag is really -1
		if (input_agent.flag==-1){
			//keeps running until flag is not -1
			while(input_agent.flag==-1){

 				srand(time(0));
 				for (int i = 0; i < 3; i++)
 				{
 					
 					v[i]=rand()%20;
 					dist.reset();
 					while(i>0&&v[i-1]==v[i]) {
 						v[i]=dist(generator);
 						//cout<<v[i]<<endl;
 						dist.reset();
 					}
 					
 				}
				string tempstring=input_agent.binarystring;
				if(tempstring[v[0]]=='1'){
					tempstring[v[0]]='0';
				}
				else{
					tempstring[v[0]]='1';
				}
				if(tempstring[v[1]]=='1'){
					tempstring[v[1]]='0';
				}
				else{
					tempstring[v[1]]='1';
				}
				if(tempstring[v[2]]=='1'){
					tempstring[v[2]]='0';
				}
				else{
					tempstring[v[2]]='1';
				}
				std::bitset<20> newid=std::bitset<20>(tempstring);
				// if new string and score are better assigns them to agent
				if(pair.second[newid.to_ulong()]>input_agent.score){
					input_agent.score=pair.second[newid.to_ulong()];
					input_agent.binarystring=tempstring;
					input_agent.flag=2; // should be set to 0, after testing is done
				}
			}
			
		}
	};
};
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
srand (1); //start from 1

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

for (int i = 0; i < ::agentcount; i++)
{
	agent_array[i].agentswap(agent_array[i]);
	cout<<agent_array[i].id<<" id \t"<<agent_array[i].score<<" \033[1;31mnew_score\033[0m "<<agent_array[i].tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
	cout<<agent_array[i].binarystring<<" \033[1;34mnew_string\033[0m "<<agent_array[i].tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
}

cout<<"threads"<<endl;
std::thread t1(threads,0,50,std::ref(agent_array),NKspace);
std::thread t2(threads,50,100,std::ref(agent_array),NKspace);
t1.join();
t2.join();
cout<<"threads"<<endl;
for(int i=0; i<100;i++){
	if(agent_array[i].flag==2){
		cout<<agent_array[i].id<<endl;
		agent_array[i].agentexplore(agent_array[i],NKspace);
		cout<<agent_array[i].id<<" id \t"<<agent_array[i].score<<" \033[1;31mnew_score\033[0m "<<agent_array[i].tempscore<<" \033[1;32mold_score\033[0m "<<"\n";
		cout<<agent_array[i].binarystring<<" \033[1;34mnew_string\033[0m "<<agent_array[i].tempstring<<" \033[1;33mold_string\033[0m "<<"\n";
	}
	//agent_array[0].flag=-1;
}

return 0;
}