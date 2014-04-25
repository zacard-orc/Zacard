//C++,CF based on MapReduce Mapper

#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

#define USER_SIZE 6
#define ITEM_SIZE 7


float  rate[USER_SIZE][ITEM_SIZE];
void ShowRare();


int main()
{
	std::ios::sync_with_stdio(false);
	string line;
	int userid,itemid;
	float rating;
	while(getline(cin,line))
	{
	  istringstream strstm(line);
	  string str1,str2,str3;
	  strstm>>str1>>str2>>str3;
	  userid=atoi(str1.c_str());
	  itemid=atoi(str2.c_str());
	  rating=atof(str3.c_str());
	  rate[userid-1][itemid-1]=rating;
	  line.clear();	
	}

	ShowRare();

	return 0;
}

void ShowRare()
{
	for(int i=0;i<USER_SIZE;i++)
	{
		cout<<i<<" ";
		for(int j=0;j<ITEM_SIZE;j++)
		{	cout<<rate[i][j]<<" "; }
		cout<<endl;
	}

}
