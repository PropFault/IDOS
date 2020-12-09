#pragma once
#include <unordered_map>
#include "ido.h"
#include <random>
#include <memory>
namespace idos{
    class IDOManager{
    public:
        typedef std::pair<IDO::ID, IDO*> Value;
    private: 
        std::unordered_map<std::string, IDO::ID> alias;
        std::unordered_map<std::string, IDO*> types;
        std::unordered_map<IDO::ID, IDO*> instances;
        std::unordered_map<std::string, std::vector<Value>> instancesByType;
        std::uniform_int_distribution<unsigned long long> dis; 
        std::random_device rand;
     public:
        IDOManager()
        :dis(std::numeric_limits<std::uint64_t>::min(),
        std::numeric_limits<std::uint64_t>::max()){}

        const std::unordered_map<std::string, IDO::ID> &getAliasList()const;
        const std::unordered_map<std::string, IDO*> &getTypes()const; 

        Value getAlias(const std::string &alias);
        IDO::ID getIDForAlias(const std::string& alias)const;
        std::string getAliasForID(const IDO::ID& id)const;
        bool hasAlias(const std::string &alias)const;

        IDO::ID generateNewID();

        Value instantiateIDO(const std::string &type, IDO::ID id, DataPack &init);
        Value instantiateIDO(const std::string &type, DataPack &init);
        template <typename T>
        Value instantiateIDO(DataPack &init){
            std::unique_ptr<T> t = std::make_unique<T>();
            return this->instantiateIDO(t->getType(), init);
        }
        template <typename T>
        Value instantiateIDO(IDO::ID id, DataPack &init){
            std::unique_ptr<T> t = std::make_unique<T>();
            return this->instantiateIDO(t->getType(), init);
        }

        IDO* at(IDO::ID id);
        template<typename T>
        T* at(IDO::ID id){
            return at(id)->as<T>();
        }
        bool hasValue(IDO::ID id)const;

        const std::vector<Value>& getInstancesOfType(const std::string &type);

        void registerAlias(const std::string &alias, IDO::ID id);
        void registerType(IDO* instanceTemplate);

        void unregisterAlias(const std::string &alias);
        void unregisterType(const std::string &typeName);
        template<typename T>
        void unregisterType(){
            std::unique_ptr<T> t = std::make_unique<T>();
            this->unregisterType(t->getType());
        }

        ~IDOManager();
    };
};

////ISsues ye:
//What happens when IDOS referenced by other IDOS gets freaking deleted?? 
///Solution: Wrap Values in "ManagedObject" type -> access to value always goes through Manager which allows advanced error handling and caching
//No way of precreating all
