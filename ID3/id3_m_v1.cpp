//C++ ID3 MAPPER
#include <iostream>  
#include <sstream>
#include <string.h>  
#include <stdio.h>
#include <malloc.h>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

  
using namespace std;  
  
#define BUFFER_SIZE 10240
#define DELIM "\t"
#define FILEDNUM 5 

set<string>                             f[FILEDNUM];                    // ENUM PER FIELD
vector<string>          inputdata[FILEDNUM];    // RAW INPUT DATA
map<string,string>      cfg;
typedef map<string,int> st;
typedef map<string,st>  tst;

  
int main(int argc, char** argv)  
{  
      char *buf;
          char *cur = NULL;  
          buf=(char*)malloc(10240*sizeof(char));

      while(fgets(buf, BUFFER_SIZE-1, stdin))  
             { 
                         for(int i=0;i<FILEDNUM;i++)
                         {
                                if(i<FILEDNUM-1)
                            {
                                        cur = strsep(&buf,DELIM); 
                                        f[i].insert(cur);
                                        inputdata[i].push_back(cur);
                                }
                                else
                                {
                                cur = strsep(&buf,"\n");
                                        f[i].insert(cur);
                                        inputdata[i].push_back(cur);
                                }
                         }
                 }

set<string>::iterator sit;
vector<string>::iterator datait;
vector<string>::iterator satit;

vector<string> tempv;

//Computer Root Entropy Raw;
for(sit=f[4].begin();sit!=f[4].end();sit++)
{
   int cc=count(inputdata[4].begin(),inputdata[4].end(),*sit);
   cout<<"root "<<*sit<<" "<<cc<<endl;
}

//Computer Attrabiute Counts;
for(int p=0;p<FILEDNUM-1;p++)
{
for(sit=f[p].begin();sit!=f[p].end();sit++)
{
   int i=0;
   set<string>::iterator sitin;
   tempv.clear();
   for(datait=inputdata[p].begin(),satit=inputdata[4].begin();datait!=inputdata[p].end();datait++,satit++)
   {
                if(*datait==*sit)
                {
                   //cout<<*satit<<" ";
                   tempv.push_back(*satit);
                }
   }
   
   for(sitin=f[4].begin();sitin!=f[4].end();sitin++)
   {
      int cc=count(tempv.begin(),tempv.end(),*sitin);
          cout<<p<<":"<<*sit<<" "<<*sitin<<" "<<cc<<endl;
   }

}
}

/*
cout<<endl;
vector<string>::iterator ittemp;
for(ittemp=tempv.begin();ittemp!=tempv.end();ittemp++)
{
   cout<<*ittemp<<" ";
}

*/


/*
for(int i=0;i<FILEDNUM;i++)
{
        for(s=f[i].begin();s!=f[i].end();s++)
    {
                cout<<*s<<" "; 
        }
    cout<<endl;
}  
cout<<endl;

for(int i=0;i<FILEDNUM;i++)
{
   for(idit=inputdata[i].begin();idit!=inputdata[i].end();idit++)
   {
      cout<<*idit<<" "; 
   }
   cout<<endl;

}
*/
 //free(buf);
 return 0;

}

void make_filter()
{
   
}

void load_cfg()
{
   FILE *fp;
   char *buf;
   char *key;
   char *val;
   buf=(char*)malloc(200*sizeof(char));

   fp=fopen("id3conf.txt","r");
   while(fgets(buf,1024,fp)!=NULL)
   {
             key=strsep(&buf,"=");
         val=strsep(&buf,"\n");
         cfg[key]=val;
   }
}
