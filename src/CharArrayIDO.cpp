#include "CharArrayIDO.h"
#include <cstring>
#include <iostream>
#include "Ref.h"
#include "datapack.h"
const std::string CharArrayIDO::PARAMS_TEXT = "Text";


CharArrayIDO::CharArrayIDO()
:IDO("CharArray"),text(NULL){};
CharArrayIDO::CharArrayIDO(const CharArrayIDO& other)
:IDO("CharArray"){
    if(other.text == NULL){
        this->text = NULL;
    }else{
        this->text = new char[std::strlen(other.text)]+1;
        std::strcpy(this->text, other.text);
    }
}

idos::DataPack CharArrayIDO::_pack()const{
    idos::DataPack pack;
    pack[PARAMS_TEXT] = this->text;
    pack["child"] = this->self;
    pack["messages"] = idos::DataArray();
    for(int i = 0; i < this->texts.size(); i++){
        pack["messages"].push_back(this->texts.at(i));
    }
    return pack;
}
void CharArrayIDO::_unpack(const idos::DataPack &pack ){
    std::string wrappedText = pack.at(PARAMS_TEXT).get<std::string>();
    if(this->text != NULL)
        delete[] this->text;
    this->text = new char[wrappedText.length()+1];
    std::strcpy(this->text, wrappedText.c_str());
    try{
        std::cout << pack.at("child").getType() << std::endl;
        this->self = pack.at("child").get<idos::UntypedRef>();
    }catch(const std::out_of_range &ex){

    }
    try{
        for(auto &i : pack.at("messages"))
            this->texts.push_back(i->get<idos::UntypedRef>());
    }catch(const std::out_of_range &ex){
        
    }
}

const char* CharArrayIDO::getText()
{
    return this->text;
}

void CharArrayIDO::setText(const char * text){
    if(text == NULL||text == nullptr)
        return;
    if(this->text != NULL)
        delete[] this->text;
    this->text = new char[std::strlen(text)+1];
    std::strcpy(this->text, text);
}

idos::IDO* CharArrayIDO::clone(){
    return (idos::IDO*) new CharArrayIDO(*this);
}

CharArrayIDO::~CharArrayIDO(){
    delete[] this->text;
}