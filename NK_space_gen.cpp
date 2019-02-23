#include <iostream>
using std::cout;
using std::endl; 
using std::to_string;
using std::string;
#include <vector>  //vectors
using std::vector;
#include <bitset> //convert numbers to binary 
#include <fstream> //writing nk space string and values to file
using std::ofstream;
#include <cmath> // pow function
#include <random> // needed for random seed
#include <algorithm>//needed for min/max_element



//declare the uniform distribution, rd for use globally
std::uniform_real_distribution<float> uni(0.,1.000000);
std::random_device rd;

//call by reference for vector v to fill with new values of nk space with the new values from uniform (0,1)
void NKspacevals_gen(vector<float>& v,int n){
	for (int i=0;i<n;++i)
		{
			v[i]=uni(rd);
			uni.reset();
		}
};

//call by reference for vector v to normalize so one value is exactly 1 and 0
void NKspacevals_unit(vector<float>& v,int n){
	vector<float>::iterator maxresult;
	vector<float>::iterator minresult;
	maxresult = max_element(v.begin(), v.end());
	minresult = min_element(v.begin(), v.end());
	v[int(distance(v.begin(), minresult))]=float(0.000);
	cout << "max element at: " << float(distance(v.begin(), maxresult))<<endl;
	//cout << "min element at: " << float(distance(v.begin(), minresult))<<endl;
	for (int i = 0; i < n; ++i)
		{	
			v[i]=v[i]/v[float(distance(v.begin(), maxresult))];
		}
};

int main(){
cout.setf(std::ios::fixed);
cout.setf(std::ios::showpoint);
cout.precision(20);
//length of element for vector 
int n=pow(2,20);
vector<float> v(n);
// creating and save strings to file
ofstream strings;
strings.open("NKspace_strings.txt");
for (int s=0; s<n;++s)
		{
			strings<<std::bitset< 20 >(s).to_string()<<endl;
		}
strings.close();
for (int j = 0; j < 1; ++j)
{
	NKspacevals_gen(v,n);
	NKspacevals_unit(v,n);
	//saving NKscores to unique files
	ofstream scores;
	scores.open("NKspace_scores_"+to_string(j)+".txt");
		for (int i = 0; i <n; i++)
		{
			scores<<"\t"<<v[i]<<endl;
		}
	scores.close();
}

return 0;
}