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
#define SOFA_COMPONENT_CONSTRAINTSET_UNILATERALINTERACTIONCONSTRAINT_CPP
#include <SofaConstraint/UnilateralInteractionConstraint.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <SofaBaseMechanics/MechanicalObject.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace constraintset
{

using namespace sofa::defaulttype;
using namespace sofa::helper;

SOFA_DECL_CLASS(UnilateralInteractionConstraint)

//TODO(dmarchal) What does this TODO mean ?
int UnilateralInteractionConstraintClass = core::RegisterObject("TODO-UnilateralInteractionConstraint")
#ifndef SOFA_FLOAT
        .add< UnilateralInteractionConstraint<Vec3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< UnilateralInteractionConstraint<Vec3fTypes> >()
#endif
        ;


#ifndef SOFA_FLOAT
template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<Vec3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_CONSTRAINT_API UnilateralInteractionConstraint<Vec3fTypes>;
#endif


void UnilateralConstraintResolutionWithFriction::init(int line, double** w, double* force)
{
    m_W[0]=w[line  ][line  ];
    m_W[1]=w[line  ][line+1];
    m_W[2]=w[line  ][line+2];
    m_W[3]=w[line+1][line+1];
    m_W[4]=w[line+1][line+2];
    m_W[5]=w[line+2][line+2];

    ////////////////// christian : the following does not work ! /////////
    if(m_prev)
    {
        force[line] = m_prev->popForce();
        force[line+1] = m_prev->popForce();
        force[line+2] = m_prev->popForce();
    }
}

void UnilateralConstraintResolutionWithFriction::resolution(int line, double** w, double* d, double* force, double * dfree)
{
    SOFA_UNUSED(w);
    SOFA_UNUSED(dfree);

    double f[2];
    double normFt;

    f[0] = force[line]; f[1] = force[line+1];
    force[line] -= d[line] / m_W[0];

    if(force[line] < 0)
    {
        force[line]=0; force[line+1]=0; force[line+2]=0;
        return;
    }

    d[line+1] += m_W[1] * (force[line]-f[0]);
    d[line+2] += m_W[2] * (force[line]-f[0]);
    force[line+1] -= 2*d[line+1] / (m_W[3] +m_W[5]) ;
    force[line+2] -= 2*d[line+2] / (m_W[3] +m_W[5]) ;

    normFt = sqrt(force[line+1]*force[line+1] + force[line+2]*force[line+2]);

    double fN = m_mu*force[line];
    if(normFt > fN)
    {
        double factor = fN / normFt;
        force[line+1] *= factor;
        force[line+2] *= factor;
    }
}

void UnilateralConstraintResolutionWithFriction::store(int line, double* force, bool convergence)
{
    SOFA_UNUSED(convergence);

    if(m_prev)
    {
        m_prev->pushForce(force[line]);
        m_prev->pushForce(force[line+1]);
        m_prev->pushForce(force[line+2]);
    }

    if(m_active)
    {
        *m_active = (force[line] != 0);
        m_active = NULL; // Won't be used in the haptic thread
    }
}


} // namespace constraintset

} // namespace component

} // namespace sofa

