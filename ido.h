#pragma once
#include <string>
#include "datapack.h"
namespace idos{
    class IDO{
    private:
        std::string type;
        std::string displayName;
    protected:
        virtual DataPack _pack()const = 0;
        virtual void _unpack(const DataPack &pack) = 0;
    public:
        const static std::string PROP_TYPE;
        const static std::string PROP_DISPLAY_NAME;
        virtual IDO* clone() = 0;
        DataPack pack()const;
        template<typename t>
        t* as(){
            return (t*)this;
        }
        void unpack(const DataPack &pack);
        virtual ~IDO();
    };
}