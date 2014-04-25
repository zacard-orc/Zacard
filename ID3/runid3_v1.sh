JAVA_HOME=/opt/java1.6
MAPR_HOME=/opt/mapr
HT_HOME=/opt/hypertable/current
HBASE_HOME=/home/hadoop/hbase-0.94.12
HADOOP_HOME=/opt/hadoop
CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar

PATH=$HOME/bin:/root/tt:/root:/opt/hypertable/current/bin:/home/ht/capistrano-master/bin:/opt/hadoop/bin:$JAVA_HOME/bin:$MAPR_HOME/bin:$HBASE_HOME/bin:/opt/cassandra/bin:$PATH


ROUND=0

inicfg()
{
  echo NODEID="$1" >  id3conf.txt
  echo NODEP="$2"  >> id3conf.txt
  echo FILE="$3"   >> id3conf.txt
  echo ROUND="$4"  >> id3conf.txt

}

startmp()
{
delout
hadoop dfs -rm /user/root/$3
hadoop dfs -put $3  /user/root/$3
inicfg $1 $2 $3 $4
hadoop jar $HADOOP_HOME/contrib/streaming/hadoop-streaming-1.2.1.jar  \
   -input $3  \
   -output  id3_out \
   -mapper ./aa022_m.o \
   -reducer ./aa022_r.o  \
   -file ./aa022_m.o -file aa022_r.o id3conf.txt \
   -jobconf mapred.reduce.tasks=1 \
   -jobconf mapre.job.name="aaa" 
   
}

delout()
{
    
        hadoop dfs -rmr /user/root/id3_out
}
#startid3 UDF UDF id3raw.txt
#startid3 nodeid,nodep,file
startid3()
{
        echo $'\n'ROUND="$ROUND"
        startmp $1 $2 $3 $ROUND
    PAR_T=`hadoop dfs -cat /user/root/id3_out/part-00000|grep NODEID|awk -F '=' '{print $2}'|awk -F ' ' '{print $1}'`

        echo $'\n'NODEID="$PAR_T"
    if [ $PAR_T == "NULL" ]
        then
        echo "I'm going return,U'know 3th par is "$3 "and PAR3 is" $PAR3
            return 0;
    fi
    PAR1=`hadoop dfs -cat /user/root/id3_out/part-00000|grep NODEID|awk -F '=' '{print $2}'|awk -F ' ' '{print $1}'`
    PAR2=`hadoop dfs -cat /user/root/id3_out/part-00000|grep NODEP|awk -F '=' '{print $2}'|awk -F ' ' '{print $1}'`
        PAR3=`hadoop dfs -cat /user/root/id3_out/part-00000|grep FILE|awk -F '=' '{print $2}'|awk -F ' ' '{print $1}'`
   
        if [ $ROUND -eq 0 ]
        then
        NODEID_R0=$PAR1
                NODEP_R0=$PAR2
        FILE_R0=$PAR3
        fi

    echo $'\n'NODEID="$PAR1" 
        echo NODEP="$PAR2"
        echo FILE="$PAR3"
    ROUND=`expr $ROUND + 1`
        case $PAR1 in
        NULL)  return 0;;
        *)     
        cat id3cont.txt|grep COND$PAR1 > id3enum.txt
    for i in `cat id3enum.txt|awk -F ':' '{print $2}'`
        do
             echo ""
                 echo subset--------------$i
             cat $PAR3|grep $i > $PAR3"_"$i
             echo  $PAR1":"$PAR2":"$PAR3"_"$i
                 echo ""
         startid3 $PAR1 $PAR2 $PAR3"_"$i
        done
    echo "The For is end,the PAR3 is "$PAR3",the 3th is "$3
        PAR1=$NODEID_R0
        PAR2=$NODEP_R0
        PAR3=$FILE_R0
        ROUND=`expr $ROUND - 1`
        ;;
    esac
}



startid3 UDF UDF id3raw.txt
