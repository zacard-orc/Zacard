//C++ K-Means Single
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <vector>

#define ROWNUM 10000
#define DIV    2
#define CLSNUM 2

using namespace std;


typedef struct Kdatastr
{
  float emt[DIV];
} KS;

typedef struct Rawdatastr
{
  float emt[DIV];
  float distance[CLSNUM];
  int   classid;
} RS;

typedef multimap<float,int> sortmap;

RS raw[ROWNUM];
KS k[CLSNUM];
 
int RDCOUNT;
sortmap ss;


void LoadRawdata(string filename);
void EuclidDis(RS *p,KS *pk);
void ShowRS(RS *p);
float CompNewK(RS *p);
void Vote(RS *p);

int main()
{
    LoadRawdata("kmdata.txt");
	float lastdd=99999;
	float rundd;
	float dd=888888;

    while(dd>2)
	{
		EuclidDis(raw,k);
		Vote(raw);
		cout<<"LASTDD="<<lastdd<<endl;
		rundd=CompNewK(raw);
		cout<<"RUNDD= "<<rundd<<endl;
		dd=fabs(rundd-lastdd);
		lastdd=rundd;
		cout<<"DD=    "<<dd<<endl;
	}
    ShowRS(raw);
}

void ShowRS(RS *p)
{
   cout<<"-----------"<<endl;
   for(int i=0;i<RDCOUNT;i++)
   {
	   for(int j=0;j<DIV;j++)
	    cout<<left<<setw(4)<<(p+i)->emt[j]<<",";
	   for(int z=0;z<CLSNUM;z++)
		cout<<left<<setw(10)<<(p+i)->distance[z];
	   cout<<","<<(p+i)->classid;
	   cout<<endl;
   }
}

void Vote(RS *p)
{
  for(int z=0;z<CLSNUM-1;z++)
  {
	int cc=0;
    for(int i=0;i<RDCOUNT;i++)
	{
     // ss[(p+i)->distance[z]]=i;
	 ss.insert(pair<float,int>((p+i)->distance[z],i));
	}
	sortmap::iterator ss_it;
	for(ss_it=ss.begin();cc<(RDCOUNT/2);ss_it++,cc++)
	{
	   //cout<<ss_it->first<<":"<<ss_it->second<<endl;
       (p+ss_it->second)->classid=1;
	}
	cout<<"-----------"<<endl;
    ss.clear();	
  }
}

float CompNewK(RS *p)
{
  //Save Last K
  KS lastk[CLSNUM];
  for(int i=0;i<CLSNUM;i++)
  {
	  for(int j=0;j<DIV;j++)
	    lastk[i].emt[j]=k[i].emt[j]; 
  }
  
  for(int z=0;z<CLSNUM;z++)
  {
		for(int j=0;j<DIV;j++)    
		{
			float sum=0;
			float cc=0;
			float avv;
			for(int i=0;i<RDCOUNT;i++)
			{ 
				if(z==(p+i)->classid)
				{
					sum+=(p+i)->emt[j];
					cc++;
				}
			}
			avv=sum/(cc);
			k[z].emt[j]=avv;
		}
  }
  vector<float> k_dlt;
  for(int i=0;i<CLSNUM;i++)
  {
	  float dltemp[DIV];
	  float dl=0;
	  cout<<"NEW K= ";
	  for(int j=0;j<DIV;j++)
	  {  
		  cout<<left<<setw(9)<<k[i].emt[j]<<" ";
		  dltemp[j]=pow((k[i].emt[j]-lastk[i].emt[j]),2);
		  dl+=dltemp[j];
	  }
	  k_dlt.push_back(sqrt(dl));
      cout<<"K-HISDIS= "<<sqrt(dl)<<endl;
  }
  //Computer K-Delta
  sort(k_dlt.begin(),k_dlt.end());
  return k_dlt[CLSNUM-1];
}



void EuclidDis(RS *p,KS *pk)
{
  for(int z=0;z<CLSNUM;z++)
  {
	for(int i=0;i<RDCOUNT;i++)
	{
		float dltemp[DIV];
		float dl=0;
		float dis=0;
		(p+i)->classid=0;
		for(int j=0;j<DIV;j++)
		{ 
		    dltemp[j]=pow((p+i)->emt[j]-(pk+z)->emt[j],2);
			dl=dl+dltemp[j];
		}
        (p+i)->distance[z]=sqrt(dl);
	}

  }
}


void LoadRawdata(string filename)
{
   std::ios::sync_with_stdio(false);
   int raw_i=0;
   int k_i=0;
   char *p=NULL;
   char *f[DIV+1];
   string line;
   ifstream ifs;
   ifs.open(filename.c_str());
   while(getline(ifs,line))
   {
      //cout<<line<<endl; 
	  int len = line.length();
	  p=new char[len+1];
	  strcpy(p,line.c_str());
	  f[0]=strsep(&p,"=");

	  if(strcmp(f[0],"kinit")==0)
	  {
		  int j;
		  for(j=1;j<DIV;j++)
		  {
		     f[j]=strsep(&p,",");
		     k[k_i].emt[j-1]=atof(f[j]);
		  }
		  f[j]=strsep(&p," ");
		  k[k_i].emt[j-1]=atof(f[j]);
		  k_i++;
	  }

	  if(strcmp(f[0],"raw")==0)
	  {
		  int j;
		  for(j=1;j<DIV;j++)
		  {
		     f[j]=strsep(&p,",");
		     raw[raw_i].emt[j-1]=atof(f[j]);
		  }
		  f[j]=strsep(&p," ");
		  raw[raw_i].emt[j-1]=atof(f[j]);
		  raw_i++;
	  }  
	  line.clear();
   }
   RDCOUNT=raw_i;
}

