source ~/.bash_profile

#//Define Syntax Area
ORG_USERPWD=kfinfo/kfinfo@rpt09
ORG_OBJFILE='/root/tt/table.conf'
DES_HOST='10.10.111.88'
DES_USER='kfinfo'
DES_PAWD='kfinfo'
TMPFILE='STRCT.TEMP'
MKTMPFILE='mktb.sql'
MKINDFILE='mkind.sql' 
CC=1


#//Define Function Merge Process
function mktable()
{
sqlplus ${ORG_USERPWD} << SQLSCOPE
set termout off
set heading off
set head off
set long 1500
set feedback off
spool ${TMPFILE}
select dbms_metadata.get_ddl('TABLE','$2','$1') from dual;
spool off
quit;
SQLSCOPE

cat $TMPFILE|grep -E "CREATE TABLE|DATE|VARCHAR2|VARCHAR|NUMBER"|tr '[A-Z]' '[a-z]'|sed 's/date/datetime/g'|sed 's/number/float/g'|sed 's/varchar2/varchar/g'|sed 's/"//g'|sed 's/create table/create table IF NOT EXISTS/g'  > $MKTMPFILE
echo ") ENGINE=InnoDB DEFAULT CHARSET=gbk;"|tr '[A-Z]' '[a-z]' >> $MKTMPFILE

cat $MKTMPFILE
mysql -h$DES_HOST -u$DES_USER -p$DES_PAWD < $MKTMPFILE
}

function mkindex()
{
sqlplus ${ORG_USERPWD} << SQLSCOPE
set termout off
set heading off
set head off
set long 200
set feedback off
spool ${TMPFILE}
select 'INDEX_NAME:'||b.column_name||';'||a.index_type||';'||a.uniqueness from  (
SELECT u.index_name,u.index_type,u.uniqueness FROM user_indexes u where u.table_name='$2' and u.table_owner='$1'
) a,
(
select d.index_name,d.column_name from dba_ind_columns d where d.table_name='$2' and d.table_owner='$1'
) b
where a.index_name=b.index_name;
spool off
quit;
SQLSCOPE

cat $TMPFILE|grep 'INDEX_NAME'|egrep -v 'select'|awk -F ':' '{print $2}'|sed 's/ //g'|tr '[A-Z]' '[a-z]' > INDENUM.TXT
for i in `cat INDENUM.TXT`
do
    if [ -n $i ]
    then
        V3=`echo $i|awk -F ';' '{print $3}'`
        V2=`echo $i|awk -F ';' '{print $2}'`
        V1=`echo $i|awk -F ';' '{print $1}'`
        if [ $V3 = "nonunique" ]
        then
                TB=`echo $2|tr '[A-Z]' '[a-z]'`
                UN=`echo $1|tr '[A-Z]' '[a-z]'`
                echo "create index $V1"_ind_"$CC USING BTREE ON $UN.$TB($V1);"  > $MKINDFILE
                cpp
                mysql -h$DES_HOST -u$DES_USER -p$DES_PAWD < $MKINDFILE
        else
                TB=`echo $2|tr '[A-Z]' '[a-z]'`
                UN=`echo $1|tr '[A-Z]' '[a-z]'`
                echo "alter table $2 add unique index $V1"_indpk_"$CC ($V1);" > $MKINDFILE
                cpp 
                mysql -h$DES_HOST -u$DES_USER -p$DES_PAWD < $MKINDFILE
        fi
    fi
done

}

function cpp()
{
  CC=`expr $CC + 1 `
}


#-----------------Main Body----------------------------------------------------
for i in `cat $ORG_OBJFILE`
do
   ORG_TBUSER=`echo $i|awk -F '.' '{print $1}'|tr '[a-z]' '[A-Z]'`
   ORG_TBNAME=`echo $i|awk -F '.' '{print $2}'|tr '[a-z]' '[A-Z]'`
   mktable $ORG_TBUSER $ORG_TBNAME
   mkindex $ORG_TBUSER $ORG_TBNAME
done
