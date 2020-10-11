#pragma once
#include "IDOManager.h"
namespace idos{
    template<typename T>
    class ManagedValue{
    private:
        IDOManager& manager;
        const IDOManager::ID& identifier;
    public:
        ManagedValue(IDOManager& manager,const IDOManager::ID& identifier)
        :manager(manager), identifier(identifier){}

        bool hasValue(){
            return manager.hasValue(identifier);
        }

        T* getValue(){
            return (T*)manager.at(identifier);
        }
        
    };
}