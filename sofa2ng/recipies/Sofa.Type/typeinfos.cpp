#include <typeinfo>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include <sofa/type/typeinfos.h>

namespace sofa
{

namespace type
{

/// Decode the type's name to a more readable form if possible
std::string SOFA_TYPE_API gettypename(const std::type_info& t)
{
    std::string name;
#ifdef __GNUC__
    char* realname = NULL;
    int status;
    realname = abi::__cxa_demangle(t.name(), 0, 0, &status);
    if (realname!=NULL)
    {
        int length = 0;
        while(realname[length] != '\0')
        {
            length++;
        }
        name.resize(length);
        for(int i=0; i<(int)length; i++)
            name[i] = realname[i];
        free(realname);
    }
#else
    name = t.name();
#endif
    // Remove namespaces
    for(;;)
    {
        std::string::size_type pos = name.find("::");
        if (pos == std::string::npos) break;
        std::string::size_type first = name.find_last_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_",pos-1);
        if (first == std::string::npos) first = 0;
        else first++;
        name.erase(first,pos-first+2);
    }
    //Remove "class "
    for(;;)
    {
        std::string::size_type pos = name.find("class ");
        if (pos == std::string::npos) break;
        name.erase(pos,6);
    }
    //Remove "struct "
    for(;;)
    {
        std::string::size_type pos = name.find("struct ");
        if (pos == std::string::npos) break;
        name.erase(pos,7);
    }
    return name;
}


} // namespace type
} // namespace sofa
