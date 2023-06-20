INSTANCE=$1
POSTFIX=$2
TIMEOUT=3600

NAME=$(basename $INSTANCE)
BASE=${NAME%.*}

for SEED in `cat seeds`
do
  ../encoder/em-encode-$POSTFIX $INSTANCE $SEED | timeout $TIMEOUT cadical -f --no-walk | grep "c total p"
done
