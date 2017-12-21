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
#ifndef SOFA_CORE_ASYNC_COMPONENTTRACKER_H
#define SOFA_CORE_ASYNC_COMPONENTTRACKER_H

#include <vector>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/core/objectmodel/BaseObject.h>

namespace sofa
{
namespace core
{
namespace async
{

using sofa::core::objectmodel::Data ;
using sofa::core::objectmodel::BaseObject ;
using sofa::core::objectmodel::BaseData ;
using sofa::core::objectmodel::BaseLink ;
using sofa::helper::WriteOnlyAccessor ;

enum class State
{
    Undefined,
    Invalid,
    Valid,
    Busy,
    Waiting,
} ;

enum class Protocol
{
    Sequential,
    Async
} ;

class ComponentState
{
public:
    Protocol m_currentprotocol {Protocol::Sequential} ;
    State    m_state {State::Undefined} ;

    friend std::ostream& operator<< ( std::ostream& os, const ComponentState& root )
    {
        return os ;
    }
    friend std::istream& operator>> ( std::istream& in, ComponentState& root )
    {
        return in ;
    }
};

class DataComponentState : public Data<ComponentState>
{
public:
    void setState(State state){
        ComponentState* s = beginWriteOnly() ;
        s->m_state = state ;
        endEdit();
    }

    State getState(){
        return getValue().m_state ;
    }

    bool isValid(){
        return getValue().m_state == State::Valid ;
    }
    bool isBusy(){
        return getValue().m_state == State::Busy ;
    }
    bool isInvalid(){
        return getValue().m_state == State::Invalid ;
    }

};

class WithStateManagement
{
public:
    DataComponentState d_state ;

    WithStateManagement(BaseObject* obj)
    {
        obj->addData(&d_state, "state");
    }

    std::vector<BaseLink*> m_links ;
    std::vector<int> m_linkcounters ;

    std::vector<int> m_datacounters ;
    std::vector<DataComponentState*> m_datas ;

    template<class T>
    void addATrackerTo(T* link)
    {
        m_links.push_back(link) ;
        m_linkcounters.push_back(-1) ;
        m_datas.push_back(nullptr) ;
        m_datacounters.push_back(-1) ;
    }


    void resetComponentsTrackingCounters()
    {
        for(unsigned int i=0;i<m_links.size();i++)
        {
            m_linkcounters[i] = -1 ;
        }
    }

    void updateComponentsTrackingCounters()
    {
        for(unsigned int i=0;i<m_datas.size();i++)
        {
            if( m_datas[i] )
                m_datacounters[i] = m_datas[i]->getCounter() ;
        }
    }

    bool areTheTrackedComponentsValid()
    {
        for(unsigned int i=0;i<m_datas.size();i++)
        {
            if( m_datas[i]==nullptr || !m_datas[i]->isValid() )
                return false;
        }
        return true;
    }

    bool haveTheTrackedComponentsChanged()
    {
        /// Check that the sources tracked does not have changed.
        for(unsigned int i=0;i<m_links.size();i++)
        {
            if(m_linkcounters[i] != m_links[i]->getCounter())
            {
                /// If the source object has changed... we need to update the data tracker
                if(m_links[i]->getLinkedBase()==0)
                {
                    m_datas[i] = nullptr ;
                    m_datacounters[i] = -1 ;
                }else
                {

                    DataComponentState* state = dynamic_cast<DataComponentState*>(m_links[i]->getLinkedBase()->findData("state")) ;
                    if(state)
                    {
                        m_datas[i] = state ;
                        m_datacounters[i] = -1 ;
                    }
                    else
                    {
                        m_datas[i] = nullptr ;
                        m_datacounters[i] = -1 ;
                    }
                }
                m_linkcounters[i] = m_links[i]->getCounter() ;
            }
        }

        for(unsigned int i=0;i<m_datas.size();i++)
        {
            if(m_datas[i] && m_datacounters[i] != m_datas[i]->getCounter())
                return true ;
        }

        return false ;
    }



};


} /// namespace async
} /// namespace core
} /// namespace sofa

#endif /// SOFA_CORE_ASYNC_COMPONENTTRACKER_H


