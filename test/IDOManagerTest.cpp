#include <boost/test/unit_test.hpp> 
#include "../src/ido.h"
#include "../src/IDOManager.h"
#include <unordered_set>
#include <thread>
#include <mutex>
#include "../src/IDOSException.h"
BOOST_AUTO_TEST_SUITE(IDOManager_UT)
using namespace idos;
/*

namespace idos{
    class IDOManager{
        const std::unordered_map<std::string, IDO::ID> &getAliasList()const;
        const std::unordered_map<std::string, IDO*> &getTypes()const; 

        /////Value getAlias(const std::string &alias);
        //IDO::ID getIDForAlias(const std::string& alias)const;
        //std::string getAliasForID(const IDO::ID& id)const;
        //bool hasAlias(const std::string &alias)const;

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
*/
#define StringIDO_PACK_TEXT "text"
class StringIDO : public IDO, public std::string{
public:
    StringIDO()
    :IDO("StringIDO"){}
    StringIDO(const std::string &str)
    :IDO("StringIDO"),std::string(str){}

    virtual IDO *clone(){
        return new StringIDO(*this);
    };
    virtual DataPack _pack() const{
        DataPack pack;
        pack[StringIDO_PACK_TEXT] = *this;
        return pack;
    }
    virtual void _unpack(const DataPack &pack){
        this->assign(pack.at(StringIDO_PACK_TEXT));
    }
};

#define IntIDO_PACK_DATA "Data"

class IntIDO : public IDO{
private:
    int64_t data;
public:
    IntIDO()
    :IDO("IntIDO"){}

    virtual IDO* clone(){
        return new IntIDO(*this);
    }
    virtual DataPack _pack() const{
        DataPack pack;
        pack[IntIDO_PACK_DATA] = data;
        return pack;
    }
    virtual void _unpack(const DataPack &pack){
        this->data = pack.at(IntIDO_PACK_DATA);
    }

    int64_t getData() const{
        return data;
    }

    void setData(int64_t newDat){
        this->data = newDat;
    }
    bool operator==(const IntIDO &other) const{
        return this->getData() == other.getData();
    }
};

struct IDOManagerFixture{
    IDOManager manager;
};

#define INIT_ALIAS(f) \
        auto testID = f.manager.generateNewID();\
        std::string alias("FirstTestAlias");\
        auto testID2 = f.manager.generateNewID();std::string alias2("SecondTestAlias");\
        f.manager.registerAlias(alias, testID);\
        f.manager.registerAlias(alias2, testID2);

BOOST_AUTO_TEST_CASE(getAliasList){
    IDOManagerFixture f;
    INIT_ALIAS(f)


    BOOST_REQUIRE_NO_THROW(f.manager.getAliasList().at(alias));
    BOOST_REQUIRE_NO_THROW(f.manager.getAliasList().at(alias2));
    BOOST_CHECK(f.manager.getAliasList().at(alias) == testID);
    BOOST_CHECK(f.manager.getAliasList().at(alias2) == testID2);
}

BOOST_AUTO_TEST_CASE(getIDForAlias){
    IDOManagerFixture f;
    INIT_ALIAS(f)

    BOOST_CHECK(f.manager.getIDForAlias(alias) == testID);
    BOOST_CHECK(f.manager.getIDForAlias(alias2) == testID2);
}

BOOST_AUTO_TEST_CASE(getAliasForID){
    IDOManagerFixture f;
    INIT_ALIAS(f)

    BOOST_CHECK(f.manager.getAliasForID(testID) == alias);
    BOOST_CHECK(f.manager.getAliasForID(testID2) == alias2);
}

BOOST_AUTO_TEST_CASE(hasAlias){
    IDOManagerFixture f;
    INIT_ALIAS(f);

    BOOST_CHECK(f.manager.hasAlias(alias));
    BOOST_CHECK(f.manager.hasAlias(alias2));
}

void testId(IDOManagerFixture &f, std::unordered_set<IDO::ID>& ids, std::mutex &mutex, std::mutex &genmutex){
    while(true){
        genmutex.lock();
        auto newID = f.manager.generateNewID();
        genmutex.unlock();
        if(ids.find(newID)!=ids.end()){
            BOOST_TEST_FAIL("DUPLICATE ID GENERATED AFTER " << ids.size() << " GENERATIONS!!");
            return;
        }
        mutex.lock();
        if(ids.size() > 10000000){
            mutex.unlock();
            break;
        }
        ids.insert(newID);
        mutex.unlock();

    }
}

BOOST_AUTO_TEST_CASE(generateNewID){
    IDOManagerFixture f;
    std::unordered_set<IDO::ID> ids;
    std::mutex idmutex;
    std::mutex genmutex;
    std::thread c1(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    std::thread c2(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    std::thread c3(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    std::thread c4(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));    
    std::thread c5(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    std::thread c6(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    std::thread c7(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    std::thread c8(testId,std::ref(f),std::ref(ids),std::ref(idmutex), std::ref(genmutex));
    c1.join();
    c2.join();
    c3.join();
    c4.join();    
    c5.join();
    c6.join();
    c7.join();
    c8.join();
    BOOST_CHECK(true);
}

void initManagerTypes(IDOManager &mgr){
    mgr.registerType(new StringIDO);
    mgr.registerType(new IntIDO);
}

BOOST_AUTO_TEST_CASE(instantiateIDO_string_id_datapack){
    IDOManagerFixture f;
    DataPack StringIDOPack;
    StringIDOPack[StringIDO_PACK_TEXT] = "Hello :)";
    StringIDOPack[IDO::PROP_DISPLAY_NAME] = "My fancy new string :)";
    DataPack IntIDOPack;
    IntIDOPack[IntIDO_PACK_DATA] = 123456;
    initManagerTypes(f.manager);
    auto stringIDOID = f.manager.generateNewID();
    auto intIDOID = f.manager.generateNewID();
    IDOManager::Value stringIDO;
    IDOManager::Value intIDO;
    BOOST_REQUIRE_NO_THROW(stringIDO = f.manager.instantiateIDO("StringIDO", stringIDOID, StringIDOPack));
    BOOST_REQUIRE_NO_THROW(intIDO = f.manager.instantiateIDO("IntIDO", intIDOID, IntIDOPack));
    BOOST_REQUIRE_THROW(f.manager.instantiateIDO("notARealType", intIDOID, StringIDOPack), IDOSException);
    BOOST_CHECK(stringIDO.second->getType() == "StringIDO");
    BOOST_CHECK(intIDO.second->getType() == "IntIDO");
    BOOST_CHECK_EQUAL(stringIDO.second->getDisplayName(), "My fancy new string :)");
    BOOST_CHECK_EQUAL((*stringIDO.second->as<StringIDO>()), "Hello :)");
    BOOST_CHECK_EQUAL(intIDO.second->as<IntIDO>()->getData(), 123456);
    intIDO.second->as<IntIDO>()->setData(654321);
    BOOST_CHECK_EQUAL(intIDO.second->as<IntIDO>()->getData(), 654321);

    IntIDO* directIntIDO = f.manager.at<IntIDO>(intIDOID);
    StringIDO* directStringIDO = f.manager.at(stringIDOID)->as<StringIDO>();

    BOOST_CHECK((*directIntIDO) == (*intIDO.second->as<IntIDO>()));
    BOOST_CHECK((*directStringIDO) == (*stringIDO.second->as<StringIDO>()));
}

BOOST_AUTO_TEST_CASE(instantiateIDO_string_pack){
    IDOManagerFixture f;
    initManagerTypes(f.manager);
    DataPack intPack;
    intPack[IntIDO_PACK_DATA] = 987654;
    intPack[IDO::PROP_DISPLAY_NAME] = "My Numba";
    
    DataPack stringPack;
    stringPack[StringIDO_PACK_TEXT] = "Big text";
    
    auto stringVal = f.manager.instantiateIDO("StringIDO", stringPack);
    auto intVal = f.manager.instantiateIDO("IntIDO", intPack);
    
    BOOST_CHECK_EQUAL(intVal.second->getDisplayName(), "My Numba");
    BOOST_CHECK_EQUAL(intVal.second->getDisplayName(), f.manager.at<IntIDO>(intVal.first)->getDisplayName());
    BOOST_CHECK_EQUAL(intVal.second, f.manager.at<IntIDO>(intVal.first));
    
}

BOOST_AUTO_TEST_SUITE_END()