#include "ido.h"
using namespace idos;

const std::string IDO::PROP_TYPE = "type";
const std::string IDO::PROP_DISPLAY_NAME = "displayName";
DataPack IDO::pack()const{
    DataPack basePack = this->_pack();
    basePack[PROP_TYPE] = this->type;
    basePack[PROP_DISPLAY_NAME] = this->displayName;
    return basePack;
}
void IDO::unpack(const DataPack &pack){
    this->type = std::any_cast<std::string>(pack.at(PROP_TYPE));
    try{
        this->displayName = std::any_cast<std::string>(pack.at(PROP_DISPLAY_NAME));
    }catch(const std::out_of_range &ex){
        //this->displayName = "not set";
    }
    this->_unpack(pack);
}


IDO::~IDO(){}