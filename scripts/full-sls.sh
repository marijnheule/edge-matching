for VERSION in `cat versions`
do
  echo $VERSION
  ./suite-sls.sh $VERSION
done
