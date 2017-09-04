#include <string>
#include <vector>
#include <SofaTest/Sofa_test.h>
using sofa::defaulttype::Vec3d ;

#include <SofaImplicitField/components/geometry/ScalarField.h>
using sofa::component::geometry::ScalarField ;

#include <SofaImplicitField/components/geometry/SphericalField.h>
using sofa::component::geometry::SphericalField ;

#include <SofaImplicitField/components/geometry/CustomField.h>
#include <sofa/core/objectmodel/BaseObject.h>
using sofa::component::geometry::CustomField ;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node ;
using sofa::core::ExecParams;
using sofa::core::objectmodel::BaseData ;

#include <SofaSimulationCommon/SceneLoaderXML.h>
using sofa::simulation::SceneLoaderXML ;

namespace
{

class CustomFieldTest : public sofa::Sofa_test<>,
        public testing::WithParamInterface<std::vector<std::string>>
{
public:
    void checkConstructor() ;
    void checkInit(const std::vector<std::string>& params) ;
};

class CustomFieldCheckValues : public sofa::Sofa_test<>,
        public testing::WithParamInterface<std::vector<std::string>>
{
public:
    void checkValue(const std::vector<std::string>& params) ;
};


void CustomFieldTest::checkConstructor()
{
    std::string scene1 =
            "<?xml version='1.0'?>"
            "<Node 	name='Root' gravity='0 0 0' time='0' animate='0'   >       \n"
            "         <RequiredPlugin name='SofaImplicitField'/>               \n"
            "         <CustomField name='field1' printLog='1'/>                \n"
            "</Node>                                                           \n" ;

    Node::SPtr root = SceneLoaderXML::loadFromMemory ("testscene",
                                                      scene1.c_str(),
                                                      scene1.size()) ;
    ASSERT_NE(root.get(), nullptr) ;
    {
        EXPECT_MSG_EMIT(Error) ;
        root->init(ExecParams::defaultInstance()) ;
    }
}

void CustomFieldTest::checkInit(const std::vector<std::string>& params)
{
    ASSERT_EQ(params.size(), (uint)2);

    std::string function = params[0] ;
    std::stringstream scene1 ;
    scene1 <<
              "<?xml version='1.0'?>"
              "<Node 	name='Root' gravity='0 0 0' time='0' animate='0'   >       \n"
              "         <RequiredPlugin name='SofaImplicitField'/>               \n"
              "         <CustomField function='"<<function<<"'/>                 \n"
                                                            "</Node>                                                           \n" ;

    Node::SPtr root = SceneLoaderXML::loadFromMemory ("testscene",
                                                      scene1.str().c_str(),
                                                      scene1.str().size()) ;
    ASSERT_NE(root.get(), nullptr) ;
    if(params[1].empty()){
        root->init(ExecParams::defaultInstance()) ;
    }else if(params[1]=="Error"){
        EXPECT_MSG_EMIT(Error) ;
        root->init(ExecParams::defaultInstance()) ;
    }

    CustomField* field = root->getNodeObject<CustomField>() ;
    ASSERT_NE(field, nullptr) ;

    BaseData* fct = field->findData("function") ;
    ASSERT_NE(fct, nullptr) ;
}

void CustomFieldCheckValues::checkValue(const std::vector<std::string>& params)
{
    ASSERT_EQ(params.size(), (uint)4) ;
    std::string function = params[0] ;
    std::string gradient = params[1] ;
    std::stringstream scene1 ;
    scene1 <<
              "<?xml version='1.0'?>"
              "<Node 	name='Root' gravity='0 0 0' time='0' animate='0'   >       \n"
              "         <RequiredPlugin name='SofaImplicitField'/>               \n"
              "         <CustomField printLog='1' function='"<<function<<"'";
    if(!gradient.empty())
        scene1 << " gradient='"<< gradient <<"'/> \n" ;
    else
        scene1 << "/> \n" ;

    scene1 << "</Node>                                                           \n" ;

    Node::SPtr root = SceneLoaderXML::loadFromMemory ("testscene",
                                                      scene1.str().c_str(),
                                                      scene1.str().size()) ;

    ASSERT_NE(root.get(), nullptr) ;
    CustomField* field = root->getNodeObject<CustomField>() ;
    ASSERT_NE(field, nullptr) ;

    if(params[3]=="Error"){
        EXPECT_MSG_EMIT(Error) ;
        root->init(ExecParams::defaultInstance()) ;

        double x,y,z;
        std::stringstream(params[2]) >> x >> y >> z;
        Vec3d t(x,y,z);
        field->getValue(t) ;
        field->getGradient(t) ;
    }
    else
    {
        root->init(ExecParams::defaultInstance()) ;
        double x,y,z,res;
        std::stringstream(params[2]) >> x >> y >> z;
        std::stringstream(params[3]) >> res ;
        Vec3d t(x,y,z);
        double result = field->getValue(t) ;
        field->getGradient(t) ;
        EXPECT_EQ(result, res) ;
    }
}


TEST_F(CustomFieldTest, checkConstructor)
{
    checkConstructor();
}

std::vector<std::vector<std::string>> possibleInput = {
    {"testmodule.sphereFunction", ""},
    {"testmodule.notAFunction", "Error"},
    {"testmodule.invalid", "Error"},
    {"testmodule.invalid.other", "Error"},
    {"", "Error"},
    {"testmodule.", "Error"},
    {".function", "Error"},
    {"testmodule", "Error"}
};


TEST_P(CustomFieldTest, checkInit)
{
    checkInit(GetParam());
}

INSTANTIATE_TEST_CASE_P(checkInit,
                        CustomFieldTest,
                        ::testing::ValuesIn(possibleInput));


std::vector<std::vector<std::string>> possibleValues = {
    {"testmodule.zeroFunction", "testmodule.gradFunction", "1 2 3", "0"},
    {"testmodule.oneFunction", "testmodule.gradFunction", "1 3 3", "1"},
    {"testmodule.xFunction", "testmodule.gradFunction", "1 2 3", "1"},
    {"testmodule.yFunction", "testmodule.gradFunction", "1 2 3", "2"},
    {"testmodule.zFunction", "testmodule.gradFunction", "1 2 3", "3"},
    {"testmodule.sphereFunction", "testmodule.gradFunction", "0 0 0", "-1.0"},

    {"testmodule3.evalFunction", "testmodule2.gradFunction", "1 2 3", "Error"},
    {"testmodule.zeroFunction", "testmodule.gradFunctionInvalid1", "1 2 3", "Error"},
    {"testmodule.zeroFunction", "testmodule.gradFunctionInvalid2", "1 3 3", "Error"},
    {"testmodule.zeroFunction", "testmodule.gradFunctionInvalid3", "1 2 3", "Error"},
    {"testmodule.zeroFunction", "testmodule.gradFunctionInvalid4", "1 2 3", "Error"},
    {"testmodule.zeroFunction", "DD", "1 2 3", "Error"}
};

TEST_P(CustomFieldCheckValues, checkValue)
{
    checkValue(GetParam());
}

INSTANTIATE_TEST_CASE_P(checkValue2,
                        CustomFieldCheckValues,
                        ::testing::ValuesIn(possibleValues));



}


