#include "IDOManager.h"
#include "IDOSException.h"
#include <memory>
#include <iostream>

using namespace idos;

const std::unordered_map<std::string, ID> &IDOManager::getAliasList()const{
    return this->alias;
}
const std::unordered_map<std::string, IDO*> &IDOManager::getTypes()const{
    return this->types;
}

IDOManager::Value IDOManager::getAlias(const std::string &alias){
    auto id = this->getIDForAlias(alias);
    return IDOManager::Value(id, this->at(id));
}

ID IDOManager::getIDForAlias(const std::string& alias)const{
    return this->alias.at(alias);
}
std::string IDOManager::getAliasForID(const ID& id)const{
    for(auto &entry : this->alias){
        if(entry.second == id)
        return entry.first;
    }
    throw IDOSException(std::string("No Alias for given ID found!"));
}



bool IDOManager::hasAlias(const std::string &alias)const{
    try{
        this->alias.at(alias);
        return true;
    }catch(const std::out_of_range& ex)
    {
        return false;
    }
}
ID IDOManager::generateNewID(){
   /* IDO::ID id;
    id.value = this->dis(this->rand);*/
    return this->dis(this->rand);
}

IDOManager::Value IDOManager::instantiateIDO(const std::string &type, ID id, DataPack &init){
    IDO* newValue = nullptr;
    try{
        newValue = this->types.at(type)->clone();
    }catch(const std::out_of_range &ex){
        throw IDOSException("TRIED INSTANTIATING IDO OF UNKNOWN TYPE!");
    }
    newValue->unpack(init);
    this->instances.insert(std::pair(id, newValue));
    IDOManager::Value wrap(id, newValue);
    this->instancesByType[type].push_back(wrap);
    return wrap;
}

IDOManager::Value IDOManager::instantiateIDO(const std::string &type, DataPack &init){
    return this->instantiateIDO(type, this->generateNewID(), init);
}

IDO* IDOManager::at(ID id){
    try{
        return this->instances.at(id);
    }catch(const std::out_of_range &ex){
        throw IDOSException("NO INSTANCE FOR PASSED ID");
    }
}

bool IDOManager::hasValue(ID id)const{
    try{
        this->instances.at(id);
        return true;
    }catch(const std::out_of_range &ex)
    {
        return false;
    }
}


const std::vector<IDOManager::Value>& IDOManager::getInstancesOfType(const std::string &type){
    try{
        return this->instancesByType.at(type);
    }catch(const std::out_of_range &ex){
        throw IDOSException("NO INSTANCES FOR GIVEN TYPE");
    }
}

void IDOManager::registerAlias(const std::string &alias, ID value){
    this->alias.insert(std::pair(alias, value));
}

void IDOManager::registerType(IDO* instanceTemplate){
    this->types.insert(std::pair(instanceTemplate->getType(), instanceTemplate));
}


void IDOManager::unregisterAlias(const std::string &alias){
    this->alias.erase(alias);
}

void IDOManager::unregisterType(const std::string &typeName){
    this->types.erase(typeName);
}


IDOManager::~IDOManager(){
    for(auto type : this->types)
        delete type.second;
    for(auto instance : this->instances)
        delete instance.second;
}