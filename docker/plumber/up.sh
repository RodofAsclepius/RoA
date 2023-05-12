rm -Rf /tmp/asclepius
mkdir /tmp/asclepius
cp -Rf api /tmp/asclepius

docker kill asclepiusdocker
docker rm asclepiusdocker
docker run --network host --rm --name asclepiusdocker -v /tmp/asclepius/api:/api asclepius