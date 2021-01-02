#include <boost/test/unit_test.hpp>
#include "../src/datapack.h"
#include "../src/datapackJSONConverter.h"
using namespace idos;
BOOST_AUTO_TEST_SUITE(DataPack_UT)
BOOST_AUTO_TEST_CASE(flat_structure){
    DataPack pack;
    std::string key1 = "Test Key";
    std::string value1 = "Funny";
    pack[key1] = value1;
    BOOST_CHECK_EQUAL(pack.at(key1), value1);

    std::string key2 = "Test 2Key";
    unsigned long long value2 = 123521;
    pack[key2] = value2;
    BOOST_CHECK_EQUAL(pack.at(key2), value2);

    std::string key3 = "3 Test Key";
    int16_t value3 = 2341;
    pack[key3] = value3;
    BOOST_CHECK_EQUAL(pack.at(key3), value3);

    std::string key4 = "4th key";
    uint8_t value4 = 32;
    pack[key4] = DataArray();
    pack[key4].push_back(value4);
    BOOST_CHECK_EQUAL(pack[key4][0],value4);
}

BOOST_AUTO_TEST_CASE(nested_structure){
    DataPack outerPack;
    DataPack innerPack;
    
    std::string innerPackKey = "inner";
    std::string valueKey = "someValue";
    char someValue = 'h';
    innerPack[valueKey] = someValue;
    outerPack[innerPackKey] = innerPack;

    BOOST_CHECK_EQUAL(innerPack[valueKey], someValue);
    BOOST_CHECK_EQUAL(outerPack[innerPackKey][valueKey], someValue);
    BOOST_CHECK_EQUAL(outerPack[innerPackKey], innerPack);
    BOOST_TEST_MESSAGE(nlohmann::json(outerPack));
}

BOOST_AUTO_TEST_CASE(complex_structure){

}
BOOST_AUTO_TEST_SUITE_END()