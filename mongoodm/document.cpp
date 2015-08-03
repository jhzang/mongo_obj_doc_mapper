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
#include "document.h"
#include <sstream>
#include "debug.h"
#include "field.h"

namespace mongoodm {

Document::Document()
    : Value(kDocumentType)
{
}

Document::~Document()
{
    Clear();
}

Document::Document(const Document &other)
    : Value(kDocumentType)
{
    CopyFrom(other);
}

Document& Document::operator=(const Document &other)
{
    CopyFrom(other);
    return *this;
}

void Document::CopyFrom(const Value &other)
{
    if (this == &other) {
        return;
    }

    Clear();

    Value::CopyFrom(other);
    if (is_null_) {
        return;
    }

    const Document &doc = dynamic_cast<const Document&>(other);
    for (ConstFieldIterator cit = doc.FieldBegin(); cit != doc.FieldEnd(); ++cit) {
        fields_.push_back((*cit)->Clone());
    }
}

void Document::Clear()
{
    for (FieldVector::iterator it = fields_.begin(); it != fields_.end(); ++it) {
        if (*it != NULL) {
            delete *it;
            *it = NULL;
        }
    }
    fields_.clear();
}

bool Document::ParseUnknownField(const std::string &name, const rapidjson::Value &json_value)
{
    Value *value = Value::Create(json_value);
    if (NULL == value) {
        return false;
    }
    Field *field = new Field(name);
    field->SetValue(value, false);
    fields_.push_back(field);
    return true;
}

Document* Document::ParseJsonString(const char *str, size_t size/* = 0*/)
{
    Document *doc = new Document();
    if (!doc->FromJsonString(str, size)) {
        delete doc;
        doc = NULL;
    }
    return doc;
}

bool Document::FromJsonString(const char *str, size_t size/* = 0*/)
{
    rapidjson::Document d;
    if (d.ParseInsitu(const_cast<char*>(str)).HasParseError()) {
        return false;
    }
    return FromJsonValue(d);
}

bool Document::FromJsonValue(const rapidjson::Value &json_value, bool strict)
{
    Clear();

    rapidjson::Value::ConstMemberIterator it;
    for (it = json_value.MemberBegin(); it != json_value.MemberEnd(); ++it) {
        std::string name(it->name.GetString(), it->name.GetStringLength());
        //MONGOODM_LOG_TRACE("Parsing " << name << "...");
        int code = ParseField(name, it->value);
        if (code < 0) {
            MONGOODM_LOG_ERROR("parsing named field \"" << name << "\" failed" << std::endl);
            return false;
        }
        else if (0 == code) {
            //MONGOODM_LOG_TRACE("unknown...");
            if (!ParseUnknownField(name, it->value)) {
                return false;
            }
        }
        //MONGOODM_LOG_TRACE("done" << std::endl);
    }
    return true;
}

std::string Document::ToJsonString() const
{
    std::ostringstream oss;
    oss << "{";
    for (ConstFieldIterator it = FieldBegin(); it != FieldEnd(); ++it) {
        oss << (*it)->ToJsonString();
        if (it + 1 != FieldEnd()) {
            oss << ",";
        }
    }
    oss << "}";
    return oss.str();
}

bool Document::ToBson(bson_t *b) const
{
    assert(b != NULL);

    for (ConstFieldIterator it = FieldBegin(); it != FieldEnd(); ++it) {
        const Field *field = *it;
        //const std::string &name = field->GetName();
        const Value *value = field->GetValue();
        if (NULL == value || value->IsNull()) {
            // ignore null value
            //if (!bson_append_null(b, name.c_str(), name.size())) {
            //    return false;
            //}
        }
        else {
            if (!field->BuildBson(b)) {
                return false;
            }
        }
    }
    return true;
}

bool Document::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        bson_t doc;
        if (!bson_append_document_begin(parent, name.c_str(), name.size(), &doc)) {
            return false;
        }
        ToBson(&doc);
        return bson_append_document_end(parent, &doc);
    }
}

bool Document::HasField(const char *name) const
{
    for (ConstFieldIterator it = FieldBegin(); it != FieldEnd(); ++it) {
        if ((*it)->GetName() == name) {
            return true;
        }
    }
    return false;
}

Document::FieldIterator Document::FindField(const char *name)
{
    for (FieldIterator it = FieldBegin(); it != FieldEnd(); ++it) {
        if ((*it)->GetName() == name) {
            return it;
        }
    }
    return FieldEnd();
}

Document::ConstFieldIterator Document::FindField(const char *name) const
{
    for (ConstFieldIterator it = FieldBegin(); it != FieldEnd(); ++it) {
        if ((*it)->GetName() == name) {
            return it;
        }
    }
    return FieldEnd();
}

Field* Document::GetField(const char *name)
{
    FieldIterator it = FindField(name);
    return (it == FieldEnd() ? NULL : *it);
}

const Field* Document::GetField(const char *name) const
{
    ConstFieldIterator it = FindField(name);
    return (it == FieldEnd() ? NULL : *it);
}

bool Document::InsertField(size_t index, const Field *field, bool clone/* = NULL*/)
{
    if (HasField(field->GetName().c_str())) {
        return false;
    }
    if (index > fields_.size()) {
        index =  fields_.size();
    }
    fields_.insert(fields_.begin() + index, (clone ? field->Clone() : const_cast<Field*>(field)));
    return true;
}

bool Document::AddField(const Field *field, bool clone/* = NULL*/)
{
    return InsertField(fields_.size(), field, clone);
}

bool Document::DelField(const char *name)
{
    for (FieldIterator it = FieldBegin(); it != FieldEnd(); ++it) {
        if ((*it)->GetName() == name) {
            if (*it != NULL) {
                delete *it;
                *it = NULL;
            }
            fields_.erase(it);
            return true;
        }
    }
    return false;
}

}  // namespace mongoodm

