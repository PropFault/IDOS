#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <unordered_map>
#include <variant>
#include <string>
#include "IDOSException.h"
#include "Ref.h"
#include <ostream>
#include <iostream>
#include <iterator>
#include  <type_traits>
namespace idos{
    class Data;
    std::ostream& operator<<(std::ostream& o, const Data& data);
    class DataPack{
    private:
        std::unordered_map<std::string, std::shared_ptr<Data>> data;
    public:
        void insert(const std::pair<std::string, std::shared_ptr<Data>> &pair){
            data.insert(pair);
        }    
        Data& operator[](const std::string &str){ 
            try
            {
                return this->at(str);
            }
            catch(const std::exception& e)
            {
                this->data[str] = std::make_shared<Data>(-1);
            }
            
            return *data[str];
        };

        const Data& at(const std::string &str)const{ return *data.at(str);};
        Data& at(const std::string &str){ return *data.at(str);};

        const std::unordered_map<std::string, std::shared_ptr<Data>> &getData() const{
            return this->data;
        }
        void setData(const std::unordered_map<std::string, std::shared_ptr<Data>> &data){
            this->data = data;
        }

        bool operator==(const DataPack &othr)const{
            return this->data == othr.data;
        }

        std::unordered_map<std::string, std::shared_ptr<Data>>::iterator begin(){
            return this->data.begin();
        }
        std::unordered_map<std::string, std::shared_ptr<Data>>::const_iterator begin()const{
            return this->data.begin();
        }
        std::unordered_map<std::string, std::shared_ptr<Data>>::iterator end(){
            return this->data.end();
        }
        std::unordered_map<std::string, std::shared_ptr<Data>>::const_iterator end()const{
            return this->data.end();
        }
        friend std::ostream& operator<<(std::ostream& o, const DataPack &pack);

    };
    inline std::ostream& operator<<(std::ostream& o, const DataPack &pack){
        o << "{";
        for(auto& entry : pack.getData()){
            o << entry.first << ":[" << *entry.second << "]"<<std::endl;
        }
        o << "}";
        return o;
    } 

    class DataArray{
    private:
        std::vector<std::shared_ptr<Data>> data;
    public:
        void push_back(const Data &data){
            this->data.push_back(std::make_shared<Data>(data));
        }
        const Data& operator[](int i)const{
            return *this->data[i];
        }
        Data& operator[](int i){
            return *this->data[i];
        }
        const Data& at(int i)const{
            return *this->data.at(i);
        }
        Data& at(int i){
            return *this->data.at(i);
        }
        const std::vector<std::shared_ptr<Data>> &getData()const{
            return this->data;
        }
        void setData(const std::vector<std::shared_ptr<Data>> &data){
            this->data = data;
        }
        bool operator==(const DataArray &a)const{
            return this->data == a.data;
        }
        
        std::vector<std::shared_ptr<Data>>::iterator begin(){
            return this->data.begin();
        }

        std::vector<std::shared_ptr<Data>>::const_iterator begin() const{
            return this->data.begin();
        }

        std::vector<std::shared_ptr<Data>>::iterator end(){
            return this->data.end();
        }

        std::vector<std::shared_ptr<Data>>::const_iterator end() const{
            return this->data.end();
        }
        friend std::ostream& operator<<(std::ostream& o, const DataArray& a);
    };
    inline std::ostream& operator<<(std::ostream& o, const DataArray& a){
        o<<"[";
        for(int i = 0; i < a.getData().size();i++){
            o << *a.getData().at(i);
            if((i+1) < a.getData().size())
                o<<",";
        }
        o<<"]"<<std::endl;
        return o;
    }
    
    class Data{
    private:
        typedef std::variant<std::monostate,int,std::string, unsigned int, long, unsigned long, long long, unsigned long long, double, float, bool, DataPack, DataArray, UntypedRef> Value;
        Value value;

    public:
        enum Type{
            NULLPTR,
            STRING,
            INT,
            UINT,
            LONG,
            ULONG,
            LONGLONG,
            ULONGLONG,
            DOUBLE,
            FLOAT,
            BOOL,
            OBJECT,
            ARRAY,
            REF
        };

        Type getType()const{
            switch(value.index()){
                case 0:
                    return NULLPTR;
                case 1:
                    return INT;
                case 2:
                    return STRING;
                case 3:
                    return UINT;
                case 4:
                    return LONG;
                case 5:
                    return ULONG;
                case 6:
                    return LONGLONG;
                case 7:
                    return ULONGLONG;
                case 8:
                    return DOUBLE;
                case 9:
                    return FLOAT;
                case 10:
                    return BOOL;
                case 11:
                    return OBJECT;
                case 12:
                    return ARRAY;
                case 13:
                    return REF;
                default:
                    throw IDOSException("idos::Data INTERNAL ERROR! Unknown Type Stored!!");
            }
        }

        void set(const std::string &s){
            this->value.emplace<std::string>(s);
        }

        void set(const int i){
            this->value.emplace<int>(i);
        }

        void set(const unsigned int ui){
            this->value.emplace<unsigned int>(ui);
        }

        void set(const long l){
            this->value.emplace<long>(l);
        }

        void set(const unsigned long ul){
            this->value.emplace<unsigned long>(ul);
        }

        void set(const long long ll){
            this->value.emplace<long long>(ll);
        }

        void set(const unsigned long long ll){
            this->value.emplace<unsigned long long>(ll);
        }

        void set(const float d){
            this->value.emplace<float>(d);
        }
        void set(const double d){
            this->value.emplace<double>(d);
        }

        void set(const bool b){
            this->value.emplace<bool>(b);
        }

        void set(const DataPack &nestedPack){
            this->value.emplace<DataPack>(nestedPack);
        }

        void set(const DataArray &array){
            this->value.emplace<DataArray>(array);
        }

        void set(const UntypedRef &ref){
            this->value.emplace<UntypedRef>(ref);
        }
        
        void set(const std::monostate &mono){
            this->value.emplace<std::monostate>(mono);
        }


        /// --- constructors ---
        Data(){
            this->set("Uninitialised");
        }
        Data(const std::string& t){
            this->set(t);
        }
        Data(const char* str){
            this->set(std::string(str));
        }
        Data(const int& t){
            this->set(t);
        }
        Data(const unsigned int& t){
            this->set(t);
        }
        Data(const long& t){
            this->set(t);
        }
        Data(const unsigned long& t){
            this->set(t);
        }
        Data(const long long& t){
            this->set(t);
        }
        Data(const unsigned long long& t){
            this->set(t);
        }
        Data(const float& t){
            this->set(t);
        }
        Data(const double& t){
            this->set(t);
        }
        Data(const bool& t){
            this->set(t);
        }
        Data(const DataPack& t){
            this->set(t);
        }
        Data(const DataArray& t){
            this->set(t);
        }
        Data(const UntypedRef& t){
            this->set(t);
        }
        Data(const std::monostate& mono){
            this->set(mono);
        }



        
        /*template<typename T> 
        T get()const{
            /*switch(this->getType()){
                case Data::Type::INT:
                    return std::get<int>(this->value);
                case Data::Type::UINT:
                    return std::get<unsigned int>(this->value);
                case Data::Type::LONG:
                    return std::get<long>(this->value);
                case Data::Type::ULONG:
                    return std::get<unsigned long>(this->value);
                case Data::Type::LONGLONG:
                    return std::get<int>(this->value);
                case Data::Type::ULONGLONG:
                    return std::get<unsigned long long>(this->value);
                case Data::Type::FLOAT:
                    return std::get<float>(this->value);
                case Data::Type::DOUBLE:
                    return std::get<double>(this->value);
                case Data::Type::BOOL:
                    return std::get<bool>(this->value);
                case Data::Type::STRING:
                    return std::get<std::string>(this->value);
                case Data::Type::OBJECT:
                    return std::get<DataPack>(this->value);
                case Data::Type::ARRAY:
                    return std::get<DataArray>(this->value);
            }*/
            //return this->value;
        //}
        /*template<typename T> 
        const T& get() const{
            return this->get<T>();
        }*/
    
        template<typename T>
        std::enable_if_t<std::is_base_of<UntypedRef, T>::value, T> get()const{
            if(this->getType() == NULLPTR){
                return UntypedRef();
            }
            return std::get<T>(this->value);
        }
        template<typename T>
        std::enable_if_t<(!std::is_base_of<UntypedRef, T>::value) && std::is_integral<T>::value, T> get()const{
            switch(this->getType()){
                case Data::Type::INT:
                    return (T)std::get<int>(this->value);
                case Data::Type::UINT:
                    return (T)std::get<unsigned int>(this->value);
                case Data::Type::LONG:
                    return (T)std::get<long>(this->value);
                case Data::Type::ULONG:
                    return (T)std::get<unsigned long>(this->value);
                case Data::Type::LONGLONG:
                    return (T)std::get<long long>(this->value);
                case Data::Type::ULONGLONG:
                    return (T)std::get<unsigned long long>(this->value);
                default:
                    throw IDOSException("NOT A NUMBER");
            }
        }

        template<typename T>
        std::enable_if_t<(!std::is_base_of<UntypedRef, T>::value) && (!std::is_integral<T>::value), const T&> get()const{
            return std::get<T>(this->value);
        }
        template<typename T>
        std::enable_if_t<(!std::is_base_of<UntypedRef, T>::value) && (!std::is_integral<T>::value), T&> get(){
            return std::get<T>(this->value);
        }

        
        // operators
        ///conv


        template<typename T>
        operator T()const{
            return this->get<T>();
        }

        bool operator==(const char* othr)const{return this->get<std::string>() == othr;};
        bool operator==(const std::string &othr)const{return this->get<std::string>() == othr;};
        bool operator==(const int &othr)const{return this->get<int>() == othr;};
        bool operator==(const unsigned int &othr)const{return this->get<unsigned int>() == othr;};
        bool operator==(const long &othr)const{return this->get<long>() == othr;};
        bool operator==(const unsigned long &othr)const{return this->get<unsigned long>() == othr;};
        bool operator==(const long long &othr)const{return this->get<long long>() == othr;};
        bool operator==(const unsigned long long &othr)const{return this->get<unsigned long long>() == othr;};
        bool operator==(const double &othr)const{return this->get<double>() == othr;};
        bool operator==(const float &othr)const{return this->get<float>() == othr;};
        bool operator==(const bool &othr)const{return this->get<bool>() == othr;};
        bool operator==(const DataPack &othr)const{return this->get<DataPack>() == othr;};
        bool operator==(const DataArray &othr)const{return this->get<DataArray>() == othr;};
        bool operator==(const UntypedRef &othr)const{return this->get<UntypedRef>() == othr;};
        
        Data &operator[] (const std::string &str){
            return this->get<DataPack>()[str];
        }

        Data &operator[](int i){
            return this->get<DataArray>()[i];
        }

        void push_back(const Data& data){
            this->get<DataArray>().push_back(data);
        }

        std::vector<std::shared_ptr<Data>>::iterator begin(){
            return this->get<DataArray>().begin();
        }

        std::vector<std::shared_ptr<Data>>::const_iterator begin() const{
            return this->get<DataArray>().begin();
        }

        std::vector<std::shared_ptr<Data>>::iterator end(){
            return this->get<DataArray>().end();
        }

        std::vector<std::shared_ptr<Data>>::const_iterator end() const{
            return this->get<DataArray>().end();
        }


        friend std::ostream& operator<<(std::ostream &o, const Data& data);
    };

    inline std::ostream& operator<<(std::ostream& o, const Data& data){
        switch(data.getType()){
            case Data::Type::STRING:
                o << data.get<std::string>();
                break;
            case Data::Type::INT:
                o << data.get<int>();
                break;
            case Data::Type::UINT:
                o << data.get<unsigned int>();
                break;
            case Data::Type::LONG:
                o << data.get<long>();
                break;
            case Data::Type::ULONG:
                o << data.get<unsigned long>();
                break;
            case Data::Type::LONGLONG:
                o << data.get<long long>();
                break;
            case Data::Type::ULONGLONG:
                o << data.get<unsigned long long>();
                break;
            case Data::Type::OBJECT:
                o << data.get<DataPack>();
                break;
            case Data::Type::ARRAY:
                o << data.get<DataArray>();
                break;
            case Data::Type::REF:
                o << data.get<UntypedRef>();
                break;
            default:
                o << "ERROR";
                break;
        }
        return o;
    }

    

}
