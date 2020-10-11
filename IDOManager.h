#pragma once
#include <unordered_map>
#include "ido.h"
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>
#include <boost/uuid/random_generator.hpp>
namespace idos{
    class IDOManager{
    public:
        typedef boost::uuids::uuid ID;
        typedef std::pair<ID, IDO*> Value;
    private: 
        std::unordered_map<std::string, Value> alias;
        std::unordered_map<std::string, IDO*> types;
        std::unordered_map<ID, IDO*,boost::hash<ID>> instances;
        std::unordered_map<std::string, std::vector<Value>> instancesByType;
        boost::uuids::random_generator gen;
    public:
        const std::unordered_map<std::string, Value> &getAliasList()const;
        const std::unordered_map<std::string, IDO*> &getTypes()const; 

        Value getAlias(const std::string &alias)const;
        bool hasAlias(const std::string &alias)const;

        Value instantiateIDO(const std::string &type, DataPack &init);

        IDO* at(ID id);
        bool hasValue(ID id)const;

        const std::vector<Value>& getInstancesOfType(const std::string &type);

        void registerAlias(const std::string &alias, Value value);
        void registerType(const std::string &typeName, IDO* instanceTemplate);

        void unregisterAlias(const std::string &alias);
        void unregisterType(const std::string &typeName);

        ~IDOManager();
    };
};

////ISsues ye:
//What happens when IDOS referenced by other IDOS gets fucking deleted?? 
///Solution: Wrap Values in "ManagedObject" type -> access to value always goes through Manager which allows advanced error handling and caching
//No way of precreating all
