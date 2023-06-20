for $VERSION in `cat versions`
do
  echo $VERSION
  ./suite-run.sh $VERSION
done
