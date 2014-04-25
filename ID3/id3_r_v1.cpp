//C++ ID3 REDUCER
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <set>
#include <stdio.h>

using namespace std;

#define BUFFER_SIZE 10240
#define DELIM       " "
#define FILEDNUM 5

struct yynn
{
float yes;
float no;
float tot;
float eny;
float pp;
void def() {yes=0;no=0;tot=0;eny=0;pp=0;};
};

typedef map<string,yynn> data;
typedef map<int,float>  ccn;
map <string,string>  cfg;
float TOTAL,ROOTPROB;

float make_epy(yynn stcom);
void  make_gain(ccn gccn);
void  show_arr(set<string> art);
void  load_cfg();


int main()
{
   load_cfg();
   char *buf=NULL;
   char *key=NULL;
   char *att1,*att2;
   set<string> arr;
   buf=(char*)malloc(10240*sizeof(char));
   data rd;
   ccn  ccns;
   data::iterator data_it;

   while(fgets(buf, BUFFER_SIZE-1, stdin))
   {
      key=strsep(&buf,DELIM);   
          att1=strsep(&buf,DELIM);
          att2=strsep(&buf,"\n");
          arr.insert(key); 
          if(strcmp(att1,"no")==0)
          { 
                rd[key].no=rd[key].no+atof(att2);  
          } 
          else
          {
            rd[key].yes=rd[key].yes+atof(att2);
          }
          rd[key].tot=rd[key].yes+rd[key].no;
   }

//DEFINE TOTAL
TOTAL=rd["root"].tot;

//DISPLAY & COMPUTER
cout<<setw(10)<<"att"<<"\t"<<"yes"<<"\t"<<"no"<<"\t"<<"tot"<<"\t"<<"eny"<<"\t\t"<<"pp"<<endl;
   
for(data_it=rd.begin();data_it!=rd.end();data_it++)
{
   yynn tem=data_it->second;
   rd[data_it->first].eny=make_epy(tem);
   rd[data_it->first].pp=(tem.tot/TOTAL)*rd[data_it->first].eny;
   yynn temp=data_it->second;
   cout<<setw(10)<<data_it->first<<"\t"<<temp.yes<<"\t"<<temp.no<<"\t"<<temp.tot<<"\t"<<setw(8)<<temp.eny<<"\t"<<temp.pp<<endl;
}

cout<<"rootprob"<<rd["root"].pp;
//DEFINE ROOTPROB BY REFERANCE
if(cfg["NODEID"]=="UDF")
{  ROOTPROB=rd["root"].pp; }
else
{  ROOTPROB=atof(cfg["NODEP"].c_str());}


//COMPUTER PROBILTIY
for(int i=0;i<FILEDNUM-1;i++)
{
        float epy=0;
        for(data_it=rd.begin();data_it!=rd.end();data_it++)
        {
     sscanf(data_it->first.c_str(),"%[^:]",buf);
     if(atoi(buf)==i && strcmp(data_it->first.c_str(),"root"))
         {
           epy=epy+rd[data_it->first].pp; 
         }
        }
        ccns.insert(pair<int,float> (i,epy));
}

make_gain(ccns);
show_arr(arr);

return 0;

}

float make_epy(yynn stcom)
{
   float res;
   float pyes=stcom.yes/stcom.tot;
   float pno=stcom.no/stcom.tot;
   res=-pyes*log(pyes)/log(float(2))-pno*log(pno)/log(float(2));
   if(isnan(res))
   {
            return 0;
   }else
   {
                return res;
   }
}

void make_gain(ccn gccn)
{
   int maxd;
   float maxg,f,nexroot,sum=0;

   maxg=0;
   //cout<<endl;
   for(int i=0;i<FILEDNUM-1;i++)
   {
           f=ROOTPROB-gccn[i];
           if(f>maxg)
           {
             maxg=f; 
                 maxd=i;
                 nexroot=gccn[i];
           }
          sum=sum+gccn[i];
   } 
  if(sum!=0) 
  {
   cout<<"NODEID="<<maxd<<endl;
   cout<<"NODEP="<<nexroot<<endl;
  }
  else
  {
   cout<<"NODEID=NULL"<<endl;    
   cout<<"NODEP=NULL"<<endl;
  }
  cout<<"FILE="<<cfg["FILE"]<<endl;
   //cout<<"NODE="<<maxd<<":"<<maxg<<":"<<nexroot<<endl;

}

void show_arr(set<string> art)
{
   set<string>::iterator art_it;
   if(cfg["ROUND"]=="0")
   {
   for(art_it=art.begin();art_it!=art.end();art_it++)
   {
     if(*art_it!="root")
     cout<<"COND"<<*art_it<<endl; 
   }
   }

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
