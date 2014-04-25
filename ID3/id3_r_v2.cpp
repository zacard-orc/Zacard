//C++ ID3 REDUCER V2
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

//SYNTEX ANNONCE
typedef map<string,yynn> data;
typedef map<string,data> sub;
typedef map<int,float>  ccn;
map <string,string>  cfg;
set<string>          f[FILEDNUM];
float TOTAL,NODEP,ROUND,DEEP=0;
int NODEID;
sub rd;

//FUNCTION ANNONC
float make_epy(yynn stcom);
void  mk_gain(float parent,ccn gccn);
void  show_arr(set<string> art);
void  load_cfg();
void  show_all();
void  trainsub(string fitcond,float PNODEP);
void  runid3(string fitcond,float parprob);
string subrule(string parsub,int id,string fenum);

int main()
{
   char *buf=NULL;
   char *key=NULL;
   char *set_id;
   char *set_enum;
   char *att1,*att2;
   set<string> arr;
   buf=(char*)malloc(102400*sizeof(char));
   NODEP=1;
   

   while(fgets(buf, BUFFER_SIZE-1, stdin))
   {
     key=strsep(&buf,":");    //SUB KEY
         set_id=strsep(&buf,":");
         set_enum=strsep(&buf," ");
         att1=strsep(&buf," ");
         att2=strsep(&buf,"\n");

         switch(atoi(set_id)) 
         {
          case 0: 
                  {f[0].insert(set_enum);
                  break;}
          case 1:
                  {f[1].insert(set_enum);
           break;}
          case 2:
                  {f[2].insert(set_enum);
                   break;}
          case 3:
                  {f[3].insert(set_enum);
                   break;}
          default:
          cout<<"error"<<endl;
         }
         string s1(set_id);
         string s2(set_enum);
         string cond=s1+":"+s2;

          if(strcmp(att1,"no")==0)
          { 
                rd[key][cond].no=rd[key][cond].no+atof(att2);  
          } 
          else
          {
            rd[key][cond].yes=rd[key][cond].yes+atof(att2);
          }
          rd[key][cond].tot=rd[key][cond].yes+rd[key][cond].no;
          
   }
   

show_all();
//cout<<"AAA"<<endl;
runid3("NULL_NULL_NULL_NULL",1);

return 0;
}

void runid3(string fitcond,float parprob)
{
    trainsub(fitcond,parprob);
    if(NODEID==-1)
        {   cout<<"I'm returning"<<endl;
                return;}
        else
        {
    set<string>::iterator f_it;
        int newid=NODEID;
        float newprob=NODEP;
        DEEP++;
    cout<<"I'm go into For.The Deep:"<<DEEP<<endl;
        for(f_it=f[newid].begin();f_it!=f[newid].end();f_it++)
        {
      string sscond=subrule(fitcond,newid,*f_it);
          cout<<"ficond="<<sscond<<endl;
      runid3(sscond,newprob);
        }
        DEEP--;
    cout<<"I'm out of For.The Deep:"<<DEEP<<endl;
        return;
    }
}


string subrule(string parsub,int id,string fenum)
{
   char *cc;
   char *f[FILEDNUM-1];
   int len = parsub.length();
   cc = new char[len+1];
   strcpy(cc,parsub.c_str());
   
   strcpy(cc,parsub.c_str());
   f[0]=strsep(&cc,"_");
   f[1]=strsep(&cc,"_");
   f[2]=strsep(&cc,"_");
   f[3]=strsep(&cc,"\n");

   for(int i=0;i<FILEDNUM-1;i++)
   {
      if(i==id)
          { len =fenum.length();
             f[i]=new char[len+1];
                 strcpy(f[i],fenum.c_str());}
   }
   string res;
   for(int i=0;i<FILEDNUM-1;i++)
   {
     string temp(f[i]);
         if(i<FILEDNUM-2)
          res=res+temp+"_";
         else
          res=res+temp;
   }
   return res;
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

void mk_gain(float parent,ccn gccn)
{

   int maxd;
   float maxg,f,nexroot,sum=0;

   maxg=0;
   for(int i=0;i<FILEDNUM-1;i++)
   {
           f=parent-gccn[i];
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
   NODEP=nexroot;
   NODEID=maxd;
   cout<<"NODEID="<<NODEID<<endl;
   cout<<"NODEP="<<NODEP<<endl;
  }
  else
  {
   cout<<"NODEID=-1"<<endl;    
   cout<<"NODEP=-1"<<endl;
   NODEP=-1;
   NODEID=-1;
  }

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

void show_all()
{

        set<string>::iterator f_it;
        string a="0:";
        for(f_it=f[0].begin();f_it!=f[0].end();f_it++)
        {
            string b=a+*f_it;  
            TOTAL=rd["NULL_NULL_NULL_NULL"][b].tot+TOTAL;
        }
        //cout<<left<<setw(40)<<"att"<<"\t"<<"yes"<<"\t"<<"no"<<"\t"<<"tot"<<"\t"<<"eny"<<"\t\t"<<"pp"<<endl;
        sub::iterator sub_it;
        for(sub_it=rd.begin();sub_it!=rd.end();sub_it++)
        {
           data::iterator data_it;
           cout<<sub_it->first<<endl;
           for(data_it=sub_it->second.begin();data_it!=sub_it->second.end();data_it++)
           {
              yynn tem=data_it->second;
              rd[sub_it->first][data_it->first].eny=make_epy(tem);
              rd[sub_it->first][data_it->first].pp=(tem.tot/TOTAL)*rd[sub_it->first][data_it->first].eny;
              yynn temp=data_it->second;
                  cout<<left<<setw(40)<<data_it->first<<"\t"<<temp.yes<<"\t"<<temp.no<<"\t"<<temp.tot<<"\t"<<setw(8)<<temp.eny<<"\t"<<temp.pp<<endl;
       }
        }
}

void trainsub(string fitcond,float PNODEP)
{
        sub::iterator sub_it=rd.find(fitcond);
        data::iterator data_it;
    //cout<<"====================================================="<<endl;
        char buf[30];
        ccn ccns;
        for(int i=0;i<FILEDNUM-1;i++)
        {
        float epy=0;
        for(data_it=sub_it->second.begin();data_it!=sub_it->second.end();data_it++)
        {
                        sscanf(data_it->first.c_str(),"%[^:]",buf);
                        if(atoi(buf)==i)
                    {
                       epy=epy+rd[sub_it->first][data_it->first].pp; 
                    }
                }
                //cout<<sub_it->first<<":"<<i<<":"<<epy<<endl;
                ccns.insert(pair<int,float> (i,epy));
        }
        mk_gain(PNODEP,ccns);
        cout<<"SUBFILE="<<sub_it->first<<endl<<endl;
} 
