//C++ ID3 MAPPER  V2
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

//SYNTEX ANNONCE
set<string>                              f[FILEDNUM];                   // ENUM PER FIELD
vector<vector<string> >  inputdata(1000);    // RAW INPUT DATA
vector<string>           procdata[FILEDNUM];
map<string,string>       cfg;
vector<vector<string> >  fenum(200);
typedef map<string,int>  st;
typedef map<string,st>   tst;
int                                              RECORDNUM;
int                      ERECORDNUM;
int                      FFLAG[FILEDNUM-1];
string                   condx[4];


//FUNCTION ANNONCE
void fclear();
void filtersub(string cond[4]);
void matrix();
  
//MAIN
int main(int argc, char** argv)  
{  
      char *buf;
          char *cur = NULL;  
          buf=(char*)malloc(10240*sizeof(char));
          int n=0;

      while(fgets(buf, BUFFER_SIZE-1, stdin))  
             { 
                         inputdata[n].resize(5);
                         for(int i=0;i<FILEDNUM;i++)
                         {
                                if(i<FILEDNUM-1)
                            {
                                        cur = strsep(&buf,DELIM); 
                                        f[i].insert(cur);
                                        inputdata[n][i]=cur;
                                }
                                else
                                {
                                cur = strsep(&buf,"\n");
                                        f[i].insert(cur);
                                        inputdata[n][i]=cur;
                                }
                                f[i].insert("NULL");
                         }
                         n++;
                 }

RECORDNUM=n;
set<string>::iterator sit;


n=0;

for(sit=f[0].begin();sit!=f[0].end();sit++)
{
  set<string>::iterator sit1;
  for(sit1=f[1].begin();sit1!=f[1].end();sit1++)
  {
      set<string>::iterator sit2;
          for(sit2=f[2].begin();sit2!=f[2].end();sit2++)
      {
                set<string>::iterator sit3;
                for(sit3=f[3].begin();sit3!=f[3].end();sit3++)
                {
                        condx[0]=*sit;
                        condx[1]=*sit1;
                        condx[2]=*sit2;
                        condx[3]=*sit3;
                        n++;
                        filtersub(condx);
                }
          }
  }
}
ERECORDNUM=n;

}

void fclear()
{
  for(int i=0;i<FILEDNUM;i++)
          procdata[i].clear();
}

void filtersub(string cond[4])
{
  fclear();
  for(int i=0;i<RECORDNUM;i++)
  {
          for(int j=0;j<FILEDNUM-1;j++)
          {
              if(cond[j]=="NULL")
                  FFLAG[j]=1;
              else
              {
                      if (inputdata[i][j]==cond[j])
                     FFLAG[j]=1;
                  else
                     FFLAG[j]=0;
              }
      }
          int res=1;
      for(int j=0;j<FILEDNUM-1;j++)
              res=FFLAG[j]&res;
          if(res==1)
      {
         for(int j=0;j<FILEDNUM;j++)
                     procdata[j].push_back(inputdata[i][j]);
          }
   }
  matrix();
  
}

void matrix()
{
        set<string>::iterator sit;
        vector<string>::iterator datait;
        vector<string>::iterator satit;
        vector<string> tempv;

        for(int p=0;p<FILEDNUM-1;p++)
        {
                for(sit=f[p].begin();sit!=f[p].end();sit++)
                {
                        if(*sit!="NULL")
            {
                         int i=0;
                         set<string>::iterator sitin;
                         tempv.clear();
                     for(datait=procdata[p].begin(),satit=procdata[4].begin();datait!=procdata[p].end();datait++,satit++)
                     {
                       if(*datait==*sit)
                           {
                                   tempv.push_back(*satit);
                       }
                         }
             for(sitin=f[4].begin();sitin!=f[4].end();sitin++)
                     {
                          if(*sitin!="NULL")
                          {
                      int cc=count(tempv.begin(),tempv.end(),*sitin);
                          cout<<condx[0]<<"_"<<condx[1]<<"_"<<condx[2]<<"_"<<condx[3];
                          cout<<":"<<p<<":"<<*sit<<" "<<*sitin<<" "<<cc<<endl;
                          }
                     }
                        }
                }
        }
}
