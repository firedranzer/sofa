#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject ;

class OBJLoader : public BaseObject
{
public:
    SOFA_CLASS(OBJLoader, BaseObject) ;

    virtual void init() override ;
} ;

void OBJLoader::init()
{
    msg_warning() << "Hello world." ;
}


#include <sofa/core/ObjectFactory.h>
using sofa::core::ObjectFactory ;
using sofa::core::ObjectCreator ;
using sofa::core::RegisterObject ;

class RegisterComponent
{
protected:
    /// Class entry being constructed
    ObjectFactory::ClassEntry entry;

public:
    /// Start the registration by giving the description of this class.
    RegisterComponent(const std::string& name){
        entry.className = name ;
    }

    /// Add an namespace to this component
    RegisterComponent& withinNamespace(const std::string& ns)
    {
        //TODO(dmarchal) this is not correct. entry should store the ns.
        //It could be equivalent to the getTarget() function in ObjectCreator.
        if(! ns.empty())
            entry.moduleName = ns;
        return *this;
    }

    /// Add more descriptive text about this class
    RegisterComponent& withDescription(const std::string& val)
    {
        entry.description = val;
        return *this;
    }


    /// Add a template instanciation of this class.
    ///
    /// \param defaultTemplate    set to true if this should be the default instance when no template name is given.
    template<class RealObject>
    RegisterComponent& addCreatorFor(bool defaultTemplate=false)
    {
        RealObject* p = NULL;
        std::string templatename = RealObject::templateName(p);

        if (defaultTemplate)
            entry.defaultTemplate = templatename;

        return addCreator(templatename, ObjectFactory::Creator::SPtr(new ObjectCreator<RealObject>));
    }

    /// This is the final operation that will actually commit the additions to the ObjectFactory.
    operator int();

private:
    /// Add a creator able to instance this class with the given templatename.
    ///
    /// See the add<RealObject>() method for an easy way to add a Creator.
    RegisterComponent& addCreator(const std::string& templatename, ObjectFactory::Creator::SPtr creator);
};

RegisterComponent& RegisterComponent::addCreator(const std::string& templatename, ObjectFactory::Creator::SPtr creator)
{
    if (entry.creatorMap.find(templatename) != entry.creatorMap.end())
    {
        msg_error("ObjectFactory") << "Component already registered: " << entry.className << "<" << templatename << ">";
        return *this;
    }

    entry.creatorMap[templatename] =  creator;
    return *this;
}


RegisterComponent::operator int()
{
    if (entry.className.empty())
    {
        msg_error("ObjectFactory") << "Registering a component with an empty name.";
        return 0 ;
    }
    msg_info("ObjectFactory") << "Registering: " << entry.moduleName+"."+entry.className ;
    ObjectFactory::ClassEntry& reg = ObjectFactory::getInstance()->getEntry(entry.moduleName+"."+entry.className);
    reg.description += entry.description;
    reg.authors += entry.authors;
    reg.license += entry.license;
    reg.className = entry.className;
    reg.moduleName = entry.moduleName;

    if (!entry.defaultTemplate.empty())
    {
        if (!reg.defaultTemplate.empty())
        {
            msg_warning("ObjectFactory") << "Default template for class " << entry.className << " already registered (" << reg.defaultTemplate << "), do not register " << entry.defaultTemplate << " as the default";
        }
        else
        {
            reg.defaultTemplate = entry.defaultTemplate;
        }
    }
    for (ObjectFactory::CreatorMap::iterator itc = entry.creatorMap.begin(), itcend = entry.creatorMap.end(); itc != itcend; ++itc)
    {
        if (reg.creatorMap.find(itc->first) != reg.creatorMap.end())
        {
            msg_warning("ObjectFactory") << "Class already registered: " << itc->first;
        }
        else
        {
            reg.creatorMap.insert(*itc);
        }
    }
    for (std::set<std::string>::iterator it = entry.aliases.begin(), itend = entry.aliases.end(); it != itend; ++it)
    {
        if (reg.aliases.find(*it) == reg.aliases.end())
        {
            ObjectFactory::getInstance()->addAlias(*it,entry.className);
        }
    }

    ObjectFactory::ClassEntry& reg2 = ObjectFactory::getInstance()->getEntry(entry.moduleName+"."+entry.className);

    return 1;

}

static int OBJLoaderClass =
        RegisterComponent("ObjLoader")
        .withinNamespace("Sofa.Loader.Mesh")
        .withDescription("Load a surface mesh from an 'obj (Wavefront)' file.")
        .addCreatorFor< OBJLoader >() ;
