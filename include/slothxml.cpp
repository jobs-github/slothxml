#include "slothxml.h"

#define BUF_SIZE 200

typedef rapidxml::xml_attribute<char> xml_attribute_t;

namespace slothxml
{
    static rapidxml::node_type XML_ELEMENT = rapidxml::node_element;

    xml_element_t * new_element(xml_document_t& doc, const char * name)
    {
        return doc.allocate_node(XML_ELEMENT, doc.allocate_string(name));
    }
    bool set_attribute(const char * str_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        const char * key = doc.allocate_string("val");
        if (!key)
        {
            return false;
        }
        const char * val = doc.allocate_string(str_val);
        if (!val)
        {
            return false;
        }
        xml_attribute_t * attr = doc.allocate_attribute(key, val);
        if (!attr)
        {
            return false;
        }
        xml_val.append_attribute(attr);
        return true;
    }
    const char * get_attribute(const xml_element_t& xml_val, const char * key)
    {
        xml_attribute_t * attr = xml_val.first_attribute(key);
        if (!attr)
        {
            return NULL;
        }
        return attr->value();
    }

    template <typename T>
    bool set_double(const T& obj_val, const char * format_str, xml_document_t& doc, xml_element_t& xml_val)
    {
        char val[BUF_SIZE];
#ifdef WIN32
        _snprintf(val, BUF_SIZE, format_str, obj_val);
#else
        snprintf(val, BUF_SIZE, format_str, obj_val);
#endif
        return set_attribute(val, doc, xml_val);
    }

    char * __load(const char * path)
    {
        if (!path)
        {
            return NULL;
        }
        FILE * f = fopen(path, "rb");
        if (!f)
        {
            return NULL;
        }
        fseek(f, 0, SEEK_END);
        long len = ftell(f);
        fseek(f, 0, SEEK_SET);
        char * data = (char *)malloc(len + 1);
        fread(data, 1, len, f);
        data[len] = '\0';
        fclose(f);
        return data;
    }

    xml_loader_t::xml_loader_t(const char * path)
    {
        data_ = __load(path);
    }
    xml_loader_t::~xml_loader_t()
    {
        if (data_)
        {
            free(data_);
            data_ = NULL;
        }
    }
    char * xml_loader_t::c_str()
    {
        return data_;
    }
}

namespace slothxml
{
    template <typename T>
    T to_number(const std::string& str)
    {
        std::stringstream ss(str);
        T result;
        return ss >> result ? result : 0;
    }

    template <typename T>
    bool from_xml(const xml_element_t& xml_val, T& obj_val)
    {
        const char * val = get_attribute(xml_val, "val");
        if (!val)
        {
            return false;
        }
        std::stringstream ss(val);
        return ss >> obj_val;
        return true;
    }

    template <typename T>
    bool to_xml(const T& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        std::stringstream ss;
        ss << obj_val;
        set_attribute(ss.str().c_str(), doc, xml_val);
        return true;
    }
}

namespace slothxml
{
    // bool
    bool encode(const bool& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return to_xml<bool>(obj_val, doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, bool& obj_val)
    {
        return from_xml <bool> (xml_val, obj_val);
    }
    // int32_t
    bool encode(const int32_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return to_xml<int32_t>(obj_val, doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, int32_t& obj_val)
    {
        return from_xml <int32_t> (xml_val, obj_val);
    }
    bool encode(const uint32_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return to_xml<uint32_t>(obj_val, doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, uint32_t& obj_val)
    {
        return from_xml <uint32_t> (xml_val, obj_val);
    }
    // float
    bool encode(const float& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return set_double <float> (obj_val, "%.8g", doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, float & obj_val)
    {
        return from_xml <float> (xml_val, obj_val);
    }
    // double
    bool encode(const double& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return set_double <double> (obj_val, "%.17g", doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, double & obj_val)
    {
        return from_xml <double> (xml_val, obj_val);
    }
    // std::string
    bool encode(const std::string& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return set_attribute(obj_val.c_str(), doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, std::string& obj_val)
    {
        const char * val = get_attribute(xml_val, "val");
        if (!val)
        {
            return false;
        }
        obj_val = val;
        return true;
    }
}

namespace slothxml
{
    template <typename T, typename I>
    bool decode_int(const xml_element_t& xml_val, T& obj_val)
    {
        I tmp_val = 0;
        if (!decode(xml_val, tmp_val))
        {
            return false;
        }
        obj_val = static_cast <T> (tmp_val);
        return true;
    }
    // int8_t
    bool encode(const int8_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return encode(static_cast<int32_t>(obj_val), doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, int8_t& obj_val)
    {
        return decode_int <int8_t, int32_t> (xml_val, obj_val);
    }
    // uint8_t
    bool encode(const uint8_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return encode(static_cast<uint32_t>(obj_val), doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, uint8_t& obj_val)
    {
        return decode_int <uint8_t, uint32_t> (xml_val, obj_val);
    }
    // int16_t
    bool encode(const int16_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return encode(static_cast<int32_t>(obj_val), doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, int16_t& obj_val)
    {
        return decode_int <int16_t, int32_t> (xml_val, obj_val);
    }
    // uint16_t
    bool encode(const uint16_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return encode(static_cast<uint32_t>(obj_val), doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, uint16_t& obj_val)
    {
        return decode_int <uint16_t, uint32_t> (xml_val, obj_val);
    }
    // int64_t
    bool encode(const int64_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return to_xml <int64_t> (obj_val, doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, int64_t& obj_val)
    {
        return from_xml <int64_t> (xml_val, obj_val);
    }
    // uint64_t
    bool encode(const uint64_t& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        return to_xml <uint64_t> (obj_val, doc, xml_val);
    }
    bool decode(const xml_element_t& xml_val, uint64_t& obj_val)
    {
        return from_xml <uint64_t> (xml_val, obj_val);
    }
}
