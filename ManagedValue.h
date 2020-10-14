#pragma once
#include "IDOManager.h"
#include <iostream>
namespace idos{
    template<typename T>
    class ManagedValue{
    private:
        IDOManager& manager;
        IDO::ID identifier;
    public:
        ManagedValue(IDOManager& manager,const IDO::ID &identifier)
        :manager(manager), identifier(identifier){
        }

        bool hasValue(){
            return manager.hasValue(identifier);
        }

        T* getValue(){
            return dynamic_cast<T*>(manager.at(identifier));
        }
        
        const IDO::ID& getIdentifier(){
            return this->identifier;
        }
    };
}