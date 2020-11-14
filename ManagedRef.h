#pragma once
#include "Ref.h"
namespace idos{
    template<typename T>
    class ManagedRef{
    private:
        IDOManager& manager;
        Ref<T> ref;
    public:
        ManagedRef(IDOManager& manager,const Ref<T> &ref)
        :manager(manager), ref(ref){
        }

        bool hasValue(){
            return ref.hasValue(manager);
        }

        T* getValue(){
            return ref.getValue(manager);
        }
        
        const IDO::ID& getIdentifier(){
            return ref.getIdentifier();
        }
    };
}