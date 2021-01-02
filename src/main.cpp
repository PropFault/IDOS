#include <iostream>
#include "ido.h"
#include "IDOManager.h"
#include "CharArrayIDO.h"
#include "Ref.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "idodao.h"
#include "Ref.h"
int main(int, char**) {
    idos::IDOManager manager;
    idos::IDODAO dao(manager);

    manager.registerType(new CharArrayIDO);
    


    auto daFunny = idos::Ref<CharArrayIDO>(dao.loadFromFile("../../../test.json"));
    for(auto &r:manager.getInstances()){
        std::cout<<r.first<<"|"<<r.second->getType()<<std::endl;
    }
    dao.saveToFile("./save.json", daFunny);
    auto id = daFunny.getIdentifier();
    std::cout << daFunny.getValue()->getText() << std::endl;
    std::cout << manager.at(id)->as<CharArrayIDO>()->getText()<< std::endl;
    std::cout << daFunny.getValue()->self << "=" << id<< std::endl;
    std::cout << "PRINTING SELF: " <<daFunny.getValue()->self.getValue()->getText() << std::endl;

    idos::DataPack initDaFunny;
    initDaFunny[idos::IDO::PROP_TYPE] = std::string("MyCharArray");
    initDaFunny[CharArrayIDO::PARAMS_TEXT] = std::string("Hello this is my char array :)");
    idos::DataPack initDaFunny2;
    initDaFunny2[idos::IDO::PROP_TYPE] = std::string("MyCharArray");
    initDaFunny2[CharArrayIDO::PARAMS_TEXT] = std::string("Hello this is your char array :(");
    auto charArray = manager.instantiateIDO<CharArrayIDO>(initDaFunny);
    manager.registerAlias("My char array", charArray.first);
    std::cout<<((CharArrayIDO*)manager.getAlias("My char array").second)->getText()<<std::endl;

    std::cout<<"Instance list"<<std::endl;
    auto instancesForType = manager.getInstancesOfType("CharArray");
    for(auto instance : instancesForType){
        std::cout<<instance.second->as<CharArrayIDO>()->getText()<<"\t";
    }
    std::cout<<std::endl;
    
    auto loaded = idos::Ref<CharArrayIDO>(dao.loadFromFile("./save.json"));
    std::cout<<loaded.getValue()->pack()<<std::endl;
}
