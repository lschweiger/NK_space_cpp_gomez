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
#include <ios>
#include <iomanip>

//declare the uniform distribution, rd for use globally
double num=240;
double sigma=sqrt(1/num);
std::normal_distribution<double> uni(0.5,sigma);
std::random_device rd;

//call by reference for vector v to fill with new values of nk space with the new values from uniform (0,1)
void NKspacevals_gen(vector<double>& input_vec,int n){
	for (int i=0;i<n;++i)
		{
			input_vec[i]=uni(rd);
			uni.reset();
			//cout<<uni(rd)<<endl;
		}
};

//call by reference for vector v to normalize so one value is exactly 1 and 0
void NKspacevals_unit(vector<double>& input_vec,int n){
	vector<double>::iterator maxresult;
	vector<double>::iterator minresult;
	maxresult = max_element(input_vec.begin(), input_vec.end());
	minresult = min_element(input_vec.begin(), input_vec.end());
	input_vec[int(distance(input_vec.begin(), minresult))]=double(0.000);
	//cout << "max element at: " << int(distance(input_vec.begin(), maxresult))<<endl;
	//cout << "min element at: " << int(distance(input_vec.begin(), minresult))<<endl;
	for (int i = 0; i < n; ++i)
		{	
			input_vec[i]=input_vec[i]/input_vec[double(distance(input_vec.begin(), maxresult))];
		}
};

int main(){
cout.setf(std::ios::fixed);
cout.setf(std::ios::showpoint);
cout.precision(20);
//length of element for vector 
int n=pow(2,20);
vector<double> v(n);
//will check if NKspace_strings exists if so skips straight to NKspace_scores creation otherwise creates it
std::fstream file;
file.open("NK_space_strings.txt", std::ios_base::out | std::ios_base::in);
if (!file.is_open())
{
	// creating and save strings to file
	cout<<"does not exists"<<endl;
	ofstream strings;
	strings.open("NK_space_strings.txt");
	for (int s=0; s<n;++s)
			{
				strings<<std::bitset< 20 >(s).to_string()<<endl;

			}
	strings.close();
}

//creating and saving scores j<# is the number of spaces to create
for (int j = 0; j < 10; ++j)
{
	NKspacevals_gen(v,n);
	NKspacevals_unit(v,n);
	//saving NKscores to unique files using fstream instead of ofstream due to writing speeds being faster with std::ios_base::out creating 1000 now takes approx 470 seconds 
	std::fstream scores;
	scores.open("NK_space_scores_"+to_string(j)+".txt",std::ios::out);
		for (vector<double>::iterator i = v.begin(); i != v.end(); ++i) //switched to vector iterator
		{

			scores<< std::fixed << std::setprecision(15)<<*i<<"\n";
			
		}
	//scores.flush();
	scores.close();
}




return 0;
}