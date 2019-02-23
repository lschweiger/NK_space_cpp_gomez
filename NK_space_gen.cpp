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



int main(){
cout.setf(std::ios::fixed);
cout.setf(std::ios::showpoint);
cout.precision(20);


//length of element for vectors and initializing each one a string of 0,1, length of 20
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


return 0;
}