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
std::uniform_real_distribution<double> uni(0.000000,1.000000);
std::uniform_real_distribution<double> rando(0,20);
std::random_device rd;
std::random_device uniform;
std::mt19937 mt(uniform());

int nk=pow(2,20);
int n=20;
int k=5;
int powk=pow(2,(k+1));
static double scores[20][64];
void score_assign(int b,int c){
	for (int i = 0; i <=b; ++i)
	{
		for (int l = 0; l <=c; ++l)
		{
			::scores[i][l]=uni(mt);
			uni.reset();
			//cout<<::scores[i][l]<<" ";
		}
	}
}

double Getscore(int rows,long cols){
	//cout<<::scores[rows][cols]<<"test"<<endl;
	return(::scores[rows][cols]);
};

//call by reference for vector v to fill with new values of nk space with the new values from uniform (0,1)
void NKspacevals_gen(vector<double>& input_vec){
	int array[::n][(k+1)];
	
	for (int i = 0; i < ::n; ++i)
	{
		for (int j = 0; j < ::k; ++j)
		{
			array[i][j]=int(::rando(rd));
			//cout<<array[i][j]<<" ";
			rando.reset();
		}
		//cout<<endl;
	}
	
	std::bitset< 7> mask;
	for (int i = 0; i < ::nk; ++i)
	{
		//cout<<i<<endl;
		long ele=0;
		double temp_score=0;
		score_assign(::n,::powk);
		for (int j = 0; j < ::n; ++j)
		{
			for (int l = 0; l < ::k; ++l)
			{
				mask.flip(array[j][l]%7);
				ele=((mask.to_ulong()));
			}
			temp_score+=Getscore(j,ele);

			
		
		}
		input_vec[i]=(temp_score/(float)::n);
		//cout<<input_vec[i]<<"vec"<<endl;
		
	}
	

};

//call by reference for vector v to normalize so one value is exactly 1 and 0
void NKspacevals_unit(vector<double>& input_vec,int n){
	vector<double>::iterator maxresult;
	vector<double>::iterator minresult;
	maxresult = max_element(input_vec.begin(), input_vec.end());
	minresult = min_element(input_vec.begin(), input_vec.end());
	input_vec[int(distance(input_vec.begin(), minresult))]=float(0.000);
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
cout.precision(15);
//length of element for vector 

vector<double> v(nk);
//will check if NKspace_strings exists if so skips straight to NKspace_scores creation otherwise creates it
std::fstream file;
file.open("NKspace_strings.txt", std::ios_base::out | std::ios_base::in);
if (!file.is_open())
{
	// creating and save strings to file
	cout<<"does not exists"<<endl;
	ofstream strings;
	strings.open("NKspace_strings.txt");
	for (int s=0; s<nk;++s)
			{
				strings<<std::bitset< 20 >(s).to_string()<<endl;

			}
	strings.close();
}


//creating and saving scores j<# is the number of spaces to create
for (int j = 0; j < 10; ++j)
{
	NKspacevals_gen(v);
	NKspacevals_unit(v,::nk);
	//saving NKscores to unique files using fstream instead of ofstream due to writing speeds being faster with std::ios_base::out creating 1000 now takes approx 470 seconds 
	std::fstream scores;
	scores.open("NKspace_scores_"+to_string(j)+".txt",std::ios::out);
		for (vector<double>::iterator i = v.begin(); i != v.end(); ++i) //switched to vector iterator
		{

			scores<< std::fixed << std::setprecision(15)<<*i<<"\n";
			
		}
	//scores.flush()
	scores.close();
	cout<<"done"<<endl;
}




return 0;
}