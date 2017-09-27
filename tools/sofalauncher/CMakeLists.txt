cmake_minimum_required(VERSION 3.1)
project(sofalauncher)


set(HEADER_FILES)

set(SOURCE_FILES
	main.cpp
	)

set(EXTRA_FILES
	README.txt
	launcher.py
	sofa-launcher.py
        integration_example.py
        example.py
        example.scn
    )


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})



