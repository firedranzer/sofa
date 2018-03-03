#include <SofaSimulationGraph/testing/BaseSimulationTest.h>
using sofa::helper::testing::BaseSimulationTest ;
using sofa::core::objectmodel::BaseObject ;
using sofa::core::objectmodel::Data ;

#include <sofa/core/DataEngine.h>
using sofa::core::DataEngine ;

#include <sofa/core/ObjectFactory.h>
using sofa::core::RegisterObject ;

#include <SofaSimulationGraph/SimpleApi.h>

/// It is a good idea to have a private namespace so that tests in the application does not risk
/// to mix each other.
namespace test
{

class ConstantValue : public BaseObject
{
public:
    SOFA_CLASS(ConstantValue, BaseObject) ;

    ConstantValue() : BaseObject(),
        m_value( initData(&m_value, 0.0f, "value", "Constant value") )
    {
        f_printLog.setValue(true) ;
    }

    ~ConstantValue(){}

    void setValue(float value){
        msg_info() << "Set value to " << value ;
        m_value.setValue(value) ;
    }

    Data<float> m_value ;
};

class BinaryOperator : public DataEngine
{
public:
    SOFA_CLASS(BinaryOperator, BaseObject) ;

    BinaryOperator() :
        m_input1( initData(&m_input1, 0.0f, "input1", "Data source 1") ),
        m_input2( initData(&m_input2, 0.0f, "input2", "Data source 2") ),
        m_value( initData(&m_value, 0.0f, "value", "Result of operation") )
    {}
    void init() {
        f_printLog.setValue(true) ;
        addInput(&m_input1) ;
        addInput(&m_input2) ;
        addOutput(&m_value) ;
    }

    void update()
    {
        dmsg_info() << "Update triggered" ;
        m_input1.getValue() ;
        m_input2.getValue() ;
        cleanDirty();

        dmsg_info() << "Got " << m_input1.getValue() << ", " << m_input2.getValue() ;

        m_value.setValue( m_input1.getValue() + m_input2.getValue() );
    }

    Data<float> m_input1 ;
    Data<float> m_input2 ;
    Data<float> m_value ;
};

class UnaryOperator : public DataEngine
{
public:
    SOFA_CLASS(UnaryOperator, BaseObject) ;

    UnaryOperator() :
        m_input( initData( &m_input, -1.0f, "input", "") ),
        m_value( initData( &m_value, -10.0f, "value", "") ){}
    void init() {
        f_printLog.setValue(true) ;
        addInput(&m_input) ;
        addOutput(&m_value) ;
    }

    void update()
    {
        dmsg_info() << "Update triggered" ;
        m_input.getValue() ;
        cleanDirty();
        dmsg_info() << "Got " << m_input.getValue() ;
        m_value.setValue( sqrt(m_input.getValue()) );
    }

    Data<float> m_input ;
    Data<float> m_value ;
};

int ConstantValueId = RegisterObject("").add<ConstantValue>();
int BinaryOperatorId = RegisterObject("").add<BinaryOperator>();
int UnarySquareId = RegisterObject("").add<UnaryOperator>();

class MyDataEngineTest : public BaseSimulationTest
{
public:
    bool doCheck()
    {
        SceneInstance s ;
        /// Calculus is sqrt(input1 + input2 + input1)
        auto input1 = sofa::simpleapi::createObject<ConstantValue>(s.root, {{"name", "input1"},
                                                                         {"value", "1.0"}}) ;
        auto input2 = sofa::simpleapi::createObject<ConstantValue>(s.root, {{"name", "input2"},
                                                                         {"value", "3.0"}}) ;
        auto input3 = sofa::simpleapi::createObject<ConstantValue>(s.root, {{"name", "input3"},
                                                                         {"value", "40.0"}}) ;
        auto add1 = sofa::simpleapi::createObject<BinaryOperator>(s.root, {{"name", "add"},
                                                                          {"input1", "@input1.value"},
                                                                          {"input2", "@input2.value"}}) ;
        auto add2 = sofa::simpleapi::createObject<BinaryOperator>(s.root, {{"name", "add2"},
                                                                          {"input1", "@input1.value"},
                                                                          {"input2", "@add.value"}}) ;
        auto add3 = sofa::simpleapi::createObject<BinaryOperator>(s.root, {{"name", "add3"},
                                                                          {"input1", "@input3.value"},
                                                                          {"input2", "@add2.value"}}) ;

        auto sqrtt = sofa::simpleapi::createObject<UnaryOperator>(s.root, {{"name", "sqrt"},
                                                                           {"input", "@add3.value"}}) ;

        s.initScene() ;

        /// TEST 1: Change the top level inputs...recomputa everything if sqrt is queried
        msg_info("TEST1") << "----------- the full graph should be displayed ---------" ;
        input1->setValue(8);
        input2->setValue(8);
        sqrtt->m_value.getValue();
        msg_info("TEST1") << "sqrt(" << input1->m_value.getValue() << "+" << input2->m_value.getValue()
                           << "+" << input1->m_value.getValue() << ") = " << sqrtt->m_value.getValue() ;

        /// TEST 2: Change the top level inputs...recomputa because add1 is needed
        msg_info("TEST2") << "----------- First ask for add then sqrt ---------" ;
        input1->setValue(4);
        input2->setValue(4);
        add1->m_value.getValue();
        msg_info("TEST2") << "add(" << input1->m_value.getValue() << "+" << input2->m_value.getValue() << ")="
                           << add1->m_value.getValue() ;

        /// Then recompute the remaining because sqrt is needed.
        sqrtt->m_value.getValue();
        msg_info("TEST2") << "sqrt(" << input1->m_value.getValue() << "+" << input2->m_value.getValue()
                           << "+" << input1->m_value.getValue() << ") = " << sqrtt->m_value.getValue() ;

        /// TEST 3: Change the middle level.. see how muc is recomputed
        msg_info("TEST3") << "----------- change input3, query for sqrt ---------" ;
        input3->setValue(0);
        sqrtt->m_value.getValue();
        msg_info("TEST3") << "sqrt(" << input1->m_value.getValue() << "+" << input2->m_value.getValue()
                           << "+" << input1->m_value.getValue() << ") = " << sqrtt->m_value.getValue() ;

        /// TEST 4: Do we recompute several times things ?
        msg_info("TEST4") << "----------- query add and sqrt without changing the values ---------" ;
        msg_info("TEST4") << "add(" << input1->m_value.getValue() << "+" << input2->m_value.getValue() << ")="
                          << add1->m_value.getValue() ;
        msg_info("TEST4") << "sqrt(" << input1->m_value.getValue() << "+" << input2->m_value.getValue()
                           << "+" << input1->m_value.getValue() << ") = " << sqrtt->m_value.getValue() ;

        return true ;
    }
};


TEST_F(MyDataEngineTest, doCheck)
{
    ASSERT_TRUE( doCheck() );
}

}
