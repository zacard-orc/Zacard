//C++ CF FILTER based on MapReduceV1 Reducer
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <map>
#include <string.h>

using namespace std;

#define USER_SIZE 6
#define ITEM_SIZE 7
#define V 2
#define S 2


struct MyPair
{
	int id;
	float value;
	MyPair(int i=0,float v=0):id(i),value(v)
	{}
};

struct cmp
{
	bool operator() (const MyPair &obj1,const MyPair &obj2) const
	{
		return obj1.value<obj2.value;	
	}
};


float  rate[USER_SIZE][ITEM_SIZE];
float  rate_avg[USER_SIZE];
map<string,string> cfg;
MyPair nbi[ITEM_SIZE][V];
MyPair nbu[ITEM_SIZE][S];

void LoadCfg();
void CrtRate();
void ShowRate();
void getAvgRate();
void getNBI();
void getNBU();
float getsim(const vector<float> &vec1,const vector<float> &vec2);
float getUserSim(const vector<float> &user1,const vector<float> &user2);
float getPredict(const vector<float> &user,int index);
float predictRate(int user,int iterm);

int main()
{
	LoadCfg();
	CrtRate();
	getAvgRate();
	cout<<endl;
	getNBI();
	cout<<endl;
	for(int i=0;i<7;i++)
	{
		for(int j=0;j<2;j++)
		{
			cout<<left<<nbi[i][j].id<<":"<<setw(11)<<nbi[i][j].value<<" ";	
		}
		cout<<endl;
	}
	cout<<endl;
	getNBU();
	cout<<endl;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<2;j++)
	    {
			cout<<left<<nbu[i][j].id<<":"<<setw(11)<<nbu[i][j].value<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	int user,iterm;
	user=atoi(cfg["PRUSER"].c_str());
	iterm=atoi(cfg["PRITEM"].c_str());
	cout<<user<<":"<<iterm<<endl;
	cout<<predictRate(user,iterm)<<endl;
}


float predictRate(int user,int iterm)
{
	float sum1=0;
	float sum2=0;
	for(int i=0;i<S;++i)
	{
        int neib_index=nbu[user][i].id;
		float neib_sim=nbu[user][i].value;
	    sum1+=neib_sim*(rate[neib_index][iterm]-rate_avg[neib_index]);
	    sum2+=fabs(neib_sim);
	}
    cout<<"rate_avg[user]="<<rate_avg[user]<<endl;
	cout<<"sum1/sum2="<<sum1/sum2<<endl;
	return rate_avg[user]+sum1/sum2;
}

float getPredict(const vector<float> &user,int index)
{
    float sum1=0;
	float sum2=0;
	//cout<<"sum1+=neib_sim*user[neib_index];"<<endl;
	//cout<<"sum2+=fabs(neib_sim);"<<endl;
	for(int i=0;i<2;++i)
	{
        int    neib_index=nbi[index][i].id;
		float  neib_sim=nbi[index][i].value;
		sum1+=neib_sim*user[neib_index];
		sum2+=fabs(neib_sim);
		/*
		cout<<"index="<<setw(4)<<index;
		cout<<",i="<<setw(4)<<i;
		cout<<",neib_index="<<setw(4)<<neib_index;
		cout<<",user[neib_index]="<<setw(8)<<user[neib_index];
		cout<<",neib_sim="<<setw(8)<<neib_sim;
		cout<<",sum1="<<setw(11)<<sum1;
		cout<<left<<",sum2="<<setw(8)<<sum2<<endl;
		*/

	}
	return sum1/sum2;
}


float getUserSim(const vector<float> &user1,const vector<float> &user2)
{
	vector<float> vec1;
	vector<float> vec2;
	int len=user1.size();
	assert(len==user2.size());
	for(int i=0;i<len;++i)
	{
		if(user1[i]!=0 || user2[i]!=0)
		{
			if(user1[i]!=0)
				vec1.push_back(user1[i]);
			else
				vec1.push_back(getPredict(user1,i));
			if(user2[i]!=0)
				vec2.push_back(user2[i]);
			else
				vec2.push_back(getPredict(user2,i));
		}
	
	}
	/*
	vector<float>::iterator vec1_it=vec1.begin();
    vector<float>::iterator vec2_it=vec2.begin();
	cout<<"----vec1:";
	for(;vec1_it!=vec1.end();vec1_it++)
	{cout<<*vec1_it<<" ";}
	cout<<endl;
	cout<<"----vec2:";
	for(;vec2_it!=vec2.end();vec2_it++)
	{cout<<*vec2_it<<" ";}
	cout<<endl;
    */
	return getsim(vec1,vec2);

}

void getNBU()
{
	for(int i=0;i<USER_SIZE;++i) 	
	{
		vector<float> user1;
		priority_queue<MyPair,vector<MyPair>,cmp> neighbour;

		for(int k=0;k<ITEM_SIZE;++k)
			user1.push_back(rate[i][k]);
        /*
		vector<float>::iterator user1_it;	
		cout<<"user1:";
		for(user1_it=user1.begin();user1_it!=user1.end();user1_it++)
		{ cout<<*user1_it<<" ";}
		cout<<endl;
		*/

		for(int j=0;j<USER_SIZE;++j)
		{
			if(j==i)
				continue;
			vector<float> user2;
			for(int k=0;k<ITEM_SIZE;++k)
				user2.push_back(rate[j][k]);
		    vector<float>::iterator user2_it;
			/*
			cout<<"user2:";
			for(user2_it=user2.begin();user2_it!=user2.end();user2_it++)
			{ cout<<*user2_it<<" ";}
			cout<<endl;
			*/
			float sim=getUserSim(user1,user2);
			cout<<"-------->j="<<j<<",sim="<<setw(8)<<sim<<endl;	
			MyPair p(j,sim);
			neighbour.push(p);
		}

		cout<<endl;

		for(int j=0;j<2;++j)
		{
	        nbu[i][j]=neighbour.top();
			cout<<"i="<<i<<":j="<<nbu[i][j].id<<":"<<nbu[i][j].value<<endl;
	        neighbour.pop();

	    }


	}		
		
}


float getsim(const vector<float> &vec1,const vector<float> &vec2)
{
  int len=vec1.size();
  assert(len==vec2.size());
  //cout<<"len_vec1:"<<len<<endl;
  float sum1=0;
  float sum2=0;
  float sum1_1=0;
  float sum2_2=0;
  float sum=0;
  for(int i=0;i<len;i++)
  {
	sum+=vec1[i]*vec2[i];
	sum1+=vec1[i];
	sum2+=vec2[i];
	sum1_1+=vec1[i]*vec1[i];
	sum2_2+=vec2[i]*vec2[i];
	//cout<<sum<<":"<<sum1<<":"<<sum2<<":"<<sum1_1<<":"<<sum2_2<<endl;
  }
  //cout<<sum<<":"<<sum1<<":"<<sum2<<":"<<sum1_1<<":"<<sum2_2<<endl;

  float ex=sum1/len;
  float ey=sum2/len;
  float ex2=sum1_1/len;
  float ey2=sum2_2/len;
  float exy=sum/len;
  float sdx=sqrt(ex2-ex*ex);
  float sdy=sqrt(ey2-ey*ey);
  //cout<<sdx<<":"<<sdy<<endl;
  assert(sdx!=0 && sdy!=0);
  float sim=(exy-ex*ey)/(sdx*sdy);
  //cout<<"sim="<<sim<<endl;
  return sim;
}



void getNBI()
{
	for(int i=0;i<ITEM_SIZE;++i)
	{
		vector<float> vec1;
		priority_queue<MyPair,vector<MyPair>,cmp> neighbour;
		
		for(int k=0;k<USER_SIZE;k++)
			vec1.push_back(rate[k][i]);

		vector<float>::iterator vec1_it;	
		//cout<<"vec1=";
		//for(vec1_it=vec1.begin();vec1_it!=vec1.end();vec1_it++)
		//{	cout<<*vec1_it<<":"; }

	    //cout<<endl;		

		
	    for(int j=0;j<ITEM_SIZE;j++)
		{
			if(i==j)
				continue;
			//cout<<"i="<<i<<",j="<<j<<",";
			vector<float> vec2;
			for(int k=0;k<USER_SIZE;k++)
				vec2.push_back(rate[k][j]);
			float sim=getsim(vec1,vec2);
			//cout<<"-------->j="<<j<<",sim="<<setw(8)<<sim<<endl;
			MyPair p(j,sim);
			neighbour.push(p);
		 //   cout<<"vec2=";
		//		vector<float>::iterator vec2_it;
		//		for(vec2_it=vec2.begin();vec2_it!=vec2.end();vec2_it++)
		//			  cout<<*vec2_it<<"-";
		//	cout<<endl;
	      	
		}

		//cout<<endl<<endl;
		
		for(int j=0;j<2;++j)
		{
			//neighbour.top()
			nbi[i][j]=neighbour.top();
			//cout<<"i="<<i<<",j="<<j<<",xx.id="<<nbi[i][j].id<<",xx.value="<<nbi[i][j].value<<endl;
		    //cout<<nbi[i][j].id<<":"<<nbi[i][j].value;	
			neighbour.pop();
		}
	
	}
}


void getAvgRate()
{
	for(int i=0;i<USER_SIZE;i++)
	{
		float sum=0;
		for(int j=0;j<ITEM_SIZE;j++)
		{
			sum=sum+rate[i][j];	
			cout<<rate[i][j]<<" ";
		}
		rate_avg[i]=sum/ITEM_SIZE;
		cout<<sum<<"|"<<ITEM_SIZE<<"|"<<rate_avg[i]<<endl;
		//cout<<"| "<<rate_avg[i]<<endl;
	}
}

void CrtRate()
{
   std::ios::sync_with_stdio(false);
   string line;
   int head_id;
   float value[ITEM_SIZE];
   while(getline(cin,line))
   {
      string str[ITEM_SIZE];
      string head;
      istringstream strstm(line);
      strstm>>head;
      head_id=atof(head.c_str());
      for(int i=0;i<ITEM_SIZE;i++)
      {
       strstm>>str[i];
       value[i]=atof(str[i].c_str());
       if(rate[head_id][i]==0)
       rate[head_id][i]=value[i];
      }
    }
}

void ShowRate()
{
	for(int i=0;i<USER_SIZE;i++)
	{
	  for(int j=0;j<ITEM_SIZE;j++)
	  cout<<rate[i][j]<<" ";
	  cout<<endl;
	}
}

void LoadCfg()
{
	FILE *fp;
    char *buf;
    char *key;
	char *val;
	buf=(char*)malloc(200*sizeof(char));
	fp=fopen("cfcfg.txt","r");
    while(fgets(buf,1024,fp)!=NULL)
    {
       key=strsep(&buf,"=");
       val=strsep(&buf,"\n");
       cfg[key]=val;
    }
}
