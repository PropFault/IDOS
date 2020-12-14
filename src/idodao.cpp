#include "idodao.h"
#include <nlohmann/json.hpp>
#include <istream>
#include <string>
#include "IDOManager.h"
#include <iostream>
#include "IDOSException.h"
#include <sstream>
#include "ido.h"
using namespace idos;

IDODAO::IDODAO(IDOManager &manager)
    : manager(manager) {}

void IDODAO::pregenerateAliases(const nlohmann::json &json)
{
    try
    {
        auto alias = json.at(IDO::PROP_ALIAS).get<std::string>();
        this->manager.registerAlias(alias, this->manager.generateNewID());
    }
    catch (const nlohmann::json::out_of_range &ex)
    {
    }
    for (auto &el : json.items())
    {
        if (el.value().type() == nlohmann::json::value_t::object)
            pregenerateAliases(el.value());
    }
}

IDO::ID IDODAO::loadIDOSFromJSON(const nlohmann::json &json)
{
    DataPack dataPack(json);
    for (auto &el : json.items())
    {
        if (el.value().type() == nlohmann::json::value_t::object)
            dataPack[el.key()] = loadIDOSFromJSON(el.value());
        if(el.value().type() == nlohmann::json::value_t::array){
            int idx = 0;
            for(auto &i : el.value().items()){
                if(i.value().type() == nlohmann::json::value_t::object){
                    dataPack[el.key()][idx] = loadIDOSFromJSON(i.value());
                }
                idx++;
            }
        }
    }
    std::cout<<dataPack<<std::endl;
    try
    {
        return manager.instantiateIDO(json.at(IDO::PROP_TYPE), manager.getIDForAlias(json.at(IDO::PROP_ALIAS)), dataPack).first;
    }
    catch (const nlohmann::json::out_of_range &exo)
    {
        try
        {
            return manager.instantiateIDO(json.at(IDO::PROP_TYPE), dataPack).first;
        }
        catch (const nlohmann::json::out_of_range &ex)
        {
            return manager.getIDForAlias(json.at(IDO::PROP_REF));
        }
    }
    throw idos::IDOSException("File is empty!");
}


DataPack prepareIDsForSerialization(DataPack &pack, idos::IDOManager & manager, bool isRoot = true){
    for(auto& elem : pack.items()){
        if(elem.value().type() == nlohmann::json::value_t::object){
            pack[elem.key()] = prepareIDsForSerialization(elem.value(), manager, false); 
        }
        if(elem.value().type() == nlohmann::json::value_t::array){
            int idx = 0;
            for(auto &i : elem.value().items()){
                if(i.value().type() == nlohmann::json::value_t::object){
                    pack[elem.key()][idx] = prepareIDsForSerialization(i.value(), manager, false);
                }
                idx++;
            }
        }
    }
    std::cout<<"Collapsed to: " << pack<<std::endl;
    try{
        auto id = pack.get<IDO::ID>();
        try{
            DataPack nPack;
            nPack["ref"] = manager.getAliasForID(id);
            return nPack;
        }catch(const IDOSException &noAlias){
            try{
                auto packed = manager.at(id)->pack();
                std::cout<<"PACKED TO: " << packed << std::endl;
                return prepareIDsForSerialization(packed, manager, isRoot);
            }catch(const std::out_of_range &noIDOS){
                throw IDOSException("[SERIALIZATION]: Failed resolving IDOS: Detected ID which does not have an alias and does not exist in the provided manager");
            }
        }
    }catch(const nlohmann::json::out_of_range &fullErr){
        if(!isRoot)
            std::cout<<"Could not resolve id in part: " << pack << std::endl;
            //throw IDOSException("[SERIALIZATION]: Failed resolving IDOS: DataPack contains nested object-type that does not provide an ID property.");
    
       return pack;
    }
    return pack;
}
void IDODAO::saveToFile(const std::string & path, IDO* value){
    std::stringstream outData;
    auto pack = value->pack();
    prepareIDsForSerialization(pack,manager);
    outData << pack;
    
    std::ofstream outFile(path);
    if(outFile.is_open()){
        outFile<<outData.rdbuf();
    }
    outFile.close();
    outData.clear();

}

//Note:
///The functions above can't be implemented because it is not possible to reverse the conversion from alias to ID (ATM). Writing direct IDs into the file 
///Would be bad since the IDs are random.
///Solution needs to be found
///Solution ideas:
///Instead of directly replacing the reference structure ({"ref": "some alias"}) with the ID, you simply replace the contents of the ref tag.
////{"ref" :"someAlias"} -> {"ref": "someID"}
////This would allow us to recognise the IDs and reverse search for the original alias. 
////Issue: Additional "useless" structure would add additional step to unpacking process. 
