ROOT_DIR=$(pwd)
STRESSTEST_TARGETS=$(find ./test/StressTest -name "*.cpp" -execdir basename {} .cpp ';')

cd $ROOT_DIR/build
for TARGET in $STRESSTEST_TARGETS
do
	sudo cmake --build $ROOT_DIR/build --config Release --target $TARGET -- -j 1 &
done
wait

sudo cp $ROOT_DIR/cicd/appSetting.txt $ROOT_DIR/build/test/StressTest/appSetting.txt
sudo chown gitlab-runner:gitlab-runner $ROOT_DIR/build -R

mysql -uacauser -hacadbservice -pacapasswd acadb < $ROOT_DIR/cicd/aca-db.sql
cd $ROOT_DIR/build/test/StressTest
ctest -j1 -C Release -T test --output-on-failure

sudo chown gitlab-runner:gitlab-runner $ROOT_DIR/build -R
