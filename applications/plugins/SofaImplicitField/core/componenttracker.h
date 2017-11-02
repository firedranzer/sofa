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
 * Contributors:                                                              *
 *   - damien.marchal@univ-lille1.fr                                          *
 *                                                                            *
 *****************************************************************************/
#ifndef SOFA_CORE_COMPONENTTRACKER_H
#define SOFA_CORE_COMPONENTTRACKER_H
#include <SofaImplicitField/config.h>

#include <mutex>
#include <thread>

namespace sofa
{
namespace core
{
namespace objectmodel
{

class BaseObject ;

enum class CStatus
{
    Invalid,
    Valid,
    Busy,
    Waiting,
} ;

struct CState
{
    CStatus      state ;
    unsigned int counter ;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The TrackedComponent class
///
/// This component holds the status of a base object.
/// Each time the status is changed, a counter is incremented allowing third party to track the
/// change and update their data acordingly
///
/// The mechanism is very similar to DataTracker but at a lighter granularity.
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////
class TrackedComponent
{
public:
    TrackedComponent(BaseObject* baseobject)
    {
        m_state.counter = 0 ;
        m_state.state = CStatus::Invalid ;
        m_this = baseobject ;
    }

    CState getState()
    {
        std::lock_guard<std::mutex> atomicBloc(m_mutex) ;
        CState t = m_state;
        return t ;
    }

    bool isInStateValid()
    {
        std::lock_guard<std::mutex> atomicBloc(m_mutex) ;
        return m_state.state == CStatus::Valid ;
    }


    BaseObject* getPointer()
    {
        return  m_this ;
    }

protected:
    /// Only the component can change its status.
    void setStatus(CStatus status){
        std::lock_guard<std::mutex> atomicBloc(m_mutex) ;
        m_state.state  = status ;
        m_state.counter++ ;
    }

    TrackedComponent() = delete ;
    BaseObject*  m_this    {nullptr};
    CState       m_state   {CStatus::Invalid, 0};
    std::mutex   m_mutex ;
} ;

class ComponentTracker
{
public:
    void addComponent(TrackedComponent* t)
    {
        m_counters.push_back(t->getState().counter);
        m_components.push_back(t);
    }

    bool hasChanged(){
        for(unsigned int i=0;i<m_components.size();i++)
        {
            if( m_counters[i] != m_components[i]->getState().counter )
                return true ;
        }
        return false ;
    }

    void updateCounter()
    {
        for(unsigned int i=0;i<m_components.size();i++)
        {
            m_counters[i] = m_components[i]->getState().counter ;
        }
    }
    void updateCounterAt(unsigned int countervalue)
    {
        for(unsigned int i=0;i<m_components.size();i++)
        {
            m_counters[i] = countervalue ;
        }
    }

private:
    std::vector<unsigned int> m_counters ;
    std::vector<TrackedComponent*> m_components ;
} ;

} /// objectmodel
} /// core
} /// sofa

#endif SOFA_CORE_COMPONENTTRACKER_H

