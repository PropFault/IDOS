#pragma once
#include "ido.h"
#include <string>
class CharArrayIDO : public idos::IDO{
private:
    char* text = NULL;
protected:
    virtual idos::DataPack _pack()const;
    virtual void _unpack(const idos::DataPack &pack);
public:
    CharArrayIDO();
    CharArrayIDO(const CharArrayIDO& other);
    const static std::string PARAMS_TEXT;
    const char* getText();
    void setText(const char* text);
    virtual IDO* clone();
    virtual ~CharArrayIDO();
};