POSTFIX=$1

for file in `ls -d ../benchmarks/*.txt`
do
  NAME=$(basename $file)
  BASE=${NAME%.*}
  echo $BASE
  ./sls.sh $file $POSTFIX > ../log/sls-$BASE-$POSTFIX.log
done
