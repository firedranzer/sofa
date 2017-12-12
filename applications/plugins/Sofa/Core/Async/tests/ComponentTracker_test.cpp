/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/******************************************************************************
 * Contributors:
 *  - damien.marchal@univ-lille1.fr
 *****************************************************************************/
#include <iostream>

#include <sofa/helper/testing/BaseTest.h>
using sofa::helper::testing::BaseTest ;

#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject ;

#include <sofa/core/objectmodel/Data.h>
using sofa::core::objectmodel::Data ;

#include <sofa/core/objectmodel/Link.h>
using sofa::core::objectmodel::BaseLink ;
using sofa::core::objectmodel::SingleLink ;

#include <Sofa/Core/Async/ComponentTracker.h>
using sofa::core::async::ComponentState ;
using sofa::core::async::WithStateManagement ;


class BeginComponent : public BaseObject,
                       public WithStateManagement
{
public:
    SOFA_CLASS(BeginComponent, BaseObject);

    BeginComponent() : WithStateManagement(this)
    {
        f_printLog.setValue(true) ;
    }

    void init()
    {
        d_state.setState(sofa::core::async::State::Invalid) ;
        update() ;        
    }

    void update()
    {
        static int i = 0;
        if(i%5 == 0){
            d_state.setState(sofa::core::async::State::Invalid) ;
            d_state.setState(sofa::core::async::State::Valid) ;
            msg_info() << "Update to state " << d_state.getCounter()  ;
        }
        i++;
    }

    void aPublicMethod(BaseObject* callee)
    {
        msg_info(callee)
                << "Calling '" << getName() << "' which is at state " << d_state.getCounter()  ;
    }
};



class ComponentWithoutAsync : public BaseObject,
                              public WithStateManagement
{
public:
    SOFA_CLASS(ComponentWithoutAsync, BaseObject);

    ComponentWithoutAsync(BeginComponent* source) :
        WithStateManagement(this),
        l_source(initLink("source", "The data source."))
    {
        l_source.set(source) ;
        f_printLog.setValue(true) ;
        addATrackerTo(&l_source) ;
    }

    void init()
    {
        update() ;
    }

    void internalCalculus()
    {
        BeginComponent* source = l_source.get() ;
        source->aPublicMethod(this) ;
        msg_info() << "Update to state: " << source->d_state.getCounter()  ;
    }

    void aPublicMethod(BaseObject* callee)
    {
        msg_info(callee)
                << "Calling '" << getName() << "' which is at state " << d_state.getCounter()  ;
    }

    void update()
    {
        if( !haveTheTrackedComponentsChanged() )
            return ;

        if( !areTheTrackedComponentsValid() )
            return ;

        d_state.setState(sofa::core::async::State::Invalid) ;
        internalCalculus() ;
        d_state.setState(sofa::core::async::State::Valid) ;

        updateComponentsTrackingCounters() ;
    }

    SingleLink<ComponentWithoutAsync, BeginComponent,
               BaseLink::FLAG_STOREPATH> l_source ;

};

class EndComponentWithoutAsync : public BaseObject,
                                 public WithStateManagement
{
public:
    SOFA_CLASS(EndComponentWithoutAsync, BaseObject);

    EndComponentWithoutAsync(ComponentWithoutAsync* source) :
        WithStateManagement(this),
        l_source(initLink("source", "The data source."))
    {
        l_source.set(source) ;
        f_printLog.setValue(true) ;
        addATrackerTo(&l_source) ;
    }

    void init()
    {
        update() ;
    }

    void update()
    {
        if( !haveTheTrackedComponentsChanged() )
            return ;

        if( !areTheTrackedComponentsValid() )
            return ;

        ComponentWithoutAsync* source = l_source.get() ;
        source->aPublicMethod(this) ;

        msg_info() << "Update to state: " << source->d_state.getCounter()  ;

        updateComponentsTrackingCounters() ;
    }

    SingleLink<EndComponentWithoutAsync, ComponentWithoutAsync,
               BaseLink::FLAG_STOREPATH> l_source ;

};


class CircularComponent : public BaseObject,
                          public WithStateManagement
{
public:
    SOFA_CLASS(CircularComponent, BaseObject);

    CircularComponent(CircularComponent* source) :
        WithStateManagement(this),
        l_source(initLink("source", "The data source."))
    {
        l_source.set(source) ;
        f_printLog.setValue(true) ;
        addATrackerTo(&l_source) ;
        if(source){
            m_internalValue = 0 ;
            d_state.setState(sofa::core::async::State::Invalid) ;
        }
        else{
            m_internalValue = 1 ;
            d_state.setState(sofa::core::async::State::Valid) ;
        }
    }

    void setComponent(CircularComponent* c)
    {
        l_source.set(c) ;
    }

    void init()
    {
    }

    void internalCalculus()
    {
        CircularComponent* source = l_source.get() ;
        m_internalValue = source->m_internalValue ;
        source->m_internalValue = 0 ;
     }

    void aPublicMethod(BaseObject* callee)
    {
        msg_info(callee)
                << "Calling '" << getName() << "' which is at state " << d_state.getCounter()  ;
    }

    void update()
    {
        if( !haveTheTrackedComponentsChanged() ){
            CircularComponent* source = l_source.get() ;

            msg_info() << "Current value is {"<< m_internalValue <<", " << d_state.getCounter() << "} calculated from " << source->getName()
                       << " {" << source->m_internalValue << ", " << source->d_state.getCounter() << "}" ;

            return ;
        }

        if( !areTheTrackedComponentsValid() ){
            CircularComponent* source = l_source.get() ;

            msg_info() << "Current value is {"<< m_internalValue <<", " << d_state.getCounter() << "} calculated from " << source->getName()
                       << " {" << source->m_internalValue << ", " << source->d_state.getCounter() << "}" ;

            return ;
        }

        d_state.setState(sofa::core::async::State::Invalid) ;
        internalCalculus() ;
        d_state.setState(sofa::core::async::State::Valid) ;

        CircularComponent* source = l_source.get() ;
        msg_info() << "New value is {"<< m_internalValue <<", " << d_state.getCounter() << "} calculated from " << source->getName()
                   << " {" << source->m_internalValue << ", " << source->d_state.getCounter() << "}" ;

        updateComponentsTrackingCounters() ;
    }

    SingleLink<CircularComponent, CircularComponent,
               BaseLink::FLAG_STOREPATH> l_source ;

    int m_internalValue ;

};



class ComponentTracker_test : public BaseTest
{
public:
    void test1()
    {
        BeginComponent   source ;
        ComponentWithoutAsync   target(&source) ;

        source.setName("source") ;
        target.setName("target") ;

        msg_info("TEST") << " ----- init ------- "  ;
        target.init() ;
        source.init() ;
        for(unsigned int i=0;i<10;i++){
            msg_info("TEST") << " ----- update "<< i <<" ------- "  ;
            target.update() ;
            source.update() ;
        }
    }

    void test2()
    {
        BeginComponent          source;
        ComponentWithoutAsync   targetA(&source) ;
        ComponentWithoutAsync   targetB(&source) ;

        source.setName("source") ;
        targetA.setName("targetA") ;
        targetB.setName("targetB") ;

        msg_info("TEST") << " ----- init ------- "  ;
        targetA.init() ;
        source.init() ;
        targetB.init() ;

        for(unsigned int i=0;i<10;i++){
            msg_info("TEST") << " ----- update "<< i <<" ------- "  ;
            targetB.update() ;
            targetA.update() ;
            source.update() ;
        }
    }

    void test3()
    {
        BeginComponent          source;
        ComponentWithoutAsync   targetA(&source) ;
        ComponentWithoutAsync   targetB(&source) ;
        EndComponentWithoutAsync   endA(&targetA) ;
        EndComponentWithoutAsync   endB(&targetB) ;

        source.setName("source") ;
        targetA.setName("targetA") ;
        targetB.setName("targetB") ;
        endA.setName("endA") ;
        endB.setName("endB") ;

        msg_info("TEST") << " ----- init ------- "  ;
        targetA.init() ;
        source.init() ;
        targetB.init() ;
        endA.init() ;
        endB.init() ;

        for(unsigned int i=0;i<10;i++){
            msg_info("TEST") << " ----- update "<< i <<" ------- "  ;
            targetB.update() ;
            endA.update() ;
            targetA.update() ;
            source.update() ;
            endB.update() ;
        }
    }

    void circularTest()
    {
        CircularComponent   targetA(nullptr) ;
        CircularComponent   targetB(&targetA) ;
        CircularComponent   targetC(&targetB) ;
        CircularComponent   targetD(&targetC) ;
        targetA.setComponent(&targetD) ;

        targetA.setName("targetA") ;
        targetB.setName("targetB") ;
        targetC.setName("targetC") ;
        targetD.setName("targetD") ;

        msg_info("TEST") << " ----- init ------- "  ;
        targetA.init() ;
        targetB.init() ;
        targetC.init() ;
        targetD.init() ;

        for(unsigned int i=0;i<10;i++){
            msg_info("TEST") << " ----- update "<< i <<" ------- "  ;
            targetA.update() ;
            targetB.update() ;
            targetD.update() ;
            targetC.update() ;
            msg_info("TEST") << " ----- update "<< i << " - "
                             << targetA.m_internalValue << ", "
                             << targetB.m_internalValue << ", "
                             << targetC.m_internalValue << ", "
                             << targetD.m_internalValue << ", "
                             << " ------- "  ;

        }
    }
};

TEST_F(ComponentTracker_test, simpleBehavior)
{
    this->test1() ;
}

TEST_F(ComponentTracker_test, singleSourceTwoTarget)
{
    this->test2() ;
}

TEST_F(ComponentTracker_test, diamondTest)
{
    this->test3() ;
}

TEST_F(ComponentTracker_test, circularTest)
{
    this->circularTest() ;
}
