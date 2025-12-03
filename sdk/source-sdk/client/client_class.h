#pragma once

class c_recv_table;
class c_recv_prop;
class c_recv_proxy_data;

typedef i_client_networkable* (*create_event_fn)();
typedef i_client_networkable* (*create_client_class_fn)(int entity_index, int serial_number);

using array_length_recv_proxy_fn = void(*)(void* p_struct, int object_id, int current_array_length);
using recv_var_proxy_fn = void(*)(const c_recv_proxy_data* data, void* p_struct, void* p_out);
using data_table_recv_var_proxy_fn = void(*)(const c_recv_prop* prop, void** p_out, void* p_data, int object_id);

enum send_prop_type
{
    dpt_int = 0,
    dpt_float,
    dpt_vector,
    dpt_vector_xy,
    dpt_string,
    dpt_array,
    dpt_data_table,
    dpt_int64,
    dpt_num_send_prop_types
};

class d_variant
{
public:
    union
    {
        float   m_float;
        long    m_int;
        char*   m_string;
        void*   m_data;
        float   m_vector[3];
        __int64 m_int64;
    };

    send_prop_type type;
};

class c_recv_proxy_data
{
public:
    const c_recv_prop*  recv_prop;
    d_variant           value;
    int                 element_index;
    int                 object_id;
};

class c_recv_prop
{
public:
    char*                           prop_name;
    send_prop_type                  prop_type;
    int                             prop_flags;
    int                             buffer_size;
    int                             is_inside_of_array;
    const void*                     p_extra_data;
    c_recv_prop*                    array_prop;
    array_length_recv_proxy_fn      array_length_proxy;
    recv_var_proxy_fn               proxy;
    data_table_recv_var_proxy_fn    data_table_proxy;
    c_recv_table*                   data_table;
    int                             offset;
    int                             element_stride;
    int                             elements_count;
    const char*                     parent_array_prop_name;
};

class c_recv_table
{
public:
    c_recv_prop*    props;
    int             props_count;
    void*           p_decoder;
    char*           net_table_name;
    bool            is_initialized;
    bool            is_in_main_list;
};

class c_client_class
{
public:
    create_client_class_fn  create_client_class;
    create_event_fn         create_event;
    char*                   network_name;
    c_recv_table*           p_recv_table;
    c_client_class*         p_next;
    int                     class_id;
};