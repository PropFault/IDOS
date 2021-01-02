#pragma once
#include "idoId.h"
#include "IDOManager.h"
#include "IDOSException.h"
#include <ostream>
namespace idos{
    class UntypedRef{
    private:
        ID identifier;
        IDOManager* manager;
    public:
        UntypedRef()
        :manager(nullptr), identifier(0){}
        UntypedRef(IDOManager& manager, const ID identifier)
        :manager(&manager), identifier(identifier){
        }
        virtual bool hasValue(){
            if(manager == nullptr)
                throw IDOSException("REFERENCE UNINITIALISED");
            return manager->hasValue(identifier);
        }

        virtual IDO* getValue()const{
            if(manager == nullptr)
                throw IDOSException("REFERENCE UNINITIALISED");
            return manager->at(identifier);
        }
        
        virtual const ID& getIdentifier()const{
            if(manager == nullptr)
                throw IDOSException("REFERENCE UNINITIALISED");
            return this->identifier;
        }

        virtual void setIdentifier(const ID& id){
            this->identifier = id;
        }

        bool operator==(const UntypedRef& othr)const{
            return this->identifier == othr.identifier && (&manager) ==  (&othr.manager);
        }
        friend std::ostream& operator<<(std::ostream& o, const UntypedRef& othr);
    };

    
    template<typename T>
    class Ref : public UntypedRef{
    public:
        Ref(){}
        Ref(IDOManager& manager, const ID identifier)
        :UntypedRef(manager,identifier){
        }

        Ref(const UntypedRef &ref)
        :UntypedRef(ref){}

        T* getValue()const override{
            return dynamic_cast<T*>(UntypedRef::getValue());
        }
        

    };
    inline  void to_json(nlohmann::json& j, const UntypedRef& p) {
        try{
            j = p.getIdentifier();
        }catch(const IDOSException &ex){

        }
    }

    inline  void from_json(const nlohmann::json& j, UntypedRef& p) {
        try
        {
            p.setIdentifier(j.get<ID>());
        }
        catch(const IDOSException& e)
        {
        }
        
    }

    inline std::ostream& operator<<(std::ostream& o, const UntypedRef& othr){
        o << "REF#";
        try{
            o << othr.getIdentifier();
            o << " -> ";
            o << *othr.getValue();
        }catch(const IDOSException& ex){
            o << ex.what();
        }
        return o;
    }

    class RefFactory{
    private:
        IDOManager &mgr;
    public:
        UntypedRef createRef(const ID &id){
            return UntypedRef(mgr, id);
        }

        template<typename T>
        Ref<T> createRef(const ID &id){
            return Ref<T>(mgr, id);
        }

        template<typename T>
        Ref<T> typeRef(const UntypedRef &blueprint){
            return Ref<T>(blueprint);
        }
    };
}

namespace std
{
    template<typename T> struct hash<idos::Ref<T>>
    {
        std::size_t operator()(idos::Ref<T> const& s) const noexcept
        {
            std::size_t h1 = std::hash<idos::ID>{}(s.getIdentifier());
            return h1;
        }
    };
}
///TO-DO:
// solve problem of idomanager being required for reference construction
