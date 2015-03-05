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
#ifndef MONGOODM_FIELD_H_
#define MONGOODM_FIELD_H_

#include "value.h"

namespace mongoodm {

class Field
{
public:
    Field(const std::string &name = "")
        : name_(name), value_(NULL)
    {
    }
    virtual ~Field()
    {
        Value::SafeDelete(value_);
    }

    Field(const Field &other)
        : name_(other.name_)
        , value_(NULL)
    {
        if (other.value_ != NULL) {
            value_ = other.value_->Clone();
        }
    }

    Field& operator=(const Field &other)
    {
        CopyFrom(other);
		return *this;
    }

    void CopyFrom(const Field &other)
    {
        if (this == &other) {
            return;
        }
        name_ = other.name_;
        SetValue(other.value_, true);
    }

    virtual Field* Clone() const { return new Field(*this); }

    inline const std::string& GetName() const { return name_; }
    inline ValueType GetValueType() const { return value_->GetType(); }
    inline const Value* GetValue() const { return value_; }
    inline Value* GetValue() { return value_; }
    inline void SetName(const std::string &name) { name_ = name; }
    inline void SetValue(const Value *value, bool clone = true)
    {
        if (clone) {
            if (value_ == value) {
                return;
            }
            if (value != NULL) {
                if (NULL == value_) {
                    value_ = value->Clone();
                }
                else {
                    value_->CopyFrom(*value);
                }
            }
            else {
                Value::SafeDelete(value_);
            }
        }
        else {
            value_ = const_cast<Value*>(value);
        }
    }

    virtual std::string ToJsonString() const
    {
        std::ostringstream oss;
        oss << "\"" << name_ << "\":";
        if (value_ == NULL) {
            oss << "null";
        }
        else {
            oss << value_->ToJsonString();
        }
        return oss.str();
    }

    virtual bool BuildBson(bson_t *parent) const
    {
        assert(parent != NULL);

        if (NULL == value_) {
            return bson_append_null(parent, name_.c_str(), name_.size());
        }
        else {
            return value_->BuildBson(parent, name_);
        }
    }

    inline void SetNull()
    {
        Value::SafeDelete(value_);
        value_ = NULL;
    }

    inline bool IsNull() const { return value_ == NULL; }

protected:
    std::string name_;
    Value *value_;
};  // class Field


template <typename T_Value>
class GenericField : public Field
{
public:
    GenericField(const std::string &name = "")
        : Field(name)
    {
        value_ = new T_Value();
    }
    virtual ~GenericField()
    {
    }

    GenericField(const GenericField &other)
    {
        Field::CopyFrom(other);
    }

    GenericField& operator=(const GenericField &other)
    {
        Field::CopyFrom(other);
		return *this;
    }

    virtual GenericField* Clone() const
    {
        return new GenericField(*this);
    }

    inline const T_Value& GetValue() const
    {
        return *((T_Value*)value_);
    }

    inline T_Value& GetValue()
    {
        return *((T_Value*)value_);
    }

    inline void SetValue(const T_Value &value)
    {
        *((T_Value*)value_) = value;
    }

    inline bool FromJsonValue(const rapidjson::Value &json_value)
    {
        return ((T_Value*)value_)->FromJsonValue(json_value);
    }
};  // class GenericField

typedef GenericField<BoolValue> BoolField;
typedef GenericField<Int32Value> Int32Field;
typedef GenericField<Int64Value> Int64Field;
typedef GenericField<UInt32Value> UInt32Field;
typedef GenericField<UInt64Value> UInt64Field;
typedef GenericField<DoubleValue> DoubleField;
typedef GenericField<StringValue> StringField;
typedef GenericField<DateTimeValue> DateTimeField;
typedef GenericField<BinaryValue> BinaryField;
typedef GenericField<ObjectIdValue> ObjectIdField;
class Document;
typedef GenericField<Document> DocumentField;

template <typename T_Value>
class ArrayField : public GenericField<GenericArrayValue<T_Value> >
{
public:
    ArrayField(const std::string &name = "")
        : GenericField<GenericArrayValue<T_Value> >(name)
    {
    }
    virtual ~ArrayField()
    {
    }

    ArrayField(const ArrayField &other)
    {
        Field::CopyFrom(other);
    }

    ArrayField& operator=(const ArrayField &other)
    {
        Field::CopyFrom(other);
		return *this;
    }

    virtual ArrayField* Clone() const
    {
        return new ArrayField(*this);
    }
};  // class ArrayField

}  // namespace mongoodm

#endif  // MONGO_FIELD_H_

