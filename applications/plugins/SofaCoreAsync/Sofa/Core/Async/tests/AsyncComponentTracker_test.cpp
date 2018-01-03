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

#include <future>

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

namespace async{

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
        if(d_state.isBusy())
        {
            if( m_result.valid() &&
                    m_result.wait_for(std::chrono::microseconds::zero()) == std::future_status::ready )
            {
                d_state.setState(sofa::core::async::State::Ready) ;
                msg_info() << "Update to state valid at " << d_state.getCounter()  ;
            }

            return ;
        }

        d_state.setState(sofa::core::async::State::Busy) ;
        msg_info() << "Update to state 'invalid' at " << d_state.getCounter()  ;
        m_result = std::async(std::launch::async, [this](){
            msg_info() << "BeginComputing "  ;
            sleep(2);
            msg_info() << "EndComputing "   ;
            return 1;
        });
    }

    void aPublicMethod(BaseObject* callee)
    {
        msg_info(callee)
                << "Calling '" << getName() << "' which is at state " << d_state.getCounter()  ;
    }

    std::future<int> m_result ;
};



class AsyncComponentWithoutAsync : public BaseObject,
        public WithStateManagement
{
public:
    SOFA_CLASS(AsyncComponentWithoutAsync, BaseObject);

    AsyncComponentWithoutAsync(BeginComponent* source) :
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
        m_counter++;
        msg_info() << "calculate{" << m_counter << "} at:" << source->d_state.getCounter()  ;
        sleep(3);
    }

    void aPublicMethod(BaseObject* callee)
    {
        msg_info(callee)
                << "Calling '" << getName() << "' which is at state " << d_state.getCounter()  ;
    }

    std::future<int> m_result ;
    void update()
    {
        /// If it is computing something
        if(d_state.isBusy())
        {
            /// We got the result...
            if( m_result.valid() &&
                    m_result.wait_for(std::chrono::microseconds::zero()) == std::future_status::ready )
            {
                d_state.setState(sofa::core::async::State::Ready) ;
                msg_info() << "Update to state valid at " << d_state.getCounter()  ;
            }
            else if( haveTheTrackedComponentsChanged() )
            {
                /// We should cancel & restart the computation.
            }
        }

        if( !haveTheTrackedComponentsChanged() )
            return ;

        if( !areTheTrackedComponentsValid() )
            return ;

        d_state.setState(sofa::core::async::State::Busy) ;
        msg_info() << "Update to state 'invalid' at " << d_state.getCounter()  ;
        m_result = std::async(std::launch::async, [this](){
            msg_info() << "Computing start"  ;
            internalCalculus() ;
            msg_info() << "Computing end"   ;
            return 1;
        });

        updateComponentsTrackingCounters() ;
    }

    bool done()
    {
        return m_counter == 3 ;
    }

    int m_counter {0} ;

    SingleLink<AsyncComponentWithoutAsync, BeginComponent,
    BaseLink::FLAG_STOREPATH> l_source ;

};

class AsyncEndComponentWithoutAsync : public BaseObject,
        public WithStateManagement
{
public:
    SOFA_CLASS(AsyncEndComponentWithoutAsync, BaseObject);

    AsyncEndComponentWithoutAsync(AsyncComponentWithoutAsync* source) :
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

        AsyncComponentWithoutAsync* source = l_source.get() ;
        source->aPublicMethod(this) ;

        msg_info() << "Update to state: " << source->d_state.getCounter()  ;

        updateComponentsTrackingCounters() ;
    }


    SingleLink<AsyncEndComponentWithoutAsync, AsyncComponentWithoutAsync,
    BaseLink::FLAG_STOREPATH> l_source ;

};


class AsyncComponentTracker_test : public BaseTest
{
public:
    void test1()
    {
        BeginComponent   source ;
        AsyncComponentWithoutAsync   target(&source) ;

        source.setName("source") ;
        target.setName("target") ;

        msg_info("TEST") << " ----- init ------- "  ;
        target.init() ;
        source.init() ;

        while(!target.done()){
            target.update() ;
            source.update() ;
        }
    }

    void test2()
    {
        BeginComponent          source;
        AsyncComponentWithoutAsync   targetA(&source) ;
        AsyncComponentWithoutAsync   targetB(&source) ;

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
        AsyncComponentWithoutAsync   targetA(&source) ;
        AsyncComponentWithoutAsync   targetB(&source) ;
        AsyncEndComponentWithoutAsync   endA(&targetA) ;
        AsyncEndComponentWithoutAsync   endB(&targetB) ;

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
};

TEST_F(AsyncComponentTracker_test, simpleBehavior)
{
    this->test1() ;
}

TEST_F(AsyncComponentTracker_test, singleSourceTwoTarget)
{
    this->test2() ;
}

TEST_F(AsyncComponentTracker_test, diamondTest)
{
    this->test3() ;
}

}
