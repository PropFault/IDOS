#include "ido.h"
#include "IDOManager.h"
#include "datapack.h"
using namespace idos;
#include <iostream>
const std::string IDO::PROP_TYPE = "type";
const std::string IDO::PROP_DISPLAY_NAME = "displayName";
const std::string IDO::PROP_ALIAS = "alias";
const std::string IDO::PROP_REF = "ref";

IDO::IDO(const std::string &type)
:type(type){}

IDO::IDO(const std::string &type, const std::string &displayName)
:type(type), displayName(displayName)
{}


DataPack IDO::pack()const{
    DataPack basePack = this->_pack();
    basePack[PROP_TYPE] = this->type;
    basePack[PROP_DISPLAY_NAME] = this->displayName;
    return basePack;
}
void IDO::unpack(const DataPack &pack){
    //this->type = pack.at(PROP_TYPE).get<std::string>();
    try{
        this->displayName = pack.at(PROP_DISPLAY_NAME).get<std::string>();
    }catch(const std::out_of_range &ex){
        //this->displayName = "not set";
    }
    this->_unpack(pack);
}


const std::string& IDO::getType()const{
    return this->type;
}

const std::string& IDO::getDisplayName()const{
    return this->displayName;
}
void IDO::setDisplayName(const std::string& name){
    this->displayName = name;
}

IDO::~IDO(){}