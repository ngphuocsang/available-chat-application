REGISTRY_SERVER=192.168.122.238:5000
docker-compose build
docker tag acadbservice:latest $REGISTRY_SERVER/acadbservice:latest
docker tag chatservservice:latest $REGISTRY_SERVER/chatservservice:latest
docker tag restservservice:latest $REGISTRY_SERVER/restservservice:latest
docker push $REGISTRY_SERVER/acadbservice:latest
docker push $REGISTRY_SERVER/chatservservice:latest
docker push $REGISTRY_SERVER/restservservice:latest
if [[ "" != $(docker images -f "dangling=true" -q) ]]; then
    docker rmi $(docker images -f "dangling=true" -q)
fi
