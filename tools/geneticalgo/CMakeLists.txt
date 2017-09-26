cmake_minimum_required(VERSION 3.1)
project(GeneticAlgo)


set(HEADER_FILES)

set(SOURCE_FILES
	main.cpp
	)

set(EXTRA_FILES
    shapegenerator.py

    geneticalgo/display_shape.py
    geneticalgo/generate_expression.py
    geneticalgo/geneticAlgorithm.pxd
    geneticalgo/geneticAlgorithm.pyx
    geneticalgo/primitives.pxd
    geneticalgo/primitives.pyx
    geneticalgo/setupBasic.py
    geneticalgo/setupLitteral.py

    scenes/accordion/accordion.pxd
    scenes/accordion/accordion.pyx
    scenes/accordion/SurfacePressureConstraint.pyscn
    scenes/accordion/controller.py
    scenes/accordion/customfield.py

    utils/script.sh
    utils/script.py
    )


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})


