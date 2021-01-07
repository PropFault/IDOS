#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include "Ref.h"
namespace idos{
    inline void to_json(nlohmann::json& j, const DataPack& p);

    inline void to_json(nlohmann::json &j, const DataArray &array);
    
    inline void to_json(nlohmann::json& j, const idos::Data& p){
        switch (p.getType())
        {
        case Data::Type::STRING:
            j = p.get<std::string>();
            break;
        case Data::Type::INT:
            j = p.get<int>();
            break;
        case Data::Type::UINT:
            j = p.get<unsigned int>();
            break;
        case Data::Type::LONG:
            j = p.get<long>();
            break;
        case Data::Type::ULONG:
            j = p.get<unsigned long>();
            break;
        case Data::Type::LONGLONG:
            j = p.get<long long>();
            break;
        case Data::Type::ULONGLONG:
            j = p.get<unsigned long long>();
            break;
        case Data::Type::DOUBLE:
            j = p.get<double>();
            break;
        case Data::Type::FLOAT:
            j = p.get<float>();
            break;
        case Data::Type::BOOL:
            j = p.get<bool>();
            break;
        case Data::Type::OBJECT:
            j = p.get<DataPack>();
            break;
        case Data::Type::ARRAY:
            j = p.get<DataArray>();
            break;
        case Data::Type::REF:
            j = p.get<UntypedRef>();
            break;
        default:
            j = nullptr;
            break;
        }
        //std::cout<<j<<std::endl;
    }

     inline void to_json(nlohmann::json& j, const std::shared_ptr<idos::Data>& p){
         j = *p;
     }
    inline void to_json(nlohmann::json& j, const DataPack& p) {
        //j = p.getData();
        for(auto& row : p.getData()){
            std::cout<<row.first << " -> " << row.second.get()->getType() << std::endl;
            j[row.first] = *row.second;
        }
    }

    inline void to_json(nlohmann::json &j, const DataArray &p){
        j = p.getData();
    }

    inline void from_json(const nlohmann::json &j, DataPack &p);
    inline void from_json(const nlohmann::json &j, DataArray &p);
    inline void from_json(const nlohmann::json &j, Data &p){
        switch(j.type()){
            case nlohmann::json::value_t::null:
                p = std::monostate();
                break;
            case nlohmann::json::value_t::boolean:
                p = j.get<bool>();
                break;
            case nlohmann::json::value_t::string:
                p = j.get<std::string>();
                break;
            case nlohmann::json::value_t::number_integer:
                p = j.get<long long>();
                break;
            case nlohmann::json::value_t::number_unsigned:
                p = j.get<unsigned long long>();
                break;
            case nlohmann::json::value_t::number_float:
                p = j.get<float>();
                break;
            case nlohmann::json::value_t::object:
                p = j.get<DataPack>();
                break;
            case nlohmann::json::value_t::array:
                p = j.get<DataArray>();
                break;
        }
    }
    inline void from_json(const nlohmann::json &j, std::shared_ptr<Data> &p){
        p = std::make_shared<Data>(j.get<Data>());
    }
    inline void from_json(const nlohmann::json &j, DataPack &p){
        p.setData(j.get<std::unordered_map<std::string, std::shared_ptr<Data>>>());
    }
    inline void from_json(const nlohmann::json &j, DataArray &p){
        p.setData(j.get<std::vector<std::shared_ptr<Data>>>());
    }
}