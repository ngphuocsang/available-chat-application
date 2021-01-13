ROOT_DIR=$(pwd)
PRIMARY_TARGETS=$(find ./test/Primary -name "*.cpp" -execdir basename {} .cpp ';')
SERVICES_TARGETS=$(find ./test/Services -name "*.cpp" -execdir basename {} .cpp ';')
CALLINGAPIS_TARGETS=$(find ./test/CallingAPIs -name "*.cpp" -execdir basename {} .cpp ';')

cd $ROOT_DIR/build
for TARGET in $PRIMARY_TARGETS
do
	sudo cmake --build $ROOT_DIR/build --config Release --target $TARGET -- -j 1 &
done
wait
for TARGET in $SERVICES_TARGETS
do
	sudo cmake --build $ROOT_DIR/build --config Release --target $TARGET -- -j 1 &
done
wait
for TARGET in $CALLINGAPIS_TARGETS
do
	sudo cmake --build $ROOT_DIR/build --config Release --target $TARGET -- -j 1 &
done
wait

sudo cp $ROOT_DIR/cicd/appSetting.txt $ROOT_DIR/build/test/Primary/appSetting.txt
sudo cp $ROOT_DIR/cicd/appSetting.txt $ROOT_DIR/build/test/Services/appSetting.txt
sudo cp $ROOT_DIR/cicd/appSetting.txt $ROOT_DIR/build/test/CallingAPIs/appSetting.txt
sudo chown gitlab-runner:gitlab-runner $ROOT_DIR/build -R

mysql -uacauser -hacadbservice -pacapasswd acadb < $ROOT_DIR/cicd/aca-db.sql
cd $ROOT_DIR/build/test/Primary
ctest -j1 -C Release -T test --output-on-failure

mysql -uacauser -hacadbservice -pacapasswd acadb < $ROOT_DIR/cicd/aca-test-db-script.sql
cd $ROOT_DIR/build/test/Services
ctest -j1 -C Release -T test --output-on-failure
cd $ROOT_DIR/build/test/CallingAPIs
ctest -j1 -C Release -T test --output-on-failure

sudo chown gitlab-runner:gitlab-runner $ROOT_DIR/build -R
