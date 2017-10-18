cmake_minimum_required(VERSION 3.1)
project(GeneticAlgo)


set(HEADER_FILES)

set(SOURCE_FILES
	main.cpp)

set(EXTRA_FILES
    shapegenerator.py

    Makefile
    geneticalgo/shapewriter.py
    geneticalgo/generate_expression.py
    geneticalgo/algorithm.py
    geneticalgo/pyxsrc/primitives.pyx
    geneticalgo/pyxsrc/primitives.pxd
    geneticalgo/setup.py

    scenes/accordion/main.py
    scenes/accordion/accordionutils.py
    scenes/accordion/scene.pyscn
    scenes/accordion/controller.py
    scenes/accordion/customfield.py
    scenes/accordion/shape.py
    scenes/accordion/shapeinv.py

    utils/script.sh
    utils/script.py
    )


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})


