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
#ifndef PYTHONFORCEFIELD_INL
#define PYTHONFORCEFIELD_INL

#include "PythonEnvironment.h"
#include "PythonScriptForceField.h"
//#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/helper/logging/Messaging.h>

#include <sofa/helper/system/FileMonitor.h>
using sofa::helper::system::FileMonitor ;
using sofa::helper::system::FileEventListener ;

namespace sofa
{

namespace core
{

namespace behavior
{

template<class DataTypes>
class MyFileEventListener : public FileEventListener
{
    PythonScriptForceField<DataTypes>* m_forcefield ;
public:
   MyFileEventListener(PythonScriptForceField<DataTypes>* psff){
        m_forcefield = psff ;
    }

    virtual ~MyFileEventListener(){}

    virtual void fileHasChanged(const std::string& filepath){
        /// This function is called when the file has changed. Two cases have
        /// to be considered if the script was already loaded once or not.
        if(!m_forcefield->scriptForceFieldInstance()){
           m_forcefield->doLoadScript();
        }else{
            std::string file=filepath;
            SP_CALL_FILEFUNC(const_cast<char*>("onReimpAFile"),
                             const_cast<char*>("s"),
                             const_cast<char*>(file.data()));

            m_forcefield->refreshBinding();
        }
    }
};





template <class DataTypes>
PythonScriptForceField<DataTypes>::PythonScriptForceField()
    : ScriptForceField<DataTypes>()
    , m_filename(initData(&m_filename, "filename",
                        "Python script filename"))
    , m_classname(initData(&m_classname, "classname",
                         "Python class implemented in the script to instanciate for the controller"))
    , m_variables(initData(&m_variables, "variables",
                           "Array of string variables (equivalent to a c-like argv)" ) )
    , m_timingEnabled(initData(&m_timingEnabled, true, "timingEnabled",
                               "Set this attribute to true or false to activate/deactivate the gathering"
                               " of timing statistics on the python execution time. Default value is set"
                               "to true." ))
    , m_doAutoReload( initData( &m_doAutoReload, false, "autoreload",
                                "Automatically reload the file when the source code is changed. "
                                "Default value is set to false" ) )
    , m_ScriptForceFieldClass(0)
    , m_ScriptForceFieldInstance(0)
{
    m_filelistener = new MyFileEventListener<DataTypes>(this) ;
}

template <class DataTypes>
PythonScriptForceField<DataTypes>::~PythonScriptForceField()
{

}


template <class DataTypes>
void PythonScriptForceField<DataTypes>::parse(sofa::core::objectmodel::BaseObjectDescription *arg)
{
    ScriptForceField<DataTypes>::parse(arg);
}



template <class DataTypes>
void PythonScriptForceField<DataTypes>::loadScript()
{
    logmsg_info(this) << "PythonScriptForceField::loadScript : class " << m_classname.getValue() << " in file " << m_filename.getValue();

    if(m_doAutoReload.getValue())
    {
        FileMonitor::addFile(m_filename.getFullPath(), m_filelistener) ;
    }
/*
    // if the filename is empty, the controller is supposed to be in an already loaded file
    // otherwise load the controller's file
    if( m_filename.isSet() && !m_filename.getRelativePath().empty() && !PythonEnvironment::runFile(m_filename.getFullPath().c_str()) )
    {
        SP_MESSAGE_ERROR( getName() << " object - "<<m_filename.getFullPath().c_str()<<" script load error." )
                return;
    }

    // classe
    PyObject* pDict = PyModule_GetDict(PyImport_AddModule("__main__"));
    m_ScriptForceFieldClass = PyDict_GetItemString(pDict,m_classname.getValueString().c_str());
    if (!m_ScriptForceFieldClass)
    {
        SP_MESSAGE_ERROR( getName() << " load error (class \""<<m_classname.getValueString()<<"\" not found)." )
                return;
    }

    // check that the class is a subclass of PythonScriptForceField
    if (1!=PyObject_IsSubclass(m_ScriptForceFieldClass,(PyObject*)&SP_SOFAPYTYPEOBJECT(PythonScriptForceField3d)))
    {
        // LOAD ERROR
        SP_MESSAGE_ERROR( getName() << " load error (class \""<<m_classname.getValueString()<<"\" does not inherit from \"Sofa.PythonScriptForceField\")." )
                return;
    }

    // créer l'instance de la classe
    m_ScriptForceFieldInstance = BuildPySPtr<Base>(this,(PyTypeObject*)m_ScriptForceFieldClass);

    if (!m_ScriptForceFieldInstance)
    {
        SP_MESSAGE_ERROR( getName() << " load error (class \""<<m_classname.getValueString()<<"\" instanciation error)." )
                return;
    }

    refreshBinding();
*/
}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::doLoadScript()
{
    loadScript() ;
}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::refreshBinding()
{
    /*
    TODO
    BIND_OBJECT_METHOD(onLoaded)
    BIND_OBJECT_METHOD(createGraph)
    BIND_OBJECT_METHOD(initGraph)
    BIND_OBJECT_METHOD(bwdInitGraph)
    BIND_OBJECT_METHOD(onKeyPressed)
    BIND_OBJECT_METHOD(onKeyReleased)
    BIND_OBJECT_METHOD(onMouseButtonLeft)
    BIND_OBJECT_METHOD(onMouseButtonRight)
    BIND_OBJECT_METHOD(onMouseButtonMiddle)
    BIND_OBJECT_METHOD(onMouseWheel)
    BIND_OBJECT_METHOD(onBeginAnimationStep)
    BIND_OBJECT_METHOD(onEndAnimationStep)
    BIND_OBJECT_METHOD(storeResetState)
    BIND_OBJECT_METHOD(reset)
    BIND_OBJECT_METHOD(cleanup)
    BIND_OBJECT_METHOD(onGUIEvent)
    BIND_OBJECT_METHOD(onScriptEvent)
    BIND_OBJECT_METHOD(draw)
    BIND_OBJECT_METHOD(onIdle)
    */
}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::script_addForce(const MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v)
// appel en python : f += addForce(x,v);
{

}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::script_addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx )
// appel en python : df += addDForce(kFactor, bFactor, dx);
{

}

template <class DataTypes>
SReal PythonScriptForceField<DataTypes>::script_getPotentialEnergy(const MechanicalParams* mparams, const DataVecCoord& x) const
// appel en python : e = getPotentialEnergy(x);
{
    return 0.f;
}

template <class DataTypes>
void PythonScriptForceField<DataTypes>::script_addKToMatrix(const MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix )
// appel en python : dMatrix = addKToMatrix(kFactor);
// avec dMatrix= { {i,j,valueToAdd},{....}.....} la liste des valeurs à ajouter à matrix
{

}


} // namespace behavior

} // namespace core

} // namespace sofa

#endif // PYTHONFORCEFIELD_INL
