#pragma once
#include "IDOManager.h"
#include "IDOSException.h"
#include "Ref.h"
#include "datapack.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "datapackJSONConverter.h"
namespace idos{
    class IDODAO{
    private:
        IDOManager &manager; 
        void pregenerateAliases(const nlohmann::json &json);
        ID loadIDOSFromJSON(const nlohmann::json &json);
    public:
        IDODAO(IDOManager &manager);

        UntypedRef loadFromFile(const char *filePath){
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
                return UntypedRef(this->manager, loadIDOSFromJSON(json));
            }catch(const nlohmann::detail::parse_error &parseErr){
                throw idos::IDOSException(std::string("Error parsing file! ") + parseErr.what());
            }
        }
        void saveToFile(const std::string & path, UntypedRef &value){
            this->saveToFile(path, value.getValue());
        }
        void saveToFile(const std::string & path, IDO* value);
        
    };


}