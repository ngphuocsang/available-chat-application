# mysql -uacauser -hacadbservice -pacapasswd acadb < aca-db.sql
microk8s kubectl delete -f kubernetes.database.yml
microk8s kubectl apply -f kubernetes.database.yml
microk8s kubectl apply -f kubernetes.restserv.yml
microk8s kubectl apply -f kubernetes.chatserv.yml