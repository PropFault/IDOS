#include "idodao.h"
#include <nlohmann/json.hpp>
#include <istream>
#include <string>
#include "IDOManager.h"
#include <iostream>
#include "IDOSException.h"
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
    }
    try
    {
        return manager.instantiateIDO(json.at(IDO::PROP_TYPE), manager.getAliasID(json.at(IDO::PROP_ALIAS)), dataPack).first;
    }
    catch (const nlohmann::json::out_of_range &exo)
    {
        try
        {
            return manager.instantiateIDO(json.at(IDO::PROP_TYPE), dataPack).first;
        }
        catch (const nlohmann::json::out_of_range &ex)
        {
            return manager.getAliasID(json.at(IDO::PROP_REF));
        }
    }
    throw idos::IDOSException("File is empty!");
}

/*template<typename T>
void saveToFile(const ManagedValue<T> &value){

}
void saveToFile(IDO* value);*/