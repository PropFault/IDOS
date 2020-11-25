#define BOOST_TEST_MODULE IDO_TEST
#include <boost/test/included/unit_test.hpp>
#include "../src/ido.h"
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

};
/*private:
        std::string type;
        std::string displayName;

    protected:
        virtual void _unpack(const DataPack &pack) = 0;

    public:
        IDO(const std::string &type);
        IDO(const std::string &type, const std::string &displayName);
        typedef ID_T ID;

        const static std::string PROP_TYPE;
        const static std::string PROP_DISPLAY_NAME;
        const static std::string PROP_ALIAS;
        const static std::string PROP_REF;
        virtual IDO *clone() = 0;
        DataPack pack() const;
        template <typename t>
        t *as()
        {
            return (t *)this;
        }
        void unpack(const DataPack &pack);

        const std::string& getType()const;
        const std::string& getDisplayName()const;
        void setDisplayName(const std::string& name);

        virtual ~IDO();
    };
    
       inline  void to_json(nlohmann::json& j, const IDO::ID& p) {
            j = nlohmann::json{ {"id", p.value} };
        }

       inline  void from_json(const nlohmann::json& j, IDO::ID& p) {
            p.value = j.at("id").get<uint64_t>();
        }*/


BOOST_AUTO_TEST_CASE(pack)
{
  IDOFixture fixture;
  auto packed = fixture.pack();
  BOOST_REQUIRE_NO_THROW(packed.at(IDO::PROP_TYPE));
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

  BOOST_REQUIRE_THROW(fixture.unpack_param_copy.at(IDO::PROP_DISPLAY_NAME), nlohmann::json::out_of_range); // freaking nlohmann. i shouldn't have this library be so interconnected with my own stuff
  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(fixture.pack_ret_key1));
  BOOST_REQUIRE_NO_THROW(fixture.unpack_param_copy.at(fixture.pack_ret_key2));
  BOOST_TEST(fixture.getDisplayName() == IDO_FIX_DISPLAYNAME);
  BOOST_TEST(fixture.unpack_param_copy.at(fixture.pack_ret_key1) == fixture.pack_ret_val1);
  BOOST_TEST(fixture.unpack_param_copy.at(fixture.pack_ret_key2) == fixture.pack_ret_val2);
}