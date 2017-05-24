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
#ifndef SOFA_COMPONENT_COLLISION_FRICTIONCONTACT_H
#define SOFA_COMPONENT_COLLISION_FRICTIONCONTACT_H
#include "config.h"

#include <sofa/core/collision/Contact.h>
#include <sofa/core/collision/Intersection.h>
#include <SofaConstraint/UnilateralInteractionConstraint.h>
#include <sofa/helper/Factory.h>
#include <SofaBaseCollision/BaseContactMapper.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/BaseMapping.h>

namespace sofa
{

namespace component
{

namespace collision
{

namespace _identifier_
{

using sofa::core::collision::DetectionOutput;
using std::list;

class SOFA_CONSTRAINT_API Identifier
{
public:
    Identifier()
    {
        if (!m_availableId.empty())
        {
            m_id = m_availableId.front();
            m_availableId.pop_front();
        }
        else
            m_id = m_cpt++;
    }

    virtual ~Identifier()
    {
        m_availableId.push_back(m_id);
    }

protected:
    static DetectionOutput::ContactId m_cpt;
    DetectionOutput::ContactId m_id;
    static list<DetectionOutput::ContactId> m_availableId;
};

}
using _identifier_::Identifier;



namespace _frictioncontact_
{

using core::collision::Contact;
using sofa::defaulttype::Vec3Types;
using core::behavior::MechanicalState;
using core::collision::DetectionOutputVector;
using sofa::core::collision::DetectionOutput;
using core::collision::TDetectionOutputVector;
using std::pair;
using std::vector;
using core::objectmodel::BaseContext;
using core::CollisionModel;

template <class TCollisionModel1, class TCollisionModel2, class ResponseDataTypes = Vec3Types >
class FrictionContact : public Contact, public Identifier
{

public:

    SOFA_CLASS(SOFA_TEMPLATE3(FrictionContact, TCollisionModel1, TCollisionModel2, ResponseDataTypes), Contact);
    typedef TCollisionModel1 CollisionModel1;
    typedef TCollisionModel2 CollisionModel2;
    typedef core::collision::Intersection Intersection;
    typedef ResponseDataTypes DataTypes1;
    typedef ResponseDataTypes DataTypes2;

    typedef MechanicalState<DataTypes1> MechanicalState1;
    typedef MechanicalState<DataTypes2> MechanicalState2;
    typedef typename CollisionModel1::Element CollisionElement1;
    typedef typename CollisionModel2::Element CollisionElement2;
    typedef DetectionOutputVector OutputVector;
    typedef TDetectionOutputVector<CollisionModel1,CollisionModel2> TOutputVector;

protected:

    FrictionContact() {}
    FrictionContact(CollisionModel1* m_model1, CollisionModel2* m_model2, Intersection* m_intersectionMethod);
    virtual ~FrictionContact();

    Data<double> d_mu, d_tol;

    CollisionModel1* m_model1;
    CollisionModel2* m_model2;

    Intersection* m_intersectionMethod;
    bool m_selfCollision; ///< true if model1==model2 (in this case, only mapper1 is used)

    ContactMapper<CollisionModel1,DataTypes1> m_mapper1;
    ContactMapper<CollisionModel2,DataTypes2> m_mapper2;

    constraintset::UnilateralInteractionConstraint<Vec3Types>::SPtr m_constraint;
    BaseContext* m_parent;

    vector< DetectionOutput* > m_contacts;
    vector< pair< pair<int, int>, double > > m_mappedContacts;


    void activateMappers();

    void setInteractionTags(MechanicalState1* mstate1, MechanicalState2* mstate2);


public:

    /////////////////// Inherited from BaseObject//////////////////////////////////////////
    virtual void cleanup() override;
    //////////////////////////////////////////////////////////////////////////////////////

    /////////////////// Inherited from Contact ///////////////////////////////////////////
    virtual std::pair<CollisionModel*,CollisionModel*> getCollisionModels() override
    {return std::make_pair(m_model1,m_model2);}

    virtual void setDetectionOutputs(OutputVector* outputs) override;

    virtual void createResponse(BaseContext* group) override;

    virtual void removeResponse() override;
    //////////////////////////////////////////////////////////////////////////////////////
};

inline long cantorPolynomia(DetectionOutput::ContactId x, DetectionOutput::ContactId y)
{
    // Polynome de Cantor de NxN sur N bijectif f(x,y)=((x+y)^2+3x+y)/2
    return (long)(((x+y)*(x+y)+3*x+y)/2);
}

}
using _frictioncontact_::FrictionContact;

} // collision

} // component

} // sofa

#endif // SOFA_COMPONENT_COLLISION_FRICTIONCONTACT_H
