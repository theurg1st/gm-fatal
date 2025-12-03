#pragma once
#include "interfaces.h"

class netvars : public singleton<netvars>
{
    friend class singleton<netvars>;

    struct netvar_table_t
    {
        std::string                     name;
        c_recv_prop* prop;
        uintptr_t                       offset;
        std::vector<c_recv_prop*>       child_props;
        std::vector<netvar_table_t>     child_tables;
    };

public:
    void initialize();
    uintptr_t get_offset(const std::string& table_name, const char* prop_name);

private:
    static netvar_table_t load_table(c_recv_table* recv_table);
    static uintptr_t get_offset(const netvar_table_t& table, const char* prop_name);

private:
    std::vector<netvar_table_t>  database;
};