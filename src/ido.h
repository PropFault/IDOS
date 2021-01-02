#pragma once
#include <string>
#include <random>
#include <nlohmann/json.hpp>
#include "idoId.h"
namespace idos
{
    class DataPack;
    class IDO
    {
    private:
        std::string type;
        std::string displayName;

    protected:
        virtual DataPack _pack() const = 0;
        virtual void _unpack(const DataPack &pack) = 0;

    public:
        IDO(const std::string &type);
        IDO(const std::string &type, const std::string &displayName);
        
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

        const std::string &getType() const;
        const std::string &getDisplayName() const;
        void setDisplayName(const std::string &name);

        virtual ~IDO();
    };

   
    inline std::ostream &operator<<(std::ostream &out, const IDO &dat)
    {
        //out << dat.pack();
        out << dat.getType() << "|" << dat.getDisplayName();
        return out;
    };

} // namespace idos

namespace std
{
    template <>
    struct hash<idos::ID>
    {
        std::size_t operator()(idos::ID const &s) const noexcept
        {
            std::size_t h1 = std::hash<uint64_t>{}(s.value);
            return h1;
        }
    };
} // namespace std