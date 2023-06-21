# assumes that cadical is in the path

INSTANCE=$1
POSTFIX=$2
TIMEOUT=3600

NAME=$(basename $INSTANCE)
BASE=${NAME%.*}

for SEED in `cat seeds`
do
  ../encoder/em-encode-$POSTFIX $INSTANCE $SEED > tmp-$SEED-$$.cnf
  SIZE=`wc tmp-$SEED-$$.cnf | awk '{print $1}'`
  SIZE=$(($SIZE-1))
  cat tmp-$SEED-$$.cnf | awk '/p cnf/ {print "p cnf "$3" "'$SIZE'} / 0/ {print $0}' > cnf-$SEED-$$.cnf
  mv cnf-$SEED-$$.cnf tmp-$SEED-$$.cnf
  ubcsat -i tmp-$SEED-$$.cnf -alg ddfw -cutoff 100000000 | grep -e "TotalCPU" -e "   1 "
#  ubcsat -i tmp-$SEED-$$.cnf -alg ddfw
done
#wait
