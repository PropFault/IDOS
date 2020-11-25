#pragma once
#include "ido.h"
#include <string>
#include <memory>
#include "IDOManager.h"
#include <vector>
class CharArrayIDO : public idos::IDO{
private:
    char* text = NULL;
    std::vector<ID> texts;
protected:
    virtual idos::DataPack _pack()const;
    virtual void _unpack(const idos::DataPack &pack);
public:
    IDO::ID self;

    CharArrayIDO();
    CharArrayIDO(const CharArrayIDO& other);
    const static std::string PARAMS_TEXT;
    const char* getText();
    void setText(const char* text);

    virtual IDO* clone();
    virtual ~CharArrayIDO();
};