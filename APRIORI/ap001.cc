#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std; 

typedef struct {                                                          //原始集合结构定义
   vector<string> ss;
} t_raw_jh;

typedef struct {                              //有效数据/统计集合定义 
   set<string> ss;
   int sup_num;
} t_raw_tj;


t_raw_jh   raw_c[50];                         //原始数据
map<set<string>,int> raw_cnt_base;            //原始元素统计集合
t_raw_tj   L[10];                             //洗出来的单集合组合
//set<set>   new_kc;                          //洗出来的排列对象集，作为下次扫描的催化剂
int raw_num=0;                                //原始数据计数器
int l_num=0;                                  //统计数据计算器
int raw_msup=2;                               //频繁项支持度

/* C[频繁set<set>]-> [Struct/raw_c] -> Map<set,int> -> L -> set<set> */

void CountEleBase(string line,const char* delim,int raw_num);   //1+pre,把一条元素洗到二维颗粒，同时统计group by count
void CountEleReal(set<set<string> > &cc);                                               //1+real,根据3的催化剂，扫描原始记录，同时统计group by count
int WashEleBase();                                                                                              //2,把MAP SORT[group by count]洗出大于支持度的对象集
set<set<string> > GetNewKc(int size);                                   //3,把洗出的对象集合，极限排列出所有可能，作为下次扫描的催化剂
void Display();                                                 //显示原始集合、单元素非排序group by count
void ClearTj();                                                                                             //把统计表给清空
void RunApriori(set<set<string> > &fc);                         //嵌套跑，当然也可以迭代WHILE跑，入参为频繁项


int main()
{
         std::ios::sync_with_stdio(false);
         string filename="./ap001.txt";
         string line;
         ifstream ifs;
         ifs.open(filename.c_str());
         
         while(getline(ifs,line))
         {
                 CountEleBase(line,",",raw_num);
                 raw_num++;
         }

        // Display();
    //得到第一组经过支持度过滤的C 
        set<set<string> > aaa;   //频繁项，根据有效支持元素得出的组合
        aaa=GetNewKc(WashEleBase());

        RunApriori(aaa);

}

void RunApriori(set<set<string> > &fc)
{
    if(l_num==1) return;
    CountEleReal(fc);
        fc.clear();
        fc=GetNewKc(WashEleBase());
        RunApriori(fc);
}

void CountEleReal(set<set<string> > &cc)
{
          //扫描记录集
          cout<<"\n-----扫描记录集-----"<<endl;
          //迭代器定义好，在该函数内之后会用到
          set<set<string> >::iterator ot_it;
          set<string>::iterator in_it;
          raw_cnt_base.clear();

          for(ot_it=cc.begin();ot_it!=cc.end();ot_it++)
          {
                 int map_cc=0; 
                 for(int i=0;i<raw_num;i++)
             {
                        vector<string>::iterator res; 
                        for(res=raw_c[i].ss.begin();res!=raw_c[i].ss.end();res++)
                        {
                                cout<<*res<<" ";
                        }
                        cout<<"|";
                        int map_in_cc=0;
                        for(in_it=(*ot_it).begin();in_it!=(*ot_it).end();in_it++)
                        {
                                cout<<*in_it<<" ";
                                res=find(raw_c[i].ss.begin(),raw_c[i].ss.end(),*in_it);
                                if(res!=raw_c[i].ss.end()) map_in_cc++;
                        }
                        if(map_in_cc==(*ot_it).size()) map_cc++;
                        cout<<"|"<<map_in_cc<<endl;
                 }
                 raw_cnt_base[*ot_it]=map_cc;
                 cout<<"-->"<<map_cc<<endl;
      }
}

set<set<string> > GetNewKc(int size)
{
           vector<string>::iterator ss_it;
           set<string> kk[size];
           set<set<string> >  new_kc;
           int pi=0;

           set<string>::iterator kk_it;
           //做两两极限排列组合
           for(int i=0;i<l_num;i++)
           {
               set<string> tmp=L[i].ss;
               for(int j=i+1;j<l_num;j++)
               {
                                for(kk_it=L[j].ss.begin();kk_it!=L[j].ss.end();kk_it++)
                                {
                                      tmp.insert(*kk_it); 
                            }
                        new_kc.insert(tmp);
                                tmp=L[i].ss;
           }
       }

           cout<<"有效支持元素 ---> C组合[频繁项]:"<<endl;
           set<set<string> >::iterator out_it;
           set<string>::iterator in_it;
           for(out_it=new_kc.begin();out_it!=new_kc.end();out_it++)
           {
                for(in_it=(*out_it).begin();in_it!=(*out_it).end();in_it++)
                {
                   cout<<*in_it<<",";
                }
                cout<<endl;
           }
           return  new_kc;
}



int WashEleBase()
{
        map<set<string>,int>::iterator raw_cnt_it;
    int i=0;
        l_num=0;
        ClearTj();
        for(raw_cnt_it=raw_cnt_base.begin();raw_cnt_it!=raw_cnt_base.end();raw_cnt_it++)
        {
           if(raw_cnt_it->second>=raw_msup)
           {
                   L[i].ss=raw_cnt_it->first;
                   L[i++].sup_num=raw_cnt_it->second;
                   l_num++;
           }
        }

        //Display
        cout<<"有效支持元素:"<<endl;
        set<string>::iterator ss_it;
        for(i=0;i<l_num;i++)
        {
                for(ss_it=L[i].ss.begin();ss_it!=L[i].ss.end();ss_it++)
                {
                        cout<<*ss_it<<",";
                }
                cout<<"|"<<L[i].sup_num<<endl;
        }
        return i;
}


void CountEleBase(string line,const char* delim,int raw_num)
{
                 char *p=NULL,*q=NULL;
                 p=const_cast<char*>(line.c_str());
                 while(p)
                 {
                         q=strsep(&p,",");
                         set<string> load_ss;
                         raw_c[raw_num].ss.push_back(q);
                         load_ss.insert(q); raw_cnt_base[load_ss]++;             
                 }
}

void Display()
{
         cout<<"----Print Map-------"<<endl;
         map<set<string>,int>::iterator raw_cnt_it;
         set<string>::iterator ss_it;
         for(raw_cnt_it=raw_cnt_base.begin();raw_cnt_it!=raw_cnt_base.end();raw_cnt_it++)
         {
                 for(ss_it=(raw_cnt_it->first).begin();ss_it!=(raw_cnt_it->first).end();ss_it++)
                 {
                        cout<<*ss_it<<" ";
                 }
                 cout<<":"<<raw_cnt_it->second<<endl;
         }

         //raw_cnt_base.clear();

         cout<<"----Print Raw-------"<<endl;
         vector<string>::iterator raw_c_it;
         for(int i=0;i<raw_num;i++)
     {
                 for(raw_c_it=raw_c[i].ss.begin();raw_c_it!=raw_c[i].ss.end();raw_c_it++)
                 {
                          cout<<*raw_c_it<<" ";
                 }
                 cout<<endl;
         }
}

void ClearTj()
{
        for(int i=0;i<10;i++)
        {
                L[i].sup_num=0;
                L[i].ss.clear();
        }

}
