#pragma once
#include "IDOManager.h"
#include "ManagedValue.h"
namespace idos{
    class IDODAO{
    private:
        IDOManager &manager; 
        void pregenerateAliases(const nlohmann::json &json);
        IDO::ID loadIDOSFromJSON(const nlohmann::json &json);

    public:
        IDODAO(IDOManager &manager);
        template <typename T>
        ManagedValue<T> loadFromFile(const char *filePath)
        {
            std::ifstream file(filePath);
            nlohmann::json json = nlohmann::json::parse(file);
            //Step 1 - pregenerate Aliases
            //Instantiate everything unintialised and register aliases
            pregenerateAliases(json);
            //Step 2 - initialise
            //Build datapacks for all objects and init
            return ManagedValue<T>(this->manager, loadIDOSFromJSON(json));
        }
        /*template<typename T>
        void saveToFile(const ManagedValue<T> &value);
        void saveToFile(IDO* value);*/
        
    };
}