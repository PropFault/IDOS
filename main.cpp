#include <iostream>
#include "ido.h"
#include "IDOManager.h"
#include "CharArrayIDO.h"
#include "ManagedValue.h"
int main(int, char**) {
    idos::IDOManager manager;
    manager.registerType("charArray", new CharArrayIDO);
    idos::DataPack initDaFunny;
    initDaFunny[idos::IDO::PROP_TYPE] = std::string("MyCharArray");
    initDaFunny[CharArrayIDO::PARAMS_TEXT] = std::string("Hello this is my char array :)");
    idos::DataPack initDaFunny2;
    initDaFunny2[idos::IDO::PROP_TYPE] = std::string("MyCharArray");
    initDaFunny2[CharArrayIDO::PARAMS_TEXT] = std::string("Hello this is your char array :(");
    auto charArray = manager.instantiateIDO("charArray", initDaFunny);
    
    manager.registerAlias("My char array", charArray);
    auto managedCharArray = idos::ManagedValue<CharArrayIDO>(manager, charArray.first);
    auto charArray2 = idos::ManagedValue<CharArrayIDO>(manager, manager.instantiateIDO("charArray", initDaFunny2).first);
    std::cout<<((CharArrayIDO*)charArray.second)->getText()<<std::endl;
    std::cout<<managedCharArray.getValue()->getText()<<std::endl;
    std::cout<<charArray2.getValue()->getText()<<std::endl;
    charArray2.getValue()->setText("I changed this haha >:)");
    std::cout<<charArray2.getValue()->getText()<<std::endl;
    std::cout<<((CharArrayIDO*)manager.getAlias("My char array").second)->getText()<<std::endl;

    std::cout<<"Instance list"<<std::endl;
    auto instancesForType = manager.getInstancesOfType("charArray");
    for(auto instance : instancesForType){
        std::cout<<instance.second->as<CharArrayIDO>()->getText()<<"\t";
    }
    std::cout<<std::endl;
}
