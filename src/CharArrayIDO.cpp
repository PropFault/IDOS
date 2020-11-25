#include "CharArrayIDO.h"
#include <cstring>
#include <iostream>
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
    pack[PARAMS_TEXT] = std::string(this->text);
    pack["child"] = this->self;
    pack["messages"] = nlohmann::json::array();
    for(int i = 0; i < this->texts.size(); i++){
        pack["messages"][i] = this->texts.at(i);
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
        this->self = pack.at("child").get<idos::IDO::ID>();
    }catch(const nlohmann::json::out_of_range &ex){

    }
    try{
        for(auto &i : pack.at("messages"))
            this->texts.push_back(i);
    }catch(const nlohmann::json::out_of_range &ex){
        
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