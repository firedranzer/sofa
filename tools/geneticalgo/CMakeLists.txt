cmake_minimum_required(VERSION 3.1)
project(GeneticAlgo)


set(HEADER_FILES)

set(SOURCE_FILES

        scenes/accordion/main.cpp)

set(EXTRA_FILES
    shapegenerator.py

    Makefile
    geneticalgo/shapewriter.py
    geneticalgo/generate_expression.py
    geneticalgo/algorithm.py

    geneticalgo/pyxsrc/distance.pyx
    geneticalgo/pyxsrc/distance.pxd
    geneticalgo/pyxsrc/primitives.pyx
    geneticalgo/pyxsrc/primitives.pxd
    geneticalgo/pyxsrc/primitives2D.pyx
    geneticalgo/pyxsrc/primitives2D.pxd
    geneticalgo/pyxsrc/piecewisePolynom2D.pyx
    geneticalgo/pyxsrc/piecewisePolynom2D.pxd
    geneticalgo/setup.py

    scenes/accordion/main.py
    scenes/accordion/accordionutils.py
    scenes/accordion/scene.pyscn
    scenes/accordion/controller.py
    scenes/accordion/customfield.py
    scenes/accordion/shape.py


    scenes/crochet/main.py
    scenes/crochet/crochetutils.py
    scenes/crochet/scene.pyscn
    scenes/crochet/controller.py
    scenes/crochet/customfield.py
    scenes/crochet/shape.py
    scenes/crochet/shapeinv.py

#    scenes/SOFIA/main.py
#    scenes/SOFIA/SOFIAutils.py
    scenes/SOFIA/scene.pyscn
    scenes/SOFIA/controller.py
    scenes/SOFIA/customfield.py
    scenes/SOFIA/SOFIA.py
    scenes/SOFIA/shape.py
#    scenes/SOFIA/shapeinv.py

#    scenes/SOFIA_SECOND_VERSION/main.py
#    scenes/SOFIA_SECOND_VERSION/SOFIAutils.py
    scenes/SOFIA_SECOND_VERSION/scene.pyscn
#    scenes/SOFIA_SECOND_VERSION/controller.py
#    scenes/SOFIA_SECOND_VERSION/customfield.py
    scenes/SOFIA_SECOND_VERSION/SOFIA.py
    scenes/SOFIA_SECOND_VERSION/shape.py
#    scenes/SOFIA_SECOND_VERSION/shapeinv.py

    utils/script.sh
    utils/script.py
    )


add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})


