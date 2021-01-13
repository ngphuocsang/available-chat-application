ROOT_DIR=$(pwd)
sudo rm $ROOT_DIR/build -R -f 
sudo mkdir $ROOT_DIR/build -p
cd $ROOT_DIR/build
sudo cmake ..
sudo cmake --build $ROOT_DIR/build --config Release --target ACA-RestServer -- -j 6
sudo cmake --build $ROOT_DIR/build --config Release --target ACA-ChatServer -- -j 6
sudo cp $ROOT_DIR/cicd/appSetting.txt $ROOT_DIR/build/source/Server/appSetting.txt
sudo chown gitlab-runner:gitlab-runner $ROOT_DIR/build -R