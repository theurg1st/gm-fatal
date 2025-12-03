#include "netvars.h"

void netvars::initialize()
{
    database.clear();

    for (c_client_class* client_class = interfaces::hl_client->get_all_classes();
        client_class != nullptr;
        client_class = client_class->p_next)
    {
        if (!client_class->p_recv_table)
            continue;

        database.emplace_back(load_table(client_class->p_recv_table));
    }
}

netvars::netvar_table_t netvars::load_table(c_recv_table* recv_table)
{
    netvar_table_t table = {};

    table.offset = 0;
    table.name = recv_table->net_table_name;

    for (int i = 0; i < recv_table->props_count; ++i)
    {
        c_recv_prop* prop = &recv_table->props[i];

        if (!prop || isdigit(prop->prop_name[0]))
            continue;

        if (prop->prop_type == dpt_data_table && prop->data_table)
        {
            table.child_tables.emplace_back(load_table(prop->data_table));
            table.child_tables.back().offset = prop->offset;
            table.child_tables.back().prop = prop;
        }
        else
        {
            table.child_props.emplace_back(prop);
        }
    }

    return table;
}

uintptr_t netvars::get_offset(const std::string& table_name, const char* prop_name)
{
    uintptr_t result = 0;

    for (const netvar_table_t& table : database)
    {
        if (table.name == table_name)
        {
            result = get_offset(table, prop_name);

            if (result != 0)
                return result;
        }
    }

    return 0;
}

uintptr_t netvars::get_offset(const netvars::netvar_table_t& table, const char* prop_name)
{
    for (const c_recv_prop* prop : table.child_props)
    {
        if (strcmp(prop->prop_name, prop_name) == 0)
            return table.offset + prop->offset;
    }

    for (const netvar_table_t& child : table.child_tables)
    {
        uintptr_t prop_offset = get_offset(child, prop_name);
        if (prop_offset != 0)
            return table.offset + prop_offset;
    }

    for (const netvar_table_t& child : table.child_tables)
    {
        if (strcmp(child.prop->prop_name, prop_name) == 0)
            return table.offset + child.offset;
    }

    return 0;
}