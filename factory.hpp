#ifndef __CLANY_FACTORY_HPP__
#define __CLANY_FACTORY_HPP__

#include <map>
#include <memory>
#include <functional>
#include "clany/clany_macros.h"

_CLANY_BEGIN
template<typename T>
class Factory
{
public:
    template<typename... Args>
    shared_ptr<T> operator() (Args&&... args)
    { return make_shared<T>(forward<Args>(args)...); };
};


template<typename BaseType, typename IDType = string>
class ObjFactory
{
private:
    typedef shared_ptr<BaseType> BasePtr;
    typedef function<BasePtr()> CreateFunc;

    ObjFactory() {};
    ObjFactory(const ObjFactory&) = delete;
    ObjFactory& operator= (const ObjFactory&) = delete;

    static ObjFactory& instance()
    {
        static ObjFactory obj_factory;
        return obj_factory;
    }
    map<IDType, CreateFunc> creators_;

public:
    typedef IDType IDType;

    static bool addType(const IDType& ID, const CreateFunc& creator)
    {
        auto& creators = instance().creators_;
        if (creators.find(ID) == creators.end()) {
            creators.insert({ID, creator});
            return true;
        }

        return false;
    }

    static bool removeType(const IDType& ID)
    {
        auto& creators = instance().creators_;
        if (creators.find(ID) != creators.end()) {
            creators.erase(ID);
            return true;
        }

        return false;
    }

    static BasePtr create(const IDType& ID)
    {
        auto& creators = instance().creators_;
        auto iter = creators.find(ID);
        if (iter != creators.end()) {
            return (iter->second)();
        }

        return nullptr;
    }
};
_CLANY_END

// Use these macro in your .cpp file
#define REGISTER_TO_FACTORY(BaseType, DerivedType) \
namespace { \
    const bool add_shape = clany::ObjFactory<BaseType>::addType(#DerivedType, Factory<DerivedType>()); \
}


// Need to use "typedef BaseType base" in derived class
#define REGISTER_TYPE_TO_FACTORY(DerivedType) \
namespace { \
    const bool add_shape = clany::ObjFactory<DerivedType::base>::addType(#DerivedType, Factory<DerivedType>()); \
}

#endif // __CLANY_FACTORY_HPP__