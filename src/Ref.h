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

        T* getValue(IDOManager& manager)const{
            return dynamic_cast<T*>(manager.at(identifier));
        }
        
        const IDO::ID& getIdentifier()const{
            return this->identifier;
        }

    };
    template<typename T>
    bool operator==(const Ref<T>& lhs, const Ref<T>& rhs) {
        return lhs.getIdentifier() == rhs.getIdentifier();
    }
    template<typename T>
    inline  void to_json(nlohmann::json& j, const Ref<T>& p) {
        j = p.getIdentifier();
    }

    template<typename T>
    inline  void from_json(const nlohmann::json& j, Ref<T>& p) {
        p.identifier = j.get<IDO::ID>();
    }
}

namespace std
{
    template<typename T> struct hash<idos::Ref<T>>
    {
        std::size_t operator()(idos::Ref<T> const& s) const noexcept
        {
            std::size_t h1 = std::hash<idos::IDO::ID>{}(s.getIdentifier());
            return h1;
        }
    };
}