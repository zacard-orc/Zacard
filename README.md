Zacard  
======  

A MachineLearn SourceFile on MapReduce C++  .


1,ID3  
======  
Two edition of ID3 Algorithm base on MapReduce V1.The code of V1 is based on C++&SHELL.The code of V2 is based on C++ purely.  
V1 ID3 is a traditional algorithm.It depend on shell to filter raw text for more times.  
V2 ID3 is a new logic implementation.It depend on Tasktracker to do more things,so it computer the all combination for every Conditions.It's goal is to ease Reduecer's pressure.  

You can run V1,by command below:  
[root@cent3 tt]# runid3.sh  
but also ,you should make the id3conf.txt first like this:  
[root@cent3 tt]# more id3conf.txt  
NODEID=UDF  
NODEP=UDF  
FILE=id3raw.txt  
ROUND=0  

Your can run V2,by commadn below:  
  hadoop jar $HADOOP_HOME/contrib/streaming/hadoop-  streaming-1.2.1.jar  \  
   -input id3raw.txt  \  
   -output  id3_out \  
   -mapper ./aa026_m.o \  
   -reducer ./aa026_r.o  \  
   -file ./aa026_m.o -file ./aa026_r.o  \  
   -jobconf mapred.reduce.tasks=1 \  
   -jobconf mapre.job.name="aaa" \  
   
So,I hope you will enjoy them and give more suggestions~~ ^_^  

2,CF      
======    
Your can run V2,by commadn below:  
  hadoop jar $HADOOP_HOME/contrib/streaming/hadoop-  streaming-1.2.1.jar  \    
   -input cfraw.txt  \    
   -output  id3_out \    
   -mapper ./aa035_m.o \    
   -reducer ./aa035_r.o  \    
   -file ./aa035_m.o -file ./aa035_r.o ./cfcfg.txt \    
   -jobconf mapred.reduce.tasks=1 \    
   -jobconf mapre.job.name="aaa" \    
    
So,I hope you will enjoy them and give more suggestions~~ ^_^  
  


3,DB2DB  
=======  
A tool for export the ddl of oracle table to mysql db  

1,You should configure the object of table into the "table.  conf".Just like 'kfinfo.db_jerh_ob_cph'.  
2,You can change the globe variable in the head of shell script.Just like OGR_TABLE,DES_TABLE ,etc.  
3,run the sciprt. ./db2db.sh.  
4,OK.  


This script is only for normal table,normal index and PrimayKey.The Job for support Partition table is also in develop.  
