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
#include <regex>

#include <sofa/core/ObjectFactory.h>
using sofa::core::ObjectFactory ;
using sofa::core::RegisterObject ;

#include <sofa/helper/system/PluginManager.h>
using sofa::helper::system::PluginManager;

#include "From.h"


namespace sofa
{

namespace component
{

namespace misc
{

int FromPluginClass = core::RegisterObject("Import a plugin & load components")
        .add< From >();

From::From()
    : d_plugin( initData(&d_plugin, std::string(""), "plugin", "plugin name to components import from"))
     ,d_import( initData(&d_import, std::string("*"), "import", "a regexp for the component name (default='*')"))
{
    /// print log by default, to identify which pluging is responsible in case of a crash during loading
    this->f_printLog.setValue(true);
}

void From::parse(sofa::core::objectmodel::BaseObjectDescription* arg)
{
    Inherit1::parse(arg);

    if(!d_plugin.isSet() )
    {
        msg_error() << "The mandatory attribute 'plugin' is not set." ;
        return ;
    }
    loadPlugin();
}

void From::loadPlugin()
{
    msg_info() << "Registering an alias from " << d_plugin.getValue()+"."+d_import.getValue() ;

    PluginManager* pluginManager = &PluginManager::getInstance();

    /// First try to load the plugin.
    if( !pluginManager->pluginIsLoaded(d_plugin.getValue()))
    {
        if(! pluginManager->loadPlugin(d_plugin.getValue()))
        {
            msg_error() << "Cannot load a plugin named '"<<d_plugin.getValue()<<"'." ;
            return ;
        }
        pluginManager->init();
    }

    for(auto& component : ObjectFactory::getInstance()->getAllEntries())
    {
        if(!component->moduleName.empty())
            std::cout << "'" << d_plugin.getValue() << "' vs '" << component->moduleName << "'" << std::endl ; ;
        if(d_plugin.getValue() == component->moduleName && d_import == "*" )
        {
            if( ObjectFactory::getInstance()->addAlias(d_plugin.getValue()+"."+component->className, component->className) )
            {
                msg_info() << "Registering an alias from " << d_plugin.getValue()+"."+component->className << " to " << component->className ;
            }
            else
            {
                msg_warning() << "Unable to register an alias between " << d_plugin.getValue()+"."+component->className << " and " << component->className ;
            }
        }
    }
}

}

}

}
