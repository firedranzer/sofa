#ifndef SOFA_TYPE_TYPEINFOS_H
#define SOFA_TYPE_TYPEINFOS_H

#include <sofa/type/config.h>
#include <string>
#include <typeinfo>

namespace sofa
{

namespace type
{

    /// Decode the type's name to a more readable form if possible
    std::string SOFA_TYPE_API gettypename(const std::type_info& t);

}
}

#endif // 
