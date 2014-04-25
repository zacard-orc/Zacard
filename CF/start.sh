hadoop dfs -put cfraw.txt  /user/root/cfraw.txt
hadoop dfs -put cfcfg.txt  /user/root/cfcfg.txt
hadoop dfs -put aa035_m.o  aa035_m.o
hadoop dfs -put aa035_r.o  aa035_r.o
hadoop jar $HADOOP_HOME/contrib/streaming/hadoop-streaming-1.2.1.jar  \
>    -input cfraw.txt  \
>    -output  cfraw \
>    -mapper ./aa035_m.o \
>    -reducer ./aa035_r.o  \
>    -file ./aa035_m.o -file aa035_r.o cfcfg.txt \
>    -jobconf mapred.reduce.tasks=1 \
>    -jobconf mapre.job.name="aaa" 
