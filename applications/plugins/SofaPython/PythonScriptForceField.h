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
#ifndef PYTHONFORCEFIELD_H
#define PYTHONFORCEFIELD_H

#include "PythonEnvironment.h"
#include "ScriptForceField.inl"
#include <sofa/core/objectmodel/DataFileName.h>

/// Forward declarations
namespace sofa{
    namespace helper{
        namespace system{ class FileEventListener; }
    }
}


namespace sofa
{

namespace core
{

namespace behavior
{

template <class DataTypes>
class SOFA_SOFAPYTHON_API PythonScriptForceField : public ScriptForceField<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(PythonScriptForceField,DataTypes),SOFA_TEMPLATE(ScriptForceField,DataTypes));

    typedef typename DataTypes::Real             Real;
    typedef typename DataTypes::Coord            Coord;
    typedef typename DataTypes::Deriv            Deriv;
    typedef typename DataTypes::VecCoord         VecCoord;
    typedef typename DataTypes::VecDeriv         VecDeriv;
    typedef core::objectmodel::Data<VecCoord>    DataVecCoord;
    typedef core::objectmodel::Data<VecDeriv>    DataVecDeriv;

    PyObject* scriptForceFieldInstance() const {return m_ScriptForceFieldInstance;}

    bool isDerivedFrom(const std::string& name, const std::string& module = "__main__");
    void doLoadScript();
    void refreshBinding();
protected:
    PythonScriptForceField();
    virtual ~PythonScriptForceField();

public:
    /// Parse the given description to assign values to this object's fields and potentially other parameters
    virtual void parse ( sofa::core::objectmodel::BaseObjectDescription* arg );

protected:
    /// @name Script interface
    ///   Function that need to be implemented for each script language
    /// Typically, all "script_*" functions call the corresponding "*" function of the script, if it exists
    /// @{

    virtual void loadScript();

    virtual void script_addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v);
    // appelle en python : f += addForce(x,v);
    virtual void script_addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx );
    // appelle en python : df += addDForce(kFactor, bFactor, dx);
    virtual SReal script_getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x) const;
    // appelle en python : e = getPotentialEnergy(x);
    virtual void script_addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix );
    // appelle en python : dMatrix = addKToMatrix(kFactor);
    // avec dMatrix= { {i,j,valueToAdd},{....}.....} la liste des valeurs à ajouter à matrix

    /// @}

public:
    sofa::core::objectmodel::DataFileName       m_filename;
    sofa::core::objectmodel::Data<std::string>  m_classname;
    sofa::core::objectmodel::Data< helper::vector< std::string > >  m_variables; // array of string variables (equivalent to a c-like argv), while waiting to have a better way to share variables
    sofa::core::objectmodel::Data<bool>         m_timingEnabled;
    sofa::core::objectmodel::Data<bool>         m_doAutoReload;

protected:
    sofa::helper::system::FileEventListener* m_filelistener ;

    PyObject *m_ScriptForceFieldClass;      // class implemented in the script to use to instanciate the python forcefield
    //PyObject *m_ScriptControllerInstanceDict;  // functions dictionnary
    PyObject *m_ScriptForceFieldInstance;   // instance of m_ScriptForceFieldClass

    // optionnal script entry points:
    PyObject *m_Func_addForce;
    PyObject *m_Func_addDForce;
    PyObject *m_Func_getPotentialEnergy;
    PyObject *m_Func_addKToMatrix;
};

using namespace sofa::defaulttype;


} // namespace behavior

} // namespace core

} // namespace sofa

#ifdef SOFA_WITH_DOUBLE
typedef sofa::core::behavior::PythonScriptForceField<sofa::defaulttype::Vec3dTypes> PythonScriptForceField3d;
#endif
#ifdef SOFA_WITH_FLOAT
#endif


#endif // PYTHONFORCEFIELD_H
