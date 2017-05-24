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
#ifndef SOFA_COMPONENT_COLLISION_FRICTIONCONTACT_INL
#define SOFA_COMPONENT_COLLISION_FRICTIONCONTACT_INL

#include <SofaConstraint/FrictionContact.h>
#include <sofa/core/visual/VisualParams.h>
#include <SofaBaseCollision/DefaultContactManager.h>
#include <SofaMeshCollision/BarycentricContactMapper.h>
#include <SofaMeshCollision/IdentityContactMapper.h>
#include <SofaMeshCollision/RigidContactMapper.h>
#include <sofa/simulation/Node.h>
#include <iostream>

namespace sofa
{

namespace component
{

namespace collision
{

using core::CollisionModel;
using sofa::core::collision::DetectionOutput;
using core::objectmodel::BaseContext;
using sofa::core::objectmodel::TagSet;
using std::vector;
using sofa::core::objectmodel::New;


template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::FrictionContact(CollisionModel1* model1, CollisionModel2* model2, Intersection* intersectionMethod)
    : d_mu (initData(&d_mu, 0.8, "mu", "friction coefficient (0 for frictionless contacts)"))
    , d_tol (initData(&d_tol, 0.0, "tol", "tolerance for the constraints resolution (0 for default tolerance)"))
    , m_model1(model1)
    , m_model2(model2)
    , m_intersectionMethod(intersectionMethod)
    , m_constraint(NULL)
    , m_parent(NULL)

{
    m_selfCollision = ((CollisionModel*)model1 == (CollisionModel*)model2);
    m_mapper1.setCollisionModel(model1);
    if (!m_selfCollision) m_mapper2.setCollisionModel(model2);
    m_contacts.clear();
    m_mappedContacts.clear();

}

template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::~FrictionContact()
{
}

template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
void FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::cleanup()
{
    if (m_constraint)
    {
        m_constraint->cleanup();

        if (m_parent != NULL)
            m_parent->removeObject(m_constraint);

        m_parent = NULL;
        m_constraint.reset();

        m_mapper1.cleanup();

        if (!m_selfCollision)
            m_mapper2.cleanup();
    }

    m_contacts.clear();
    m_mappedContacts.clear();
}


template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
void FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::setDetectionOutputs(OutputVector* o)
{
    TOutputVector& outputs = *static_cast<TOutputVector*>(o);
    // We need to remove duplicate contacts
    const double minDist2 = 0.00000001f;

    m_contacts.clear();

    if (m_model1->getContactStiffness(0) == 0 || m_model2->getContactStiffness(0) == 0)
    {
        msg_error() << "Disabled FrictionContact with " << (outputs.size()) << " collision points.";
        return;
    }

    m_contacts.reserve(outputs.size());

    int SIZE = outputs.size();

    // The following procedure cancels the duplicated detection outputs
    for (int cpt=0; cpt<SIZE; cpt++)
    {
        DetectionOutput* o = &outputs[cpt];

        bool found = false;
        for (unsigned int i=0; i<m_contacts.size() && !found; i++)
        {
            DetectionOutput* p = m_contacts[i];
            if ((o->point[0]-p->point[0]).norm2()+(o->point[1]-p->point[1]).norm2() < minDist2)
                found = true;
        }

        if (!found)
            m_contacts.push_back(o);
    }

    msg_info_when(m_contacts.size()<outputs.size()) << "Duplicated contacts found. Removed " << (outputs.size()-m_contacts.size()) <<" / " << outputs.size() << " collision points." << msgendl;
}


template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
void FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::activateMappers()
{
    if (!m_constraint)
    {
        // Get the mechanical model from mapper1 to fill the constraint vector
        MechanicalState1* mmodel1 = m_mapper1.createMapping(GenerateStirngID::generate().c_str());
        // Get the mechanical model from mapper2 to fill the constraints vector
        MechanicalState2* mmodel2 = m_selfCollision ? mmodel1 : m_mapper2.createMapping(GenerateStirngID::generate().c_str());
        m_constraint = New<constraintset::UnilateralInteractionConstraint<Vec3Types> >(mmodel1, mmodel2);
        m_constraint->setName( getName() );
        setInteractionTags(mmodel1, mmodel2);
        m_constraint->setCustomTolerance( d_tol.getValue() );
    }

    int size = m_contacts.size();
    m_constraint->clear(size);
    if (m_selfCollision)
        m_mapper1.resize(2*size);
    else
    {
        m_mapper1.resize(size);
        m_mapper2.resize(size);
    }
    int i = 0;
    const double d0 = m_intersectionMethod->getContactDistance() + m_model1->getProximity() + m_model2->getProximity();

    m_mappedContacts.resize(m_contacts.size());
    for (vector<DetectionOutput*>::const_iterator it = m_contacts.begin(); it!=m_contacts.end(); it++, i++)
    {
        DetectionOutput* o = *it;
        CollisionElement1 elem1(o->elem.first);
        CollisionElement2 elem2(o->elem.second);
        int index1 = elem1.getIndex();
        int index2 = elem2.getIndex();

        typename DataTypes1::Real r1 = 0.;
        typename DataTypes2::Real r2 = 0.;

        // Create mapping for first point
        index1 = m_mapper1.addPointB(o->point[0], index1, r1
#ifdef DETECTIONOUTPUT_BARYCENTRICINFO
                , o->baryCoords[0]
#endif
                                  );
        // Create mapping for second point
        if (m_selfCollision)
        {
            index2 = m_mapper1.addPointB(o->point[1], index2, r2
#ifdef DETECTIONOUTPUT_BARYCENTRICINFO
                    , o->baryCoords[1]
#endif
                                      );
        }
        else
        {
            index2 = m_mapper2.addPointB(o->point[1], index2, r2
#ifdef DETECTIONOUTPUT_BARYCENTRICINFO
                    , o->baryCoords[1]
#endif
                                      );
        }
        double distance = d0 + r1 + r2;

        m_mappedContacts[i].first.first = index1;
        m_mappedContacts[i].first.second = index2;
        m_mappedContacts[i].second = distance;
    }

    // Update mappings
    m_mapper1.update();
    m_mapper1.updateXfree();
    if (!m_selfCollision) m_mapper2.update();
    if (!m_selfCollision) m_mapper2.updateXfree();
}

template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
void FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::createResponse(BaseContext* group)
{

    activateMappers();
    const double mu_ = this->d_mu.getValue();
    // Checks if friction is considered
    if (mu_ < 0.0)
        msg_error() << "mu has to take positive value.";

    int i=0;
    if (m_constraint)
    {
        for (vector<DetectionOutput*>::const_iterator it = m_contacts.begin(); it!=m_contacts.end(); it++, i++)
        {
            DetectionOutput* o = *it;
            int index1 = m_mappedContacts[i].first.first;
            int index2 = m_mappedContacts[i].first.second;
            double distance = m_mappedContacts[i].second;

            // Polynome de Cantor de NxN sur N bijectif f(x,y)=((x+y)^2+3x+y)/2
            long index = cantorPolynomia(o->id,m_id);

            // Add contact in unilateral constraint
            m_constraint->addContact(mu_, o->normal, distance, index1, index2, index, o->id);
        }

        if (m_parent!=NULL)
        {
            m_parent->removeObject(this);
            m_parent->removeObject(m_constraint);
        }

        m_parent = group;
        if (m_parent!=NULL)
        {
            m_parent->addObject(this);
            m_parent->addObject(m_constraint);
        }
    }
}

template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
void FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::removeResponse()
{
    if (m_constraint)
    {
        m_mapper1.resize(0);
        m_mapper2.resize(0);
        if (m_parent!=NULL)
        {
            m_parent->removeObject(this);
            m_parent->removeObject(m_constraint);
        }
        m_parent = NULL;
    }
}

template < class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes  >
void FrictionContact<TCollisionModel1,TCollisionModel2,ResponseDataTypes>::setInteractionTags(MechanicalState1* mstate1, MechanicalState2* mstate2)
{
    TagSet tagsm1 = mstate1->getTags();
    TagSet tagsm2 = mstate2->getTags();
    TagSet::iterator it;
    for(it=tagsm1.begin(); it != tagsm1.end(); it++)
        m_constraint->addTag(*it);
    for(it=tagsm2.begin(); it!=tagsm2.end(); it++)
        m_constraint->addTag(*it);
}

} // namespace collision

} // namespace component

} // namespace sofa

#endif
