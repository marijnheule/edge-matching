POSTFIX=$1

for file in `ls -d ../benchmarks/*.txt`
do
  NAME=$(basename $file)
  BASE=${NAME%.*}
  echo $BASE
  ./run.sh $file $POSTFIX > ../log/$BASE-$POSTFIX.log
done
