#pragma once
#include "IDOManager.h"
#include "IDOSException.h"
#include "ManagedValue.h"
#include <fstream>
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
            if(!file.is_open())
                throw idos::IDOSException(std::string("Error opening file ") + filePath + "!");
            try{
                nlohmann::json json = nlohmann::json::parse(file);
                //Step 1 - pregenerate Aliases
                //Instantiate everything unintialised and register aliases
                pregenerateAliases(json);
                //Step 2 - initialise
                //Build datapacks for all objects and init
                return ManagedValue<T>(this->manager, loadIDOSFromJSON(json));
            }catch(const nlohmann::detail::parse_error &parseErr){
                throw idos::IDOSException(std::string("Error parsing file! ") + parseErr.what());
            }
        }
        template<typename T>
        void saveToFile(const std::string & path, ManagedValue<T> &value){
            this->saveToFile(path, value.getValue());
        }
        void saveToFile(const std::string & path, IDO* value);
        
    };
}