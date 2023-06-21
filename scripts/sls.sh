# assumes that cadical is in the path

INSTANCE=$1
POSTFIX=$2
TIMEOUT=3600
DIR=/tmp/

NAME=$(basename $INSTANCE)
BASE=${NAME%.*}

for SEED in `cat seeds`
do
  ../encoder/em-encode-$POSTFIX $INSTANCE $SEED > $DIR/tmp-$SEED-$$.cnf
  SIZE=`wc $DIR/tmp-$SEED-$$.cnf | awk '{print $1}'`
  SIZE=$(($SIZE-1))
  cat $DIR/tmp-$SEED-$$.cnf | awk '/p cnf/ {print "p cnf "$3" "'$SIZE'} / 0/ {print $0}' > $DIR/cnf-$SEED-$$.cnf
  mv $DIR/cnf-$SEED-$$.cnf $DIR/tmp-$SEED-$$.cnf
  ubcsat -i $DIR/tmp-$SEED-$$.cnf -alg ddfw -cutoff 100000000 | grep -e "TotalCPU" -e "   1 " &
done
wait

rm $DIR/tmp-*-$$.cnf
