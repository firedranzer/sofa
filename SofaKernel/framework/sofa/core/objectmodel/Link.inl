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
#ifndef SOFA_CORE_OBJECTMODEL_LINK_INL
#define SOFA_CORE_OBJECTMODEL_LINK_INL

#include <sofa/core/objectmodel/BaseLink.h>
#include <sofa/core/ExecParams.h>
#include <sofa/helper/stable_vector.h>

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace sofa
{

namespace core
{

namespace objectmodel
{

/////////////////////// DEFINITION OF TLINK ////////////////////////////////////////////////
template<class TOwnerType, class TDestType, unsigned TFlags>
TLink<TOwnerType, TDestType, TFlags>::TLink()
    : BaseLink(ActiveFlags)
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
TLink<TOwnerType, TDestType, TFlags>::TLink(const InitLink<OwnerType>& init)
    : BaseLink(init, ActiveFlags), m_owner(init.owner)
{
    if (m_owner) m_owner->addLink(this);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
TLink<TOwnerType, TDestType, TFlags>::~TLink()
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
size_t TLink<TOwnerType, TDestType, TFlags>::size(const core::ExecParams* params = 0) const
{
    return (size_t)m_value[core::ExecParams::currentAspect(params)].size();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
bool TLink<TOwnerType, TDestType, TFlags>::empty(const core::ExecParams* params = 0) const
{
    return m_value[core::ExecParams::currentAspect(params)].empty();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
const Container& TLink<TOwnerType, TDestType, TFlags>::getValue(const core::ExecParams* params = 0) const
{
    return m_value[core::ExecParams::currentAspect(params)];
}

template<class TOwnerType, class TDestType, unsigned TFlags>
const_iterator TLink<TOwnerType, TDestType, TFlags>::begin(const core::ExecParams* params = 0) const
{
    return m_value[core::ExecParams::currentAspect(params)].cbegin();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
const_iterator TLink<TOwnerType, TDestType, TFlags>::end(const core::ExecParams* params = 0) const
{
    return m_value[core::ExecParams::currentAspect(params)].cend();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
const_reverse_iterator TLink<TOwnerType, TDestType, TFlags>::rbegin(const core::ExecParams* params = 0) const
{
    return m_value[core::ExecParams::currentAspect(params)].crbegin();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
const_reverse_iterator TLink<TOwnerType, TDestType, TFlags>::rend(const core::ExecParams* params = 0) const
{
    return m_value[core::ExecParams::currentAspect(params)].crend();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
bool TLink<TOwnerType, TDestType, TFlags>::add(DestPtr v)
{
    if (!v) return false;
    const int aspect = core::ExecParams::currentAspect();
    unsigned int index = TraitsContainer::add(m_value[aspect],v);
    this->updateCounter(aspect);
    added(v, index);
    return true;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
bool TLink<TOwnerType, TDestType, TFlags>::add(DestPtr v, const std::string& path)
{
    if (!v && path.empty()) return false;
    const int aspect = core::ExecParams::currentAspect();
    unsigned int index = TraitsContainer::add(m_value[aspect],v);
    TraitsValueType::setPath(m_value[aspect][index],path);
    this->updateCounter(aspect);
    added(v, index);
    return true;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
bool TLink<TOwnerType, TDestType, TFlags>::addPath(const std::string& path)
{
    if (path.empty()) return false;
    DestType* ptr = NULL;
    if (m_owner)
        TraitsFindDest::findLinkDest(m_owner, ptr, path, this);
    return add(ptr, path);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
bool TLink<TOwnerType, TDestType, TFlags>::remove(DestPtr v)
{
    if (!v) return false;
    const int aspect = core::ExecParams::currentAspect();
    unsigned int index = TraitsContainer::find(m_value[aspect],v);
    if (index >= m_value[aspect].size()) return false;
    TraitsContainer::remove(m_value[aspect],index);
    this->updateCounter(aspect);
    removed(v, index);
    return true;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
bool TLink<TOwnerType, TDestType, TFlags>::removePath(const std::string& path)
{
    if (path.empty()) return false;
    const int aspect = core::ExecParams::currentAspect();
    unsigned int n = m_value[aspect].size();
    for (unsigned int index=0; index<n; ++index)
    {
        std::string p = getPath(index);
        if (p == path)
        {
            DestPtr v = m_value[aspect][index];
            TraitsContainer::remove(m_value[aspect],index);
            this->updateCounter(aspect);
            removed(v, index);
            return true;
        }
    }
    return false;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
const BaseClass* TLink<TOwnerType, TDestType, TFlags>::getDestClass() const
{
    return DestType::GetClass();
}

const BaseClass* TLink<TOwnerType, TDestType, TFlags>::getOwnerClass() const
{
    return OwnerType::GetClass();
}

size_t TLink<TOwnerType, TDestType, TFlags>::getSize() const
{
    return size();
}

std::string TLink<TOwnerType, TDestType, TFlags>::getPath(unsigned int index) const
{
    const int aspect = core::ExecParams::currentAspect();
    if (index >= m_value[aspect].size())
        return std::string();
    std::string path;
    const ValueType& value = m_value[aspect][index];
    if (!TraitsValueType::path(value, path))
    {
        DestType* ptr = TraitsDestPtr::get(TraitsValueType::get(value));
        if (ptr)
            path = BaseLink::CreateString(TraitsDestCasts::getBase(ptr), TraitsDestCasts::getData(ptr),
                                          TraitsOwnerCasts::getBase(m_owner));
    }
    return path;
}

Base* TLink<TOwnerType, TDestType, TFlags>::getLinkedBase(unsigned int index=0) const
{
    return TraitsDestCasts::getBase(getIndex(index));
}
BaseData* TLink<TOwnerType, TDestType, TFlags>::getLinkedData(unsigned int index=0) const
{
    return TraitsDestCasts::getData(getIndex(index));
}
std::string TLink<TOwnerType, TDestType, TFlags>::getLinkedPath(unsigned int index=0) const
{
    return getPath(index);
}

/// @name Serialization API
/// @{

/// Read the command line
bool TLink<TOwnerType, TDestType, TFlags>::read( const std::string& str )
{
    if (str.empty())
        return true;

    bool ok = true;

    Container& container = m_value[core::ExecParams::currentAspect()];
    container.clear() ;

    // Allows spaces in links values for single links
    if (!getFlag(BaseLink::FLAG_MULTILINK))
    {
        DestType* ptr = NULL;

        if (m_owner && !TraitsFindDest::findLinkDest(m_owner, ptr, str, this))
        {
            // This is not an error, as the destination can be added later in the graph
            // instead, we will check for failed links after init is completed
            //ok = false;
        }
        else if (str[0] != '@')
        {
            ok = false;
        }

        add(ptr, str);
    }
    else
    {
        std::istringstream istr(str.c_str());
        std::string path;

        // Find the target of each path, and store those targets in
        // a temporary vector of (pointer, path) pairs
        typedef std::vector< std::pair<DestPtr, std::string> > PairVector;
        PairVector newList;
        while (istr >> path)
        {
            DestType *ptr = NULL;
            if (m_owner && !TraitsFindDest::findLinkDest(m_owner, ptr, path, this))
            {
                // This is not an error, as the destination can be added later in the graph
                // instead, we will check for failed links after init is completed
                //ok = false;
            }
            else if (path[0] != '@')
            {
                ok = false;
            }
            newList.push_back(std::make_pair(ptr, path));
        }

        // Add the objects that are not already present to the container of this Link
        for (typename PairVector::iterator i = newList.begin(); i != newList.end(); i++)
        {
            const DestPtr ptr = i->first;
            const std::string& path = i->second;

            if (TraitsContainer::find(container, ptr) == container.size()) // Not found
                add(ptr, path);
        }

        /** TODO(dmarchal) REMOVE before PR.
            // Remove the objects from the container that are not in the new list
            for (size_t i = 0; i != container.size(); i++)
            {
                DestPtr dest(container[i]);
                bool destFound = false;
                typename PairVector::iterator j = newList.begin();
                while (j != newList.end() && !destFound)
                {
                    if (j->first == dest)
                        destFound = true;
                    j++;
                }

                if (!destFound)
                    remove(dest);
            }
            */
    }

    return ok;
}


/// Check that a given path is valid, that the pointed object exists and is of the right type
template <class TContext>
static bool TLink<TOwnerType, TDestType, TFlags>::CheckPath( const std::string& path, TContext* context)
{
    if (path.empty())
        return false;
    if (!context)
    {
        std::string p,d;
        return BaseLink::ParseString( path, &p, (ActiveFlags & FLAG_DATALINK) ? &d : NULL, NULL);
    }
    else
    {
        return TraitsFindDest::checkPath(path, context);
    }
}

/// @}

/// Copy the value of an aspect into another one.
void TLink<TOwnerType, TDestType, TFlags>::copyAspect(int destAspect, int srcAspect)
{
    BaseLink::copyAspect(destAspect, srcAspect);
    m_value[destAspect] = m_value[srcAspect];
}

/// Release memory allocated for the specified aspect.
void TLink<TOwnerType, TDestType, TFlags>::releaseAspect(int aspect)
{
    BaseLink::releaseAspect(aspect);
    TraitsContainer::clear(m_value[aspect]);
}

sofa::core::objectmodel::Base* TLink<TOwnerType, TDestType, TFlags>::getOwnerBase() const
{
    return TraitsOwnerCasts::getBase(m_owner);
}
sofa::core::objectmodel::BaseData* TLink<TOwnerType, TDestType, TFlags>::getOwnerData() const
{
    return TraitsOwnerCasts::getData(m_owner);
}

void TLink<TOwnerType, TDestType, TFlags>::setOwner(OwnerType* owner)
{
    m_owner = owner;
    m_owner->addLink(this);
}

DestType* TLink<TOwnerType, TDestType, TFlags>::getIndex(unsigned int index) const
{
    const int aspect = core::ExecParams::currentAspect();
    if (index < m_value[aspect].size())
        return TraitsDestPtr::get(TraitsValueType::get(m_value[aspect][index]));
    else
        return NULL;
}

void TLink<TOwnerType, TDestType, TFlags>::added(DestPtr ptr, unsigned int index) = 0;
void TLink<TOwnerType, TDestType, TFlags>::removed(DestPtr ptr, unsigned int index) = 0;



/////////////////////// DEFINITION OF MULTILINK ////////////////////////////////////////////////
template<class TOwnerType, class TDestType, unsigned TFlags>
MultiLink<TOwnerType, TDestType, TFlags>::MultiLink(const BaseLink::InitLink<OwnerType>& init)
    : Inherit(init), m_validator(NULL)
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
MultiLink<TOwnerType, TDestType, TFlags>::MultiLink(const BaseLink::InitLink<OwnerType>& init, DestPtr val)
    : Inherit(init), m_validator(NULL)
{
    if (val) this->add(val);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
virtual MultiLink<TOwnerType, TDestType, TFlags>::~MultiLink()
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void MultiLink<TOwnerType, TDestType, TFlags>::setValidator(ValidatorFn fn)
{
    m_validator = fn;
}

/// Check that a given list of path is valid, that the pointed object exists and is of the right type
template<class TOwnerType, class TDestType, unsigned TFlags>
template<class TContext>
static bool MultiLink<TOwnerType, TDestType, TFlags>::CheckPaths( const std::string& str, TContext* context)
{
    if (str.empty())
        return false;
    std::istringstream istr( str.c_str() );
    std::string path;
    bool ok = true;
    while (istr >> path)
    {
        ok &= TLink<TOwnerType,TDestType,TFlags|BaseLink::FLAG_MULTILINK>::CheckPath(path, context);
    }
    return ok;
}

/// Update pointers in case the pointed-to objects have appeared
/// @return false if there are broken links
template<class TOwnerType, class TDestType, unsigned TFlags>
virtual bool MultiLink<TOwnerType, TDestType, TFlags>::updateLinks()
{
    if (!this->m_owner) return false;
    bool ok = true;
    const int aspect = core::ExecParams::currentAspect();
    std::size_t n = this->size();
    for (std::size_t i = 0; i<n; ++i)
    {
        ValueType& value = this->m_value[aspect][i];
        std::string path;
        if (TraitsValueType::path(value, path))
        {
            DestType* ptr = TraitsDestPtr::get(TraitsValueType::get(value));
            if (!ptr)
            {
                TraitsFindDest::findLinkDest(this->m_owner, ptr, path, this);
                if (ptr)
                {
                    DestPtr v = ptr;
                    TraitsValueType::set(value,v);
                    this->updateCounter(aspect);
                    this->added(v, i);
                }
                else
                {
                    ok = false;
                }
            }
        }
    }
    return ok;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
DestType* MultiLink<TOwnerType, TDestType, TFlags>::get(unsigned int index, const core::ExecParams* params = 0) const
{
    const int aspect = core::ExecParams::currentAspect(params);
    if (index < this->m_value[aspect].size())
        return TraitsDestPtr::get(TraitsValueType::get(this->m_value[aspect][index]));
    else
        return NULL;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
DestType* MultiLink<TOwnerType, TDestType, TFlags>::operator[](unsigned int index) const
{
    return get(index);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void MultiLink<TOwnerType, TDestType, TFlags>::added(DestPtr val, unsigned int index)
{
    if (m_validator)
        (this->m_owner->*m_validator)(val, index, true);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void MultiLink<TOwnerType, TDestType, TFlags>::removed(DestPtr val, unsigned int index)
{
    if (m_validator)
        (this->m_owner->*m_validator)(val, index, false);
}


///////////////////////////// DEFINITION OF SINGLELNK //////////////////////////////////////////////
template<class TOwnerType, class TDestType, unsigned TFlags>
SingleLink<TOwnerType,TDestType,TFlags>::SingleLink()
    : m_validator(NULL)
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
SingleLink<TOwnerType,TDestType,TFlags>::SingleLink(const BaseLink::InitLink<OwnerType>& init)
    : Inherit(init), m_validator(NULL)
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
SingleLink<TOwnerType,TDestType,TFlags>::SingleLink(const BaseLink::InitLink<OwnerType>& init, DestPtr val)
    : Inherit(init), m_validator(NULL)
{
    if (val) this->add(val);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
virtual SingleLink<TOwnerType,TDestType,TFlags>::~SingleLink()
{
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::setValidator(ValidatorFn fn)
{
    m_validator = fn;
}

template<class TOwnerType, class TDestType, unsigned TFlags>
std::string SingleLink<TOwnerType,TDestType,TFlags>::getPath() const
{
    return Inherit::getPath(0);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
DestType* SingleLink<TOwnerType,TDestType,TFlags>::get(const core::ExecParams* params = 0) const
{
    const int aspect = core::ExecParams::currentAspect(params);
    return TraitsDestPtr::get(TraitsValueType::get(this->m_value[aspect].get()));
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::reset()
{
    const int aspect = core::ExecParams::currentAspect();
    ValueType& value = this->m_value[aspect].get();
    const DestPtr before = TraitsValueType::get(value);
    if (!before) return;
    TraitsValueType::set(value, NULL);
    this->updateCounter(aspect);
    changed(before, NULL);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::set(DestPtr v)
{
    const int aspect = core::ExecParams::currentAspect();
    ValueType& value = this->m_value[aspect].get();
    const DestPtr before = TraitsValueType::get(value);
    if (v == before) return;
    TraitsValueType::set(value, v);
    this->updateCounter(aspect);
    changed(before, v);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::set(DestPtr v, const std::string& path)
{
    const int aspect = core::ExecParams::currentAspect();
    ValueType& value = this->m_value[aspect].get();
    const DestPtr before = TraitsValueType::get(value);
    if (v != before)
        TraitsValueType::set(value, v);
    TraitsValueType::setPath(value, path);
    this->updateCounter(aspect);
    if (v != before)
        changed(before, v);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::setPath(const std::string& path)
{
    if (path.empty()) { reset(); return; }
    DestType* ptr = NULL;
    if (this->m_owner)
        TraitsFindDest::findLinkDest(this->m_owner, ptr, path, this);
    set(ptr, path);
}

template<class TOwnerType, class TDestType, unsigned TFlags>
virtual bool SingleLink<TOwnerType,TDestType,TFlags>::updateLinks()
{
    if (!this->m_owner) return false;
    bool ok = true;
    const int aspect = core::ExecParams::currentAspect();
    ValueType& value = this->m_value[aspect].get();
    std::string path;
    if (TraitsValueType::path(value, path))
    {
        DestType* ptr = TraitsDestPtr::get(TraitsValueType::get(value));
        if (!ptr)
        {
            TraitsFindDest::findLinkDest(this->m_owner, ptr, path, this);
            if (ptr)
            {
                set(ptr, path);
            }
            else
            {
                ok = false;
            }
        }
    }
    return ok;
}

#ifndef SOFA_MAYBE_DEPRECATED
// Convenient operators to make a SingleLink appear as a regular pointer
template<class TOwnerType, class TDestType, unsigned TFlags>
operator DestType*() const
{
    return get();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
DestType* SingleLink<TOwnerType,TDestType,TFlags>::operator->() const
{
    return get();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
DestType& SingleLink<TOwnerType,TDestType,TFlags>::operator*() const
{
    return *get();
}

template<class TOwnerType, class TDestType, unsigned TFlags>
DestPtr SingleLink<TOwnerType,TDestType,TFlags>::operator=(DestPtr v)
{
    set(v);
    return v;
}
#endif

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::added(DestPtr val, unsigned int /*index*/)
{
    if (m_validator)
    {
        DestPtr after = val;
        (this->m_owner->*m_validator)(NULL, after);
        if (after != val)
            TraitsValueType::set(this->m_value[core::ExecParams::currentAspect()].get(), after);
    }
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::removed(DestPtr val, unsigned int /*index*/)
{
    if (m_validator)
    {
        DestPtr after = NULL;
        (this->m_owner->*m_validator)(val, after);
        if (after)
            TraitsValueType::set(this->m_value[core::ExecParams::currentAspect()].get(), after);
    }
}

template<class TOwnerType, class TDestType, unsigned TFlags>
void SingleLink<TOwnerType,TDestType,TFlags>::changed(DestPtr before, DestPtr val)
{
    if (m_validator)
    {
        DestPtr after = val;
        (this->m_owner->*m_validator)(before, after);
        if (after != val)
            TraitsValueType::set(this->m_value[core::ExecParams::currentAspect()].get(), after);
    }
}

} // namespace objectmodel

} // namespace core

// the SingleLink class is used everywhere
using core::objectmodel::SingleLink;

// the MultiLink class is used everywhere
using core::objectmodel::MultiLink;

} // namespace sofa

#endif
