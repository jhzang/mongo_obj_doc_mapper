/**
 * Copyright (c) 2015 Junheng Zang(junheng.zang@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.  
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */
#ifndef MONGOODM_DOCUMENT_H_
#define MONGOODM_DOCUMENT_H_

#include <vector>
#include <string>
#include <mongoc.h>
#include <rapidjson/rapidjson.h>
#include "value.h"

namespace mongoodm {

class Field;

class Document : public Value
{
public:
    typedef std::vector<Field*> FieldVector;
    typedef FieldVector::iterator FieldIterator;
    typedef FieldVector::const_iterator ConstFieldIterator;

public:
    Document();
    ~Document();

    Document(const Document &other);
    Document& operator=(const Document &other);
    virtual void CopyFrom(const Value &other);
    virtual Value* Clone() const { return new Document(*this); }
    void Clear();

    static Document* ParseJsonString(const char *str, size_t size = 0);
    virtual bool FromJsonString(const char *str, size_t size = 0);
    virtual bool FromJsonValue(const rapidjson::Value &json_value);
    virtual std::string ToJsonString() const;
    virtual bool ToBson(bson_t *b) const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;
    /**
     * @brief Try to parse named field
     * @return  1: named field parsed successfully
     *          0: not a named field
     *         -1: it is a named field but parsing failed(e.g. data type is not the same as declared)
     */
    virtual int ParseField(const std::string &name, const rapidjson::Value &json_value) { return 0; }

    FieldIterator FieldBegin() { return fields_.begin(); }
    ConstFieldIterator FieldBegin() const { return fields_.begin(); }
    FieldIterator FieldEnd() { return fields_.end(); }
    ConstFieldIterator FieldEnd() const { return fields_.end(); }
    size_t GetFieldCount() const { return fields_.size(); }
    bool HasField(const char *name) const;
    FieldIterator FindField(const char *name);
    ConstFieldIterator FindField(const char *name) const;
    Field* GetField(const char *name);
    const Field* GetField(const char *name) const;
    bool InsertField(size_t index, const Field *field, bool clone = true);
    bool AddField(const Field *field, bool clone = true);
    bool DelField(const char *name);

    Field* GetIdField() { return GetField("_id"); }
    const Field* GetIdField() const { return GetField("_id"); }

protected:
    bool ParseUnknownField(const std::string &name, const rapidjson::Value &json_value);

protected:
    FieldVector fields_;
};  // class Document

}  // namespace mongoodm

#endif  // MONGOODM_DOCUMENT_H_
