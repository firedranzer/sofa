rm -rf ng/intree/kernel/*
rm -rf ng/intree/framework/*

sofa2ng/tools/mc.py sofa2ng/recipies/bootstrap-migration.rcpy ./ ng/intree
sofa2ng/tools/mc.py sofa2ng/recipies/Sofa.Config/Sofa.Config.rcpy ./ ng/intree/kernel
#sofa2ng/tools/mc.py sofa2ng/recipies/Sofa.Type/Sofa.Type.rcpy ./ ng/kernel
sofa2ng/tools/mc.py sofa2ng/recipies/Sofa.Component.Utils/Sofa.Component.Utils.rcpy ./ ng/intree/framework

#### Prepare kernel
echo "cmake_minimum_required(VERSION 3.5)" > ng/intree/kernel/CMakeLists.txt
echo 'set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/kernel/CMakeLists.txt
echo 'set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/kernel/CMakeLists.txt
echo "add_subdirectory(Sofa.Config)" >> ng/intree/kernel/CMakeLists.txt
echo "add_subdirectory(Sofa.Config/tests)" >> ng/intree/kernel/CMakeLists.txt

#### Prepare framework
echo "cmake_minimum_required(VERSION 3.5)" > ng/intree/framework/CMakeLists.txt
echo 'set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/framework/CMakeLists.txt
echo 'set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/framework/CMakeLists.txt
echo "add_subdirectory(Sofa.Component.Utils)" >> ng/intree/framework/CMakeLists.txt


