#ifndef __slothxml_20160529043812_h__
#define __slothxml_20160529043812_h__

#ifdef WIN32
#pragma warning(disable: 4305)
#pragma warning(disable: 4800)
#pragma warning(disable: 4996)
#endif // WIN32

#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <typeinfo>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

typedef rapidxml::xml_node<char> xml_element_t;
typedef rapidxml::xml_document<char> xml_document_t;

namespace slothxml
{
    struct xml_loader_t
    {
        xml_loader_t(const char * path);
        ~xml_loader_t();
        char * c_str();
    private:
        char * data_;
    };
    xml_element_t * new_element(xml_document_t& doc, const char * name);
}

namespace slothxml
{
    bool encode(const bool& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, bool& obj_val);

    bool encode(const int32_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, int32_t& obj_val);

    bool encode(const uint32_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, uint32_t& obj_val);

    bool encode(const float& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, float & obj_val);

    bool encode(const double& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, double & obj_val);

    bool encode(const std::string& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, std::string& obj_val);
}

namespace slothxml
{
    bool encode(const int8_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, int8_t& obj_val);

    bool encode(const uint8_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, uint8_t& obj_val);

    bool encode(const int16_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, int16_t& obj_val);

    bool encode(const uint16_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, uint16_t& obj_val);

    bool encode(const int64_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, int64_t& obj_val);

    bool encode(const uint64_t& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    bool decode(const xml_element_t& xml_val, uint64_t& obj_val);
}

namespace slothxml
{
    template <typename T>
    bool encode(const std::vector<T>& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    template <typename T>
    bool decode(const xml_element_t& xml_val, std::vector<T>& obj_val);

    template <typename T>
    bool encode(const std::map<std::string, T>& obj_val, xml_document_t& doc, xml_element_t& xml_val);
    template <typename T>
    bool decode(const xml_element_t& xml_val, std::map<std::string, T>& obj_val);
}

namespace slothxml
{
    template<typename T>
    bool encode_field(const T& field, const char * field_name, xml_document_t& doc, xml_element_t& xml_val);
    template<typename T>
    bool decode_field(const xml_element_t& xml_val, const char * field_name, T& field, bool& field_in_xml);
}

namespace slothxml
{
    template<typename T>
    bool encode(const T& obj_val, const char * root_name, std::string& xml_val);
    template<typename T>
    bool decode(const std::string& xml_val, const char * root_name, T& obj_val);
}

namespace slothxml
{
    template<typename T>
    bool dump(const T& obj_val, const char * root_name, const char * path);
    template<typename T>
    bool load(const char * path, const char * root_name, T& obj_val);
}

// implement
namespace slothxml
{
    template <typename T>
    bool encode(const std::vector<T>& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        typename std::vector<T>::const_iterator it;
        for (it = obj_val.begin(); it != obj_val.end(); ++it)
        {
            xml_element_t * child = new_element(doc, "item");
            if (!encode(*it, doc, *child))
            {
                return false;
            }
            xml_val.append_node(child);
        }
        return true;
    }
    template <typename T>
    bool decode(const xml_element_t& xml_val, std::vector<T>& obj_val)
    {
        const xml_element_t * child_ptr = xml_val.first_node("item");
        while (child_ptr)
        {
            T temp;
            if (!decode(*child_ptr, temp))
            {
                return false;
            }
            obj_val.push_back(temp);
            child_ptr = child_ptr->next_sibling();
        }
        return true;
    }

    template <typename T>
    bool encode(const std::map<std::string, T>& obj_val, xml_document_t& doc, xml_element_t& xml_val)
    {
        typename std::map<std::string, T>::const_iterator it;
        for (it = obj_val.begin(); it != obj_val.end(); ++it)
        {
            xml_element_t * child = new_element(doc, it->first.c_str());
            if (!encode(it->second, doc, *child))
            {
                return false;
            }
            xml_val.append_node(child);
        }
        return true;
    }
    template <typename T>
    bool decode(const xml_element_t& xml_val, std::map<std::string, T>& obj_val)
    {
        const xml_element_t * child_ptr = xml_val.first_node();
        while (child_ptr)
        {
            T temp;
            if (!decode(*child_ptr, temp))
            {
                return false;
            }
            obj_val[child_ptr->name()] = temp;
            child_ptr = child_ptr->next_sibling();
        }
        return true;
    }
}

namespace slothxml
{
    template<typename T>
    bool encode_field(const T& field, const char * field_name, xml_document_t& doc, xml_element_t& xml_val)
    {
        xml_element_t * child = new_element(doc, field_name);
        if (!encode(field, doc, *child))
        {
            return false;
        }
        xml_val.append_node(child);
        return true;
    }
    template<typename T>
    bool decode_field(const xml_element_t& xml_val, const char * field_name, T& field, bool& field_in_xml)
    {
        field_in_xml = false;
        const xml_element_t * child_ptr = xml_val.first_node(field_name);
        if (child_ptr)
        {
            field_in_xml = true;
            if (!decode(*child_ptr, field))
            {
                return false;
            }
        }
        return true;
    }
}

namespace slothxml
{
    template<typename T>
    bool encode(const T& obj_val, const char * root_name, std::string& xml_val)
    {
        xml_document_t doc;
        xml_element_t * root = new_element(doc, root_name);
        if (!encode(obj_val, doc, *root))
        {
            return false;
        }
        doc.append_node(root);
        rapidxml::print(std::back_inserter(xml_val), doc, 0);
        return true;
    }
    template<typename T>
    bool decode(const std::string& xml_val, const char * root_name, T& obj_val)
    {
        xml_document_t doc;
        char * xml = doc.allocate_string(xml_val.c_str());
        doc.parse <0> (xml);
        xml_element_t * root = doc.first_node(root_name);
        if (!root)
        {
            return false;
        }
        return decode(*root, obj_val);
    }
}

namespace slothxml
{
    template<typename T>
    bool dump(const T& obj_val, const char * root_name, const char * path)
    {
        xml_document_t doc;
        xml_element_t * root = new_element(doc, root_name);
        if (!encode(obj_val, doc, *root))
        {
            return false;
        }
        doc.append_node(root);
        std::ofstream os;
        try
        {
            os.open(path, std::ios::binary);
        }
        catch (...)
        {
            return false;
        }
        os << doc;
        os.close();
        return true;
    }

    template<typename T>
    bool load(const char * path, const char * root_name, T& obj_val)
    {
        xml_loader_t loader(path);
        char * xml = loader.c_str();
        if (!xml)
        {
            return false;
        }
        xml_document_t doc;
        doc.parse <0> (xml);
        xml_element_t * root = doc.first_node(root_name);
        if (!root)
        {
            return false;
        }
        return decode(*root, obj_val);
    }
}

#endif // __slothxml_20160529043812_h__
