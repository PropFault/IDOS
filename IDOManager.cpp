#include "IDOManager.h"
#include "IDOSException.h"
using namespace idos;

const std::unordered_map<std::string, IDOManager::Value> &IDOManager::getAliasList()const{
    return this->alias;
}
const std::unordered_map<std::string, IDO*> &IDOManager::getTypes()const{
    return this->types;
}

IDOManager::Value IDOManager::getAlias(const std::string &alias)const{
    return this->alias.at(alias);
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


IDOManager::Value IDOManager::instantiateIDO(const std::string &type, DataPack &init){
    IDO* newValue = nullptr;
    try{
        newValue = this->types.at(type)->clone();
    }catch(const std::out_of_range &ex){
        throw IDOSException("TRIED INSTANTIATING IDO OF UNKNOWN TYPE!");
    }
    newValue->unpack(init);
    ID newID = this->gen();
    this->instances.insert(std::pair(newID, newValue));
    IDOManager::Value wrap(newID, newValue);
    this->instancesByType[type].push_back(wrap);
    return wrap;
}

IDO* IDOManager::at(IDOManager::ID id){
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

void IDOManager::registerAlias(const std::string &alias, IDOManager::Value value){
    this->alias.insert(std::pair(alias, value));
}

void IDOManager::registerType(const std::string &typeName, IDO* instanceTemplate){
    this->types.insert(std::pair(typeName, instanceTemplate));
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