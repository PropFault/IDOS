#pragma once
#include <nlohmann/json.hpp>
namespace idos{
class ID_T
{
public:
    uint64_t value;
    ID_T(const uint64_t &wrap)
        : value(wrap) {}
    ID_T()
        : ID_T(0) {}
    operator uint64_t() { return value; }
};
inline bool operator==(const ID_T &lhs, const ID_T &rhs)
{
    return lhs.value == rhs.value;
};
inline bool operator<(const ID_T &lhs, const ID_T &rhs)
{
    return lhs.value < rhs.value;
};
inline std::ostream &operator<<(std::ostream &out, const ID_T &data)
{
    out << data.value;
    return out;
};
 inline void to_json(nlohmann::json &j, const ID_T &p)
    {
        j = nlohmann::json{{"id", p.value}};
    }

    inline void from_json(const nlohmann::json &j, ID_T &p)
    {
        p.value = j.at("id").get<uint64_t>();
    };

typedef ID_T ID;
}