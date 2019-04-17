void threads(int a,int b,vector<Agent>& agent_array,std::pair<vector<string>,vector<double>> NKspace){
	for (int i = a; i < b; i++)
	{
			agent_array[i].agentassignment(i,agent_array[i]);
			agent_array[i].agentchange(i,agent_array[i],NKspace);
	}
};

std::thread t1(threads,0,50,std::ref(agent_array),NKspace);
std::thread t2(threads,50,100,std::ref(agent_array),NKspace);
t1.join();
t2.join();