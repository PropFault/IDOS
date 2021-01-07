#include <boost/test/unit_test.hpp>
#include "../src/IDOManager.h"
#include "../src/idodao.h"
#include <string>
#include <exception>
#include "../src/CharArrayIDO.h"
#include "../src/ido.h"
#include "../src/Ref.h"
#include "../src/datapackJSONConverter.h"

using namespace idos;
BOOST_AUTO_TEST_SUITE(IDODAO_UT)

class PointIDO : public IDO{
private:
    float x;
    float y;
    float z;
    Ref<CharArrayIDO> text;
public:
    PointIDO()
    :IDO("Point"){
    }

    PointIDO(float x, float y, float z, const Ref<CharArrayIDO> &text)
    :IDO("Point"), x(x), y(y), z(z), text(text){}

    virtual DataPack _pack() const{
        DataPack p;
        p["x"] = x;
        p["y"] = y;
        p["z"] = z;
        p["text"] = text;
        return p;
    }
    virtual void _unpack(const DataPack &pack){
        std::cout<<pack.at("x").getType()<<std::endl;
        this->x = pack.at("x").get<float>();
        this->y = pack.at("y").get<float>();
        this->z = pack.at("z").get<float>();
        this->text = pack.at("text").get<UntypedRef>();
    }
    virtual IDO *clone(){
        return new PointIDO();
    }

    bool operator==(const PointIDO &othr) {
        return this->x == othr.x && this->y == othr.y && this->z == othr.z && std::string(this->text.getValue()->getText()) == othr.text.getValue()->getText();
    }
};

class SquareIDO : public IDO{
private:
    Ref<PointIDO> origin;
    float size;
public:
    SquareIDO():IDO("Square"){}
    SquareIDO(const Ref<PointIDO>& origin, float size):IDO("Square"),origin(origin), size(size){}
        virtual DataPack _pack() const{
        DataPack p;
        p["origin"] = origin;
        p["size"] = size;
        return p;
    }
    virtual void _unpack(const DataPack &pack){
        this->origin = pack.at("origin").get<UntypedRef>();
        this->size = pack.at("size").get<float>();
    }
    virtual IDO *clone(){
        return new SquareIDO();
    }
    bool operator==(const SquareIDO &othr)const{
        return this->size == othr.size && (*this->origin.getValue()) == (*othr.origin.getValue()); 
    }
};

class IDODAOFixture{
public:
    IDOManager manager;
    IDODAO dao;
    std::string jsonData = "{"
                            "\"type\": \"CharArray\","
                            "\"displayName\": \"Hehe\","
                            "\"alias\": \"myCharArray\","
                            "\"Text\": \"muhAss\","
                            "\"self\": {\"ref\": \"myCharArray\"},"
                            "\"child\":{\n"
                                "\"type\": \"CharArray\",\n"
                                "\"Text\": \"I am a child\"\n"
                            "},\n"
                            "\"messages\":[\n"
                                "{\"type\": \"CharArray\",\"Text\": \"Element 1\"},\n"
                                "{\"type\": \"CharArray\", \"Text\": \"Element 2\", \"displayName\": \"Element 2\"},\n"
                                "{\"type\": \"CharArray\", \"Text\": \"Element 3rd\", \"displayName\": \"Element three\"},\n"
                                "{\"type\": \"CharArray\", \"Text\": \"Element 4th\", \"displayName\": \"Element 4\"}\n"
                            "]\n"
                        "}";
    std::string jsonFilePath = "./ut_testfile.json";
    Ref<SquareIDO> square;
    Ref<PointIDO> square_origin;
    IDODAOFixture()
    :manager(), dao(manager){
        std::ofstream os(jsonFilePath,std::ofstream::out);
        os << jsonData;
        os.close();
        manager.registerType(new CharArrayIDO());
        manager.registerType(new PointIDO());
        manager.registerType(new SquareIDO());
        DataPack dat_square_o;
        CharArrayIDO prototype;
        prototype.setText("my origin");

        dat_square_o["x"] =  10.0f;
        dat_square_o["y"] = 5.32f;
        dat_square_o["z"] = -2.3f;
        dat_square_o["text"] = Ref<PointIDO>(manager, manager.instantiateIDO<CharArrayIDO>(prototype.pack()));
        square_origin = Ref<PointIDO>(manager, manager.instantiateIDO<PointIDO>(dat_square_o));
        manager.registerAlias("globalOrigin", square_origin.getIdentifier());

        DataPack dat_square;
        DataPack dat_square_origin_ref;
        dat_square["origin"] = square_origin;
        dat_square["size"] = 21.0f;
        square = Ref<SquareIDO>(manager, manager.instantiateIDO<SquareIDO>(dat_square));
    }

    ~IDODAOFixture(){
        if(std::remove(jsonFilePath.c_str()) != 0){
            std::cout<< "Can't delete UnitTest file"<<std::endl;
        }else{

        }
    }
};

BOOST_AUTO_TEST_CASE(saveAndLoadIDO){
    IDODAOFixture f;
    f.dao.saveToFile("./square.json",f.square);
    BOOST_CHECK_EQUAL((*Ref<SquareIDO>(f.dao.loadFromFile("./square.json")).getValue()), (*f.square.getValue()));
    std::remove("./square.json");
}

BOOST_AUTO_TEST_CASE(loadIDO){
    IDODAOFixture f;
    Ref<CharArrayIDO> a = f.dao.loadFromFile(f.jsonFilePath.c_str());
    DataPack data = nlohmann::json::parse(f.jsonData).get<DataPack>();
    BOOST_CHECK_EQUAL(a.getValue()->getText(), data.at("Text").get<std::string>());
    BOOST_CHECK_EQUAL(a.getValue()->getDisplayName(), data.at("displayName").get<std::string>());
    BOOST_CHECK_EQUAL(a.getValue()->getTexts().at(0).getValue()->getText(), data.at("messages").get<DataArray>().at(0).get<DataPack>().at("Text").get<std::string>());
}

BOOST_AUTO_TEST_SUITE_END()