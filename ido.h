#pragma once
#include <string>
#include "datapack.h"
#include <random>
#include <nlohmann/json.hpp>

namespace idos
{
    class IDOManager;
    class ID_T{
        public:
        uint64_t value;
        ID_T(const uint64_t& wrap)
        :value(wrap){}
        ID_T()
        :ID_T(0){}
        operator uint64_t(){return value;}
    };
    inline bool operator==(const ID_T& lhs, const ID_T& rhs) {
        return lhs.value == rhs.value;
    }
 
    class IDO
    {
    private:
        std::string type;
        std::string displayName;

    protected:
        virtual DataPack _pack() const = 0;
        virtual void _unpack(const DataPack &pack) = 0;

    public:
        typedef ID_T ID;

        const static std::string PROP_TYPE;
        const static std::string PROP_DISPLAY_NAME;
        const static std::string PROP_ALIAS;
        const static std::string PROP_REF;
        virtual IDO *clone() = 0;
        DataPack pack() const;
        template <typename t>
        t *as()
        {
            return (t *)this;
        }
        void unpack(const DataPack &pack);
        virtual ~IDO();
    };
    
       inline  void to_json(nlohmann::json& j, const IDO::ID& p) {
            j = nlohmann::json{ {"id", p.value} };
        }

       inline  void from_json(const nlohmann::json& j, IDO::ID& p) {
            p.value = j.at("id").get<uint64_t>();
        }

} // namespace idos

namespace std
{
    template<> struct hash<idos::ID_T>
    {
        std::size_t operator()(idos::ID_T const& s) const noexcept
        {
            std::size_t h1 = std::hash<uint64_t>{}(s.value);
            return h1;
        }
    };
}