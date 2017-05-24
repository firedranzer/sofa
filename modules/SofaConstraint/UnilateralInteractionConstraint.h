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
#ifndef SOFA_COMPONENT_CONSTRAINTSET_UNILATERALINTERACTIONCONSTRAINT_H
#define SOFA_COMPONENT_CONSTRAINTSET_UNILATERALINTERACTIONCONSTRAINT_H
#include "config.h"

#include <sofa/core/behavior/PairInteractionConstraint.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/defaulttype/VecTypes.h>
#include <iostream>
#include <map>
#include <deque>


namespace sofa
{

namespace component
{

namespace constraintset
{

class UnilateralConstraintResolution : public core::behavior::ConstraintResolution
{
public:
    virtual void resolution(int line, double** w, double* d, double* force, double *dfree)
    {
        SOFA_UNUSED(dfree);
        force[line] -= d[line] / w[line][line];
        if(force[line] < 0)
            force[line] = 0.0;
    }
};


// A little experiment on how to best save the forces for the hot start.
//  TODO : save as a map (index of the contact <-> force)
class PreviousForcesContainer
{
public:
    PreviousForcesContainer() : m_resetFlag(true) {}
    double popForce()
    {
        m_resetFlag = true;
        if(m_forces.empty()) return 0;
        double f = m_forces.front();
        m_forces.pop_front();
        return f;
    }

    void pushForce(double f)
    {
        if(m_resetFlag)
        {
            m_forces.clear();
            m_resetFlag = false;
        }
        m_forces.push_back(f);
    }

protected:
    std::deque<double> m_forces;
    bool m_resetFlag; // We delete all forces that were not read
};



class SOFA_CONSTRAINT_API UnilateralConstraintResolutionWithFriction : public core::behavior::ConstraintResolution
{
public:
    UnilateralConstraintResolutionWithFriction(double mu, PreviousForcesContainer* prev=NULL, bool* active = NULL)
        : m_mu(mu)
        , m_prev(prev)
        , m_active(active)
    {
        nbLines=3;
    }

    virtual void init(int line, double** w, double* force);
    virtual void resolution(int line, double** w, double* d, double* force, double *dFree);
    virtual void store(int line, double* force, bool convergence);

protected:
    double m_mu;
    double m_W[6];
    PreviousForcesContainer* m_prev;
    bool* m_active; // Will set this after the resolution
};


namespace _unilateralinteractionconstraint_ {

using core::behavior::PairInteractionConstraint;

template<class DataTypes>
class UnilateralInteractionConstraint : public PairInteractionConstraint<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(UnilateralInteractionConstraint,DataTypes), SOFA_TEMPLATE(PairInteractionConstraint,DataTypes));

    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename DataTypes::MatrixDeriv::RowConstIterator MatrixDerivRowConstIterator;
    typedef typename DataTypes::MatrixDeriv::ColConstIterator MatrixDerivColConstIterator;
    typedef typename DataTypes::MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef typename DataTypes::MatrixDeriv::ColIterator MatrixDerivColIterator;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;
    typedef typename core::behavior::MechanicalState<DataTypes> MechanicalState;

    typedef core::behavior::BaseConstraint::ConstraintBlockInfo ConstraintBlockInfo;
    typedef core::behavior::BaseConstraint::PersistentID PersistentID;
    typedef core::behavior::BaseConstraint::ConstCoord ConstCoord;
    typedef core::behavior::BaseConstraint::ConstDeriv ConstDeriv;
    typedef core::behavior::BaseConstraint::ConstArea ConstArea;

    typedef core::behavior::BaseConstraint::VecConstraintBlockInfo VecConstraintBlockInfo;
    typedef core::behavior::BaseConstraint::VecPersistentID VecPersistentID;
    typedef core::behavior::BaseConstraint::VecConstCoord VecConstCoord;
    typedef core::behavior::BaseConstraint::VecConstDeriv VecConstDeriv;
    typedef core::behavior::BaseConstraint::VecConstArea VecConstArea;

    typedef core::objectmodel::Data<VecCoord>		DataVecCoord;
    typedef core::objectmodel::Data<VecDeriv>		DataVecDeriv;
    typedef core::objectmodel::Data<MatrixDeriv>    DataMatrixDeriv;

    typedef typename core::behavior::PairInteractionConstraint<DataTypes> Inherit;

protected:

    struct Contact
    {
        int m1, m2;		///< the two extremities of the spring: masses m1 and m2
        Deriv norm;		///< contact normal, from m1 to m2
        Deriv t;		///< added for friction
        Deriv s;		///< added for friction
        Real contactDistance;

        unsigned int id;
        long contactId;
        PersistentID localId;
        double mu;		///< friction coefficient

        Coord P, Q;

        mutable Real dfree;
    };

    sofa::helper::vector<Contact> m_contacts;
    Real m_epsilon;
    bool m_yetIntegrated;
    double m_customTolerance;

    bool* m_contactsStatus;

    /// Computes constraint violation in position and stores it into resolution global vector
    ///
    /// @param v Global resolution vector
    virtual void getPositionViolation(defaulttype::BaseVector *v);

    ///Computes constraint violation in velocity and stores it into resolution global vector
    ///
    /// @param v Global resolution vector
    virtual void getVelocityViolation(defaulttype::BaseVector *v);

public:

    unsigned int m_constraintId;

protected:

    UnilateralInteractionConstraint(MechanicalState* object1=NULL, MechanicalState* object2=NULL)
        : Inherit(object1, object2)
        , m_epsilon(Real(0.001))
        , m_yetIntegrated(false)
        , m_customTolerance(0.0)
        , m_contactsStatus(NULL)
    {
    }

    virtual ~UnilateralInteractionConstraint()
    {
        if(m_contactsStatus)
            delete[] m_contactsStatus;
    }

public:

    void setCustomTolerance(double tol);

    void clear(int reserve = 0);

    virtual void addContact(double mu, Deriv norm, Coord P, Coord Q, Real contactDistance, int m1, int m2, Coord Pfree, Coord Qfree, long id=0, PersistentID localid=0);
    void addContact(double mu, Deriv norm, Coord P, Coord Q, Real contactDistance, int m1, int m2, long id=0, PersistentID localid=0);
    void addContact(double mu, Deriv norm, Real contactDistance, int m1, int m2, long id=0, PersistentID localid=0);

    ////////////////////////// Inherited from BaseConstraint ////////////////////
    virtual void buildConstraintMatrix(const core::ConstraintParams* cParams, DataMatrixDeriv &c1, DataMatrixDeriv &c2, unsigned int &cIndex,
                                       const DataVecCoord &x1, const DataVecCoord &x2) override;

    virtual void getConstraintViolation(const core::ConstraintParams* cParams, defaulttype::BaseVector *v, const DataVecCoord &x1, const DataVecCoord &x2,
                                        const DataVecDeriv &v1, const DataVecDeriv &v2) override;

    virtual void getConstraintInfo(const core::ConstraintParams* cParams, VecConstraintBlockInfo& blocks, VecPersistentID& ids, VecConstCoord& positions, VecConstDeriv& directions, VecConstArea& areas)override;

    virtual void getConstraintResolution(const core::ConstraintParams *,std::vector<core::behavior::ConstraintResolution*>& resTab, unsigned int& offset) override;
    /////////////////////////////////////////////////////////////////////////

    bool isActive() const;

    ////////////////////////// Inherited from BaseObject ////////////////////
    virtual void draw(const core::visual::VisualParams* vparams) override;
    /////////////////////////////////////////////////////////////////////////
};
}

using _unilateralinteractionconstraint_::UnilateralInteractionConstraint;

#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_CONSTRAINTSET_UNILATERALINTERACTIONCONSTRAINT_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec3dTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec2dTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec1dTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec6dTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Rigid3dTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Rigid2dTypes>;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec3fTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec2fTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec1fTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Vec6fTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Rigid3fTypes>;
//extern template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<defaulttype::Rigid2fTypes>;
#endif
#endif

} // namespace constraintset

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_CONSTRAINTSET_UNILATERALINTERACTIONCONSTRAINT_H
