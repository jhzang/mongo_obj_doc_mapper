#!/usr/bin/env python
"""Code generator of mongoodm"""
import sys
import os
import argparse
import string
import xml.etree.cElementTree
import inflection
import pprint

#Error codes
E_SUCCESS                = 0
E_NOT_EXIST              = 1
E_INVALID_ARGUMENT       = 2
E_INSUFFICIENT_ARGUMENTS = 3
E_PARSE                  = 4
E_UNDEFINED              = 5
E_INVALID_DIR            = 6
E_INVALID_ENTITY         = 7
E_INVALID_TYPE           = 8
E_GENERATE_CODE          = 9

# constants
__version__ = '0.1.0'
description = 'Code generator of mongoodm. Developed by Junheng Zang(junheng.zang@gmail.com).'
default_config_filename = 'db.xml'
gen_base_dir = "generated"
basic_data_type_dict = { 
        'int32': {
            'field': 'mongoodm::Int32Field', 
            'value': 'mongoodm::Int32Value',
            'setter_params': 'int32_t value',
            'setter_args': 'value'
            },
        'uint32': {
            'field': 'mongoodm::UInt32Field', 
            'value': 'mongoodm::UInt32Value',
            'setter_params': 'uint32_t value',
            'setter_args': 'value'
            },
        'int64': {
            'field': 'mongoodm::Int64Field', 
            'value': 'mongoodm::Int64Value',
            'setter_params': 'int64_t value',
            'setter_args': 'value'
            }, 
        'uint64': {
            'field': 'mongoodm::UInt64Field', 
            'value': 'mongoodm::UInt64Value',
            'setter_params': 'uint64_t value',
            'setter_args': 'value'
            },
        'bool': {
            'field': 'mongoodm::BoolField', 
            'value': 'mongoodm::BoolValue',
            'setter_params': 'bool value',
            'setter_args': 'value'
            },
        'double': {
            'field': 'mongoodm::DoubleField', 
            'value': 'mongoodm::DoubleValue',
            'setter_params': 'double value',
            'setter_args': 'value'
            },
        'datetime': {
            'field': 'mongoodm::DateTimeField', 
            'value': 'mongoodm::DateTimeValue',
            'setter_params': 'time_t value',
            'setter_args': 'value'
            }, 
        'string': {
            'field': 'mongoodm::StringField', 
            'value': 'mongoodm::StringValue',
            'setter_params': 'const std::string &value',
            'setter_args': 'value'
            }, 
        'binary': {
            'field': 'mongoodm::BinaryField', 
            'value': 'mongoodm::BinaryValue',
            'setter_params': 'bson_subtype_t subtype, const std::string &data',
            'setter_args': 'subtype, data'
            },
        'objectid': {
            'field': 'mongoodm::ObjectIdField', 
            'value': 'mongoodm::ObjectIdValue',
            'setter_params': 'const std::string &value',
            'setter_args': 'value'
            }
        }

document_h_field_number_def_template = '''\
        ${field_number_tag} = ${field_number},\
'''

document_h_basic_field_accessor_template = '''\
    // ${field_name}
    bool has_${field_name}() const { return has_bit(${field_number_tag}); }
    void clear_${field_name}();
    const ${field_value_class_name}* ${field_name}() const;
    ${field_value_class_name}& mutable_${field_name}();
'''

document_h_basic_field_setter_template = '''\
    void set_${field_name}(${field_setter_params}) { mutable_${field_name}().SetValue(${field_setter_args}); }
'''

document_cpp_basic_field_method_template = '''\
void ${class_name}::clear_${field_name}()
{
    if (has_${field_name}()) {
        clear_has_${field_name}();
        DelField("${field_name}");
        ${field_name}_ = NULL;
    }
}

const ${field_value_class_name}* ${class_name}::${field_name}() const
{
    if (!has_${field_name}()) {
        return NULL;
    }
    return &(${field_name}_->GetValue());
}

${field_value_class_name}& ${class_name}::mutable_${field_name}()
{
    if (!has_${field_name}()) {
        set_has_${field_name}();
        ${field_name}_= new ${field_class_name}("${field_name}");
        AddField(${field_name}_, false);
    }
    return ${field_name}_->GetValue();
}
'''

document_h_array_field_accessor_template = '''\
    // ${field_name}
    bool has_${field_name}() const { return has_bit(${field_number_tag}); }
    void clear_${field_name}();
    bool ${field_name}_size() const;
    const ${array_member_class_name}* ${field_name}(size_t index) const;
    ${array_member_class_name}* mutable_${field_name}(size_t index);
    ${array_member_class_name}& add_${field_name}_member();
    bool del_${field_name}_member(size_t index);
    ${field_value_class_name}* ${field_name}();
    ${field_value_class_name}& mutable_${field_name}();
'''

document_cpp_array_field_method_template = '''\
bool ${class_name}::${field_name}_size() const
{
    return has_${field_name}() ? ${field_name}_->GetValue().GetSize() : 0;
}

void ${class_name}::clear_${field_name}()
{
    if (has_${field_name}()) {
        clear_has_${field_name}();
        DelField("${field_name}");
        ${field_name}_ = NULL;
    }
}

const ${array_member_class_name}* ${class_name}::${field_name}(size_t index) const
{
    if (!has_${field_name}()) {
        return NULL;
    }
    return dynamic_cast<const ${array_member_class_name}*>(${field_name}_->GetValue().GetMember(index));
}

${array_member_class_name}* ${class_name}::mutable_${field_name}(size_t index)
{
    if (!has_${field_name}()) {
        return NULL;
    }
    return dynamic_cast<${array_member_class_name}*>(${field_name}_->GetValue().GetMember(index));
}

${array_member_class_name}& ${class_name}::add_${field_name}_member()
{
    if (!has_${field_name}()) {
        set_has_${field_name}();
        ${field_name}_ = new ${field_class_name}("${field_name}");
        AddField(${field_name}_, false);
    }
    ${array_member_class_name} *value = new ${array_member_class_name}();
    ${field_name}_->GetValue().AddMember(value, false);
    return *value;
}

bool ${class_name}::del_${field_name}_member(size_t index)
{
    if (!has_${field_name}()) {
        return false;
    }
    return ${field_name}_->GetValue().DelMember(index);
}

${field_value_class_name}* ${class_name}::${field_name}()
{
    if (!has_${field_name}()) {
        return NULL;
    }
    return &(${field_name}_->GetValue());
}

${field_value_class_name}& ${class_name}::mutable_${field_name}()
{
    if (!has_${field_name}()) {
        set_has_${field_name}();
        ${field_name}_ = new ${field_class_name}("${field_name}");
        AddField(${field_name}_, false);
    }
    return ${field_name}_->GetValue();
}
'''

document_h_field_bit_method_template = '''\
    void set_has_${field_name}() { set_has_bit(${field_number_tag}); }
    void clear_has_${field_name}() { clear_has_bit(${field_number_tag}); }\
'''

document_h_field_var_def_template = '''\
    ${field_class_name} *${field_name}_;\
'''

document_h_template = '''\
#ifndef ${header_guard}
#define ${header_guard}

#include <mongoodm/mongoodm.h>
${include_file_list}
${namespace_begin}
class ${class_name} : public mongoodm::Document
{
private:
    enum {
${field_number_defs}
    };

public:
    ${class_name}();
    virtual ~${class_name}();

    ${class_name}(const ${class_name} &other);
    ${class_name}& operator=(const ${class_name} &other);
    void CopyFrom(const ${class_name} &other);
    void Clear();

    virtual bool ParseField(const std::string &name, const rapidjson::Value &json_value);

${field_accessors}
private:
    bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
${field_bit_methods}

private:
${field_var_defs}

    unsigned long long _has_bits_;
};
${namespace_end}
#endif  // ${header_guard}
'''

document_cpp_template = '''\
#include "${h_filename}"

#define FIX_FIELD(field_name, field_class) \\
    do {\\
        if (has_##field_name()) {\\
            field_name##_ = (field_class*)GetField(#field_name);\\
        }\\
        else {\\
            field_name##_ = NULL;\\
        }\\
    } while (0)

#define HANDLE_FIELD(field_name, field_class) \\
    do {\\
        if (name.compare(#field_name) == 0) {\\
            field_name##_ = new field_class(name);\\
            if (field_name##_->FromJsonValue(json_value)) {\\
                fields_.push_back(field_name##_);\\
                set_has_##field_name();\\
                return true;\\
            }\\
            else {\\
                delete field_name##_;\\
                field_name##_ = NULL;\\
                return false;\\
            }\\
        }\\
    } while (0)

${namespace_begin}
${class_name}::${class_name}()
    : ${field_initiators}
    , _has_bits_(0)
{
}

${class_name}::~${class_name}()
{
    Clear();
}

${class_name}::${class_name}(const ${class_name} &other)
{
    CopyFrom(other);
}

${class_name}& ${class_name}::operator=(const ${class_name} &other)
{
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

void ${class_name}::CopyFrom(const ${class_name} &other)
{
    Clear();
    Document::CopyFrom(other);
    _has_bits_ = other._has_bits_;
${copyfrom_fix_fields_section}
}

void ${class_name}::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
${clear_fields_section}
}

bool ${class_name}::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
${parsefield_handle_fields_section}
    return false;
}

${field_methods_impl}
${namespace_end}
'''
CMakeLists_template = '''\
project(%(project_name)s C CXX)
cmake_minimum_required(VERSION 2.8)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

# Possible values of CMAKE_BUILD_TYPE are empty, Debug, Release, RelWithDebInfo and MinSizeRel
set(CMAKE_BUILD_TYPE Debug)
message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
message(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")

# mongodb C driver
find_package(PkgConfig)
pkg_check_modules(LIBMONGOC REQUIRED libmongoc-1.0)
include_directories(${LIBMONGOC_INCLUDE_DIRS})
# rapidjson
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/rapidjson/include)
# mongoodm
set(MONGOODM_DIR ${PROJECT_SOURCE_DIR}/thirdparty/mongo_obj_doc_mapper)
set(MONGOODM_INCLUDE ${MONGOODM_DIR})
include_directories(${MONGOODM_INCLUDE})

set(SRCS %(srcs)s)

add_executable(%(exe_name)s ${SRCS} ${MONGOODM_DIR}/mongoodm/mongoodm_all.cpp)
set_target_properties(%(exe_name)s PROPERTIES OUTPUT_NAME %(exe_name)s)
target_link_libraries(myapp ${LIBMONGOC_LIBRARIES})

'''

def get_namespace_begin(namespace):
    return '' if not namespace else '\nnamespace %s {\n' % namespace

def get_namespace_end(namespace):
    return '' if not namespace else '\n}  // namespace %s\n' % namespace

def get_header_guard(namespace, class_name):
    return '%s_%s_H_' % (namespace.upper(), inflection.underscore(class_name).upper())

def construct_embeded_document_type_name(*args):
    parts = []
    for arg in args:
        parts.append(arg.capitalize())
    return '_'.join(parts)

def get_field_number_tag(field_name):
    return 'k%sFieldNumber' % inflection.camelize(field_name)

def parse_document(document_element, document_type_name, is_embeded, documents):
    #print 'Parsing document %s...' % document_type_name
    #xml.etree.cElementTree.dump(document_element)
    document = {}
    document['name'] = document_type_name
    document['is_embeded'] = is_embeded
    document['fields'] = []
    for field_element in document_element.findall('field'):
        field = {}
        if 'name' not in field_element.attrib:
            print 'document %s field no name attribute' % document['name']
            return False
        field['name'] = field_element.attrib['name']
        #print 'Parsing %s.%s...' % (document['name'], field['name'])
        if 'type' not in field_element.attrib:
            print 'document %s field no type attribute' % document['name']
            return False
        field['type'] = field_element.attrib['type']
        if field['type'] == 'document':
            if 'ref' in field_element.attrib:
                field['ref'] = field_element.attrib['ref']
            elif len(field_element) > 0:
                field['ref'] = construct_embeded_document_type_name(document['name'], field['name'])
                parse_document(field_element, field['ref'], True, documents)
            else:
                print 'document %s field %s undefined document type' % (document['name'], filed['name'])
                return False
            if field['ref'] in basic_data_type_dict:
                field['value_class_name'] = basic_data_type_dict[field['ref']]['value']
            else:
                field['value_class_name'] = field['ref']
            field['class_name'] = 'mongoodm::GenericField<%s>' % field['value_class_name']
        elif field['type'] == 'array':
            if 'ref' in field_element.attrib:
                field['ref'] = field_element.attrib['ref']
            elif len(field_element) > 0:
                field['ref'] = construct_embeded_document_type_name(document['name'], field['name'], 'Member')
                parse_document(field_element, field['ref'], True, documents)
            else:
                print 'document %s field %s unknown array member type' % (document['name'], field['name'])
                return False
            if field['ref'] in basic_data_type_dict:
                field['array_member_class_name'] = basic_data_type_dict[field['ref']]['value']
            else:
                field['array_member_class_name'] = field['ref']
            field['class_name'] = 'mongoodm::ArrayField<%s>' % field['array_member_class_name']
            field['value_class_name'] = 'mongoodm::GenericArrayValue<%s>' % field['array_member_class_name']
        else:
            field['value_class_name'] = basic_data_type_dict[field['type']]['value']
            field['class_name'] = basic_data_type_dict[field['type']]['field']
            field['setter_params'] = basic_data_type_dict[field['type']]['setter_params']
            field['setter_args'] = basic_data_type_dict[field['type']]['setter_args']
        document['fields'].append(field)
    documents[document['name']] = document
    #pprint.pprint(document)
    return True

def parse_documents(document_elements):
    if not document_elements:
        print 'database no document elements'
        return {}
    documents = {}
    for document_element in document_elements:
        if 'name' not in document_element.attrib:
            print 'document no name attribute'
            return False
        document_name = inflection.camelize(document_element.attrib['name'])
        assert parse_document(document_element, document_name, False, documents), 'Parsing document failed: %s' % document_element
    return documents

def parse(filename):
    """Entry point of config file parser"""
    if not os.access(filename, os.R_OK):
        print 'File "%s" does not exist or you do not have read permission' % filename
        return None
    db = {}
    tree = xml.etree.cElementTree.parse(filename);
    root = tree.getroot()
    assert root.tag == 'database'
    if 'name' not in root.attrib:
        print 'database no name attribute'
        return None
    db['name'] = root.attrib['name']
    db['namespace'] = ''
    if 'namespace' in root.attrib:
        db['namespace'] = root.attrib['namespace']
    db['documents'] = parse_documents(root.findall("document"))
    return db 

def generate_document_code(document, namespace, output_dir, is_overwrite_mode):
    print 'Generating document %s...' % document['name']
    document['class_name'] = document['name']
    document['h_filename'] = '%s.h' % inflection.underscore(document['name'])
    document['cpp_filename'] = '%s.cpp' % inflection.underscore(document['name'])

    index = 0
    include_files = []
    field_number_defs = []
    field_accessors = []
    field_bit_methods = []
    field_var_defs = []
    for field in document['fields']:
        # include_files
        if field['type'] not in basic_data_type_dict:
            if field['type'] != 'array' or field['ref'] not in basic_data_type_dict:
                include_files.append('#include "%s.h"' % inflection.underscore(field['ref']))
        # field_number_defs
        field_number_def = string.Template(document_h_field_number_def_template).substitute(
                field_number_tag=get_field_number_tag(field['name']),
                field_number=index)
        index += 1
        field_number_defs.append(field_number_def)
        # field_accessors
        if field['type'] != 'array':
            field_accessor = string.Template(document_h_basic_field_accessor_template).substitute(
                    field_name=field['name'],
                    field_value_class_name=field['value_class_name'],
                    field_number_tag=get_field_number_tag(field['name']))
        else:
            field_accessor = string.Template(document_h_array_field_accessor_template).substitute(
                    field_name=field['name'],
                    field_value_class_name=field['value_class_name'],
                    array_member_class_name=field['array_member_class_name'],
                    field_number_tag=get_field_number_tag(field['name']))
        # field setter
        if field['type'] in basic_data_type_dict and field['type'] != 'document':
            field_accessor += string.Template(document_h_basic_field_setter_template).substitute(
                    field_name=field['name'],
                    field_setter_params=field['setter_params'],
                    field_setter_args=field['setter_args'])
        field_accessors.append(field_accessor)
        # field_bit_methods
        field_bit_methods.append(string.Template(document_h_field_bit_method_template).substitute(
                field_name=field['name'],
                field_number_tag=get_field_number_tag(field['name'])))
        # field_var_defs
        field_var_defs.append(string.Template(document_h_field_var_def_template).substitute(
                field_name=field['name'],
                field_class_name=field['class_name']))

    document_h_pathname = os.path.join(output_dir, document['h_filename'])
    if os.path.exists(document_h_pathname) and not is_overwrite_mode:
        print 'File %s exists' % document_h_pathname
        return False, document['cpp_filename']

    document_h_content = string.Template(document_h_template).substitute(
            namespace_begin=get_namespace_begin(namespace),
            namespace_end=get_namespace_end(namespace),
            header_guard=get_header_guard(namespace, document['class_name']),
            include_file_list='\n'.join(include_files),
            class_name=document['class_name'],
            field_number_defs='\n'.join(field_number_defs),
            field_accessors='\n'.join(field_accessors),
            field_bit_methods='\n'.join(field_bit_methods),
            field_var_defs='\n'.join(field_var_defs))
    with open(document_h_pathname, 'w') as f_header:
        f_header.write(document_h_content)
    print '%s generated' % document_h_pathname

    document_cpp_pathname = os.path.join(output_dir, document['cpp_filename'])
    if os.path.exists(document_cpp_pathname) and not is_overwrite_mode:
        print 'File %s exists' % document_cpp_pathname
        return False, document['cpp_filename']
    field_initiators_list = []
    copyfrom_fix_fields_list = []
    clear_fields_list = []
    parsefield_handle_fields_list = []
    field_methods_impl_list = []
    for field in document['fields']:
        field_initiators_list.append('%s_(NULL)' % field['name'])
        copyfrom_fix_fields_list.append('\tFIX_FIELD(%s, %s);' % (field['name'], field['class_name']))
        clear_fields_list.append('\t%s_ = NULL;' % field['name'])
        parsefield_handle_fields_list.append('\tHANDLE_FIELD(%s, %s);' % (field['name'], field['class_name']))
        if field['type'] != 'array':
            field_methods_impl_list.append(string.Template(document_cpp_basic_field_method_template).substitute(
                    class_name=document['class_name'],
                    field_name=field['name'],
                    field_class_name=field['class_name'],
                    field_value_class_name=field['value_class_name']))
        else:
            field_methods_impl_list.append(string.Template(document_cpp_array_field_method_template).substitute(
                    class_name=document['class_name'],
                    field_name=field['name'],
                    field_class_name=field['class_name'],
                    field_value_class_name=field['value_class_name'],
                    array_member_class_name=field['array_member_class_name']))
    document_cpp_content = string.Template(document_cpp_template).substitute(
            namespace_begin=get_namespace_begin(namespace),
            namespace_end=get_namespace_end(namespace),
            h_filename=document['h_filename'],
            class_name=document['class_name'],
            field_initiators='\n\t, '.join(field_initiators_list),
            copyfrom_fix_fields_section='\n'.join(copyfrom_fix_fields_list), 
            clear_fields_section='\n'.join(clear_fields_list),
            parsefield_handle_fields_section='\n'.join(parsefield_handle_fields_list),
            field_methods_impl='\n'.join(field_methods_impl_list))
    with open(document_cpp_pathname, 'w') as f_source:
        f_source.write(document_cpp_content)
    print '%s generated' % document_cpp_pathname
    return True, document['cpp_filename']

def generate_CMakeLists(db, output_dir, is_overwrite_mode, src_list):
    CMakeLists_pathname = os.path.join(output_dir, 'CMakeLists.txt')
    if os.path.exists(CMakeLists_pathname) and not is_overwrite_mode:
        print 'File %s exists' % CMakeLists_pathname
        return False
    CMakeLists_content = CMakeLists_template % dict(
            project_name='myapp',
            exe_name='myapp',
            srcs=' '.join(src_list))
    with open(CMakeLists_pathname, 'w') as f:
        f.write(CMakeLists_content)
    print '%s generated' % CMakeLists_pathname
    return True

def generate_code(db, output_dir, is_overwrite_mode):
    src_list = []
    for document_name, document in db['documents'].iteritems():
        is_successful, src_file_name = generate_document_code(document, db['namespace'], output_dir, is_overwrite_mode)
        if not is_successful:
            return False
        src_list.append(src_file_name)
    if not generate_CMakeLists(db, output_dir, is_overwrite_mode, src_list):
        return False
    print 'done!'
    return True

def main():
    arg_parser = argparse.ArgumentParser(description=description)
    arg_parser.add_argument('--version', action='version', version='%(prog)s ' + __version__, help='version number')
    arg_parser.add_argument('-o', '--output', default='.', help='output directory')
    arg_parser.add_argument('--overwrite', action='store_true', help='if overwrite existing code files')
    arg_parser.add_argument('db_config', default=default_config_filename, action='store', help='database config file name')
    args = arg_parser.parse_args()
    print 'Database config file: %s' % args.db_config
    print 'Output directory: %s' % os.path.abspath(args.output)
    print 'Overwrite mode: %s' % args.overwrite

    if not os.path.exists(args.output):
        os.makedirs(args.output)

    print 'Parsing config file %s ...' % args.db_config
    dbcfg = parse(args.db_config)
    if not dbcfg:
        return E_PARSE
    print 'Generating code ...'
    if not generate_code(dbcfg, os.path.abspath(args.output), args.overwrite):
        return E_GENERATE_CODE
    return 0

if __name__ == "__main__":
    sys.exit(main())

