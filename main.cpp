#include <iostream>
#include "ido.h"
#include "IDOManager.h"
#include "CharArrayIDO.h"
#include "ManagedValue.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "idodao.h"
int main(int, char**) {
    idos::IDOManager manager;
    idos::IDODAO dao(manager);

    manager.registerType(new CharArrayIDO);
    


    auto daFunny = dao.loadFromFile<CharArrayIDO>("./../../test.json");
    dao.saveToFile("./save.json", daFunny);
    auto id = daFunny.getIdentifier();
    std::cout << daFunny.getValue()->getText() << std::endl;
    std::cout << manager.at(id)->as<CharArrayIDO>()->getText()<< std::endl;
    std::cout << daFunny.getValue()->self << "=" << id<< std::endl;
    std::cout << "PRINTING SELF: " << manager.at(daFunny.getValue()->self)->as<CharArrayIDO>()->getText() << std::endl;

    idos::DataPack initDaFunny;
    initDaFunny[idos::IDO::PROP_TYPE] = std::string("MyCharArray");
    initDaFunny[CharArrayIDO::PARAMS_TEXT] = std::string("Hello this is my char array :)");
    idos::DataPack initDaFunny2;
    initDaFunny2[idos::IDO::PROP_TYPE] = std::string("MyCharArray");
    initDaFunny2[CharArrayIDO::PARAMS_TEXT] = std::string("Hello this is your char array :(");
    auto charArray = manager.instantiateIDO<CharArrayIDO>(initDaFunny);
    manager.registerAlias("My char array", charArray.first);
    auto managedCharArray = idos::ManagedValue<CharArrayIDO>(manager, charArray.first);
    auto charArray2 = idos::ManagedValue<CharArrayIDO>(manager, manager.instantiateIDO<CharArrayIDO>(initDaFunny2).first);
    std::cout<<((CharArrayIDO*)charArray.second)->getText()<<std::endl;
    std::cout<<managedCharArray.getValue()->getText()<<std::endl;
    std::cout<<managedCharArray.getValue()->getText()<<std::endl;
    
    std::cout<<charArray2.getValue()->getText()<<std::endl;
    charArray2.getValue()->setText("I changed this haha >:)");
    std::cout<<charArray2.getValue()->getText()<<std::endl;
    std::cout<<((CharArrayIDO*)manager.getAlias("My char array").second)->getText()<<std::endl;

    std::cout<<"Instance list"<<std::endl;
    auto instancesForType = manager.getInstancesOfType("CharArray");
    for(auto instance : instancesForType){
        std::cout<<instance.second->as<CharArrayIDO>()->getText()<<"\t";
    }
    std::cout<<std::endl;
}
