#pragma once
#include "IDOManager.h"
namespace idos{
    template<typename T>
    class Ref{
    private:
        IDO::ID identifier;
    public:
        Ref(const IDO::ID identifier)
        :identifier(identifier){
        }

        bool hasValue(IDOManager& manager){
            return manager.hasValue(identifier);
        }

        T* getValue(IDOManager& manager){
            return dynamic_cast<T*>(manager.at(identifier));
        }
        
        const IDO::ID& getIdentifier(){
            return this->identifier;
        }
    };
}