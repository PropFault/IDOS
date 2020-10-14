#pragma once
#include <string>
#include "datapack.h"
#include <random>

namespace idos
{
    class IDOManager;
    class IDO
    {
    private:
        std::string type;
        std::string displayName;

    protected:
        virtual DataPack _pack() const = 0;
        virtual void _unpack(const DataPack &pack) = 0;

    public:
        typedef uint64_t ID;

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
} // namespace idos