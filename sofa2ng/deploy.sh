rm -rf ng/intree/kernel/*
rm -rf ng/intree/modules/*

mkdir ng/intree/extlibs
mkdir ng/intree/plugins
mkdir ng/intree/packages


sofa2ng/tools/mc.py sofa2ng/recipies/bootstrap-migration.rcpy ./ ng/intree
sofa2ng/tools/mc.py sofa2ng/recipies/Sofa.Config/Sofa.Config.rcpy ./ ng/intree/kernel
#sofa2ng/tools/mc.py sofa2ng/recipies/Sofa.Type/Sofa.Type.rcpy ./ ng/kernel
sofa2ng/tools/mc.py sofa2ng/recipies/Sofa.Component.Utils/Sofa.Component.Utils.rcpy ./ ng/intree/modules

#### Prepare kernel
echo "cmake_minimum_required(VERSION 3.5)" > ng/intree/kernel/CMakeLists.txt
echo 'set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/kernel/CMakeLists.txt
echo 'set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/kernel/CMakeLists.txt
echo "add_subdirectory(Sofa.Config)" >> ng/intree/kernel/CMakeLists.txt
echo "add_subdirectory(Sofa.Config/tests)" >> ng/intree/kernel/CMakeLists.txt

#### Prepare modules
echo "cmake_minimum_required(VERSION 3.5)" > ng/intree/modules/CMakeLists.txt
echo 'set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/modules/CMakeLists.txt
echo 'set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake")' >> ng/intree/modules/CMakeLists.txt
echo "add_subdirectory(Sofa.Component.Utils)" >> ng/intree/modules/CMakeLists.txt


