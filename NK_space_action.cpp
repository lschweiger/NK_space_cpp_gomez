#include <iostream>
using std::cout;
using std::endl; 
using std::to_string;
using std::string;
#include <fstream>
#include <utility>
#include <vector>  //vectors
using std::vector;
#include <cmath>
int n=pow(2,20);


void open_space_scores(int j, vector<float>& input_vec){
	std::fstream in_scores;
	in_scores.open("NKspace_scores_"+to_string(j)+".txt");
	for (int i =0; i<n;++i)
	{
		float i_score;
		in_scores>>i_score;
		cout<<i_score<<'\n';
	}
in_scores.close();
};



int main(){
cout.setf(std::ios::fixed);
cout.setf(std::ios::showpoint);
cout.precision(5);
vector<string> NKspacevals (n);
vector<float> NKspacescore (n);
open_space_scores(0,NKspacescore);
// placing both vectors into a pair
std::pair <vector<string>,vector<float>> NKspace(NKspacevals,NKspacescore);

return 0;
}