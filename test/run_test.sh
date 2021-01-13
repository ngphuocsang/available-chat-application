cd ./build/test/Services
ctest -j1 -C Release -T test --output-on-failure
cd ../CallingAPIs
ctest -j1 -C Release -T test --output-on-failure