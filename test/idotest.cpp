#include <boost/test/unit_test.hpp>
#include "../src/ido.h"
#include "../src/datapack.h"
using namespace idos;
#define IDO_FIX_IDOTYPE "IDOFixture"
#define IDO_FIX_DISPLAYNAME "IDO Test-Fixture :)"

class IDOFixture : public IDO{
public:
  bool copyConstructorCalled;
  std::string pack_ret_key1;
  std::string pack_ret_val1;
  std::string pack_ret_key2;
  uint32_t pack_ret_val2;

  DataPack unpack_param_copy;
  IDOFixture()
  :IDO(IDO_FIX_IDOTYPE, IDO_FIX_DISPLAYNAME), 
  pack_ret_key1("complex obj"), pack_ret_key2("unsigned int32_t"), pack_ret_val1("Very nice value :)"), pack_ret_val2(298429349), copyConstructorCalled(false){
  }
  IDOFixture(const IDOFixture& other)
  :IDOFixture()
  {
    copyConstructorCalled = true;
  }
  virtual DataPack _pack() const override{
    DataPack pack;
    pack[pack_ret_key1] = pack_ret_val1;
    pack[pack_ret_key2] = pack_ret_val2;
    return pack;
  }
  
  virtual void _unpack(const DataPack &pack){
    unpack_param_copy = pack;
  }
  
  virtual IDO *clone(){
    return new IDOFixture(*this);
  }
  bool operator==(const IDOFixture &oth)const{
    return this->pack_ret_val1 == oth.pack_ret_val1 && this->pack_ret_val2 == oth.pack_ret_val2 && this->getDisplayName() == oth.getDisplayName();
  }
  bool operator!=(const IDOFixture &oth)const{
    return !((*this) == oth);
  }
};

std::ostream& operator<<(std::ostream& out, const IDOFixture &oth){
  out << "IDOFixture{displayname="<<oth.getDisplayName()<<";val1="<<oth.pack_ret_val1<<";val2="<<oth.pack_ret_val2<<"}";
  return out;
}
BOOST_AUTO_TEST_SUITE(IDO_UT)
BOOST_AUTO_TEST_CASE(pack)
{
  IDOFixture fixture;
  auto packed = fixture.pack();
  BOOST_REQUIRE_NO_THROW(packed.at(idos::IDO::PROP_TYPE));
  BOOST_REQUIRE_NO_THROW(packed.at(IDO::PROP_DISPLAY_NAME));
  BOOST_REQUIRE_NO_THROW(packed.at(fixture.pack_ret_key1));
  BOOST_REQUIRE_NO_THROW(packed.at(fixture.pack_ret_key2));
  BOOST_TEST(packed.at(IDO::PROP_TYPE) == IDO_FIX_IDOTYPE);
  BOOST_TEST(packed.at(IDO::PROP_DISPLAY_NAME) == IDO_FIX_DISPLAYNAME);
  BOOST_TEST(packed.at(fixture.pack_ret_key1) == fixture.pack_ret_val1);
  BOOST_TEST(packed.at(fixture.pack_ret_key2) == fixture.pack_ret_val2);
}

BOOST_AUTO_TEST_CASE(unpack_full)
{
  IDOFixture fixture;
  DataPack pack;
  std::string newDisplayName = "new display name";
  pack[IDO::PROP_DISPLAY_NAME] = newDisplayName;
  pack[fixture.pack_ret_key1] = fixture.pack_ret_val1;
  pack[fixture.pack_ret_key2] = fixture.pack_ret_val2;
  fixture.unpack(pack);

  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(IDO::PROP_DISPLAY_NAME));
  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(fixture.pack_ret_key1));
  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(fixture.pack_ret_key2));
  BOOST_TEST(fixture.getDisplayName() == newDisplayName);
  BOOST_TEST(fixture.unpack_param_copy.at(IDO::PROP_DISPLAY_NAME) == fixture.getDisplayName());
  BOOST_TEST(fixture.unpack_param_copy.at(IDO::PROP_DISPLAY_NAME) == newDisplayName);
  BOOST_TEST(fixture.unpack_param_copy.at(fixture.pack_ret_key1) == fixture.pack_ret_val1);
  BOOST_TEST(fixture.unpack_param_copy.at(fixture.pack_ret_key2) == fixture.pack_ret_val2);
}

BOOST_AUTO_TEST_CASE(unpack_no_displayname){
  IDOFixture fixture;
  DataPack pack;
  pack[fixture.pack_ret_key1] = fixture.pack_ret_val1;
  pack[fixture.pack_ret_key2] = fixture.pack_ret_val2;
  fixture.unpack(pack);

  BOOST_REQUIRE_THROW(fixture.unpack_param_copy.at(IDO::PROP_DISPLAY_NAME), std::out_of_range);
  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(fixture.pack_ret_key1));
  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(fixture.pack_ret_key2));
  BOOST_TEST(fixture.getDisplayName() == IDO_FIX_DISPLAYNAME);
  BOOST_TEST(fixture.unpack_param_copy.at(fixture.pack_ret_key1) == fixture.pack_ret_val1);
  BOOST_TEST(fixture.unpack_param_copy.at(fixture.pack_ret_key2) == fixture.pack_ret_val2);
}


BOOST_AUTO_TEST_CASE(clone){
  IDOFixture fixture;
  IDOFixture* prt;
  BOOST_REQUIRE_NO_THROW(prt = dynamic_cast<IDOFixture*>(fixture.clone()));
  BOOST_TEST((*prt) == fixture);
  prt->pack_ret_val2 = fixture.pack_ret_val2+1;
  BOOST_TEST((*prt)!=fixture);
  BOOST_TEST(prt->copyConstructorCalled);
}

BOOST_AUTO_TEST_CASE(at){
  IDOFixture fixture;
  IDO* prt = &fixture;
  IDOFixture* backCast;
  BOOST_REQUIRE_NO_THROW(backCast = prt->as<IDOFixture>());
  BOOST_TEST(backCast != nullptr);
  BOOST_TEST((*backCast) == fixture);
  BOOST_REQUIRE_NO_THROW(prt->as<std::vector<std::string>>());
}

BOOST_AUTO_TEST_CASE(getDisplayName){
  IDOFixture fixture;
  BOOST_TEST(fixture.getDisplayName() == IDO_FIX_DISPLAYNAME);
}

BOOST_AUTO_TEST_CASE(setDisplayName){
  IDOFixture fixture;
  std::string newDisplayName("displayNameNew");
  fixture.setDisplayName(newDisplayName);
  BOOST_TEST(fixture.getDisplayName() == newDisplayName);
}

BOOST_AUTO_TEST_CASE(getType){
  IDOFixture fixture;
  BOOST_TEST(fixture.getType() == IDO_FIX_IDOTYPE);
}
BOOST_AUTO_TEST_SUITE_END()