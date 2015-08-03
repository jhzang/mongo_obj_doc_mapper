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
#ifndef MONGOODM_VALUE_H_
#define MONGOODM_VALUE_H_

#include <cassert>
#include <string>
#include <vector>
#include <bson.h>
#include <rapidjson/document.h>

namespace mongoodm {

enum ValueType
{
    //kNullType,
    kBoolType,
    kInt32Type, 
    kInt64Type, 
    kUInt32Type, 
    kUInt64Type, 
    kDoubleType,
    kDateTimeType,
    //TIMESTAMP_FIELD,  // Used internally by mongodb
    kStringType,
    kBinaryType,
    kObjectIdType,
    kDocumentType,
    kArrayType,
    //kReferenceType,
};

class Value
{
public:
    Value(ValueType type) : type_(type), is_null_(false) {}
    virtual ~Value() {}

    virtual void CopyFrom(const Value &other)
    {
        assert(type_ == other.type_);

        if (this == &other) {
            return;
        }
        is_null_ = other.is_null_;
    }

    virtual Value* Clone() const = 0;

    ValueType GetType() const { return type_; }
    virtual bool IsNull() const { return is_null_; }
    virtual void SetNull(bool value) { is_null_ = value; }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false) = 0;
    virtual std::string ToJsonString() const = 0;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const = 0;

    static Value* Create(const rapidjson::Value &json_value);

    template<typename T_Value>
    static T_Value* CreateTypedValue(const rapidjson::Value &json_value);

    template<typename T_Value>
    static void SafeDelete(T_Value *&value);

protected:
    ValueType type_;
    bool is_null_;
};  // class Value


template <typename T_Number, ValueType value_type, bson_type_t bson_value_type>
class NumberValue : public Value
{
public:
    NumberValue(T_Number value = 0) : Value(value_type), value_(value) {}
    virtual ~NumberValue() {}

    virtual void CopyFrom(const Value &other)
    {
        if (this == &other) {
            return;
        }

        Value::CopyFrom(other);
        if (!is_null_) {
            value_ = (dynamic_cast<const NumberValue&>(other)).value_;
        }
    }

    virtual Value* Clone() const { return new NumberValue(*this); }

    inline T_Number GetValue() const { return value_; }
    inline void SetValue(T_Number value)
    {
        value_ = value;
        is_null_ = false;
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

protected:
    T_Number value_;
};

typedef NumberValue<int, kInt32Type, BSON_TYPE_INT32> Int32Value;
typedef NumberValue<long long, kInt64Type, BSON_TYPE_INT64> Int64Value;
typedef NumberValue<unsigned int, kUInt32Type, BSON_TYPE_INT32> UInt32Value;
typedef NumberValue<unsigned long long, kUInt64Type, BSON_TYPE_INT64> UInt64Value;
typedef NumberValue<double, kDoubleType, BSON_TYPE_DOUBLE> DoubleValue;

template<>
class NumberValue<bool, kBoolType, BSON_TYPE_BOOL> : public Value
{
public:
    NumberValue(bool value = true) : Value(kBoolType), value_(value) {}
    virtual ~NumberValue() {}

    virtual void CopyFrom(const Value &other)
    {
        if (this == &other) {
            return;
        }

        Value::CopyFrom(other);
        if (!is_null_) {
            value_ = (dynamic_cast<const NumberValue&>(other)).value_;
        }
    }

    virtual Value* Clone() const
    {
        return const_cast<NumberValue<bool, kBoolType, BSON_TYPE_BOOL>*>(this);
    }

    inline bool GetValue() const { return value_; }
    inline void SetValue(bool value)
    {
        value_ = value;
        is_null_ = false;
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

protected:
    bool value_;
};

typedef NumberValue<bool, kBoolType, BSON_TYPE_BOOL> BoolValue;

extern const BoolValue TrueValue;
extern const BoolValue FalseValue;

/**
 * @class DateTimeValue
 * @brief The raw value is in millisecond
 */
class DateTimeValue : public Value
{
public:
    DateTimeValue(time_t value = 0) : Value(kDateTimeType), value_(value * 1000) {}

    virtual void CopyFrom(const Value &other)
    {
        if (this == &other) {
            return;
        }

        Value::CopyFrom(other);
        if (!is_null_) {
            value_ = (dynamic_cast<const DateTimeValue&>(other)).value_;
        }
    }
 
    virtual Value* Clone() const { return new DateTimeValue(*this); }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

    inline void SetTime(time_t t_s) { value_ = (int64_t)t_s * 1000; }
    inline time_t GetTime() const { return value_ / 1000; }
    inline void SetTimeValue(struct timeval &tv)
    {
        value_ = (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000L;
    }
    inline struct timeval GetTimeValue() const
    {
        struct timeval tv = {0};
        tv.tv_sec = value_ / 1000;
        tv.tv_usec = (value_ - tv.tv_sec * 1000) * 1000;
        return tv;
    }

protected:
    int64_t value_;
};


extern std::string kEmptyString;

class StringValue : public Value
{
public:
    StringValue(const std::string &value = kEmptyString) : Value(kStringType), value_(value) {}
    StringValue(const char *str, size_t size = 0);
    virtual ~StringValue() {}

    virtual void CopyFrom(const Value &other)
    {
        if (this == &other) {
            return;
        }

        Value::CopyFrom(other);
        if (!is_null_) {
            value_ = (dynamic_cast<const StringValue&>(other)).value_;
        }
    }

    virtual Value* Clone() const
    {
        return new StringValue(*this);
    }

    inline const std::string& GetValue() const { return value_; }
    inline void SetValue(const std::string &value)
    {
        value_ = value;
        is_null_ = false;
    }
    inline void SetValue(const char *value, int n = -1)
    {
        if (NULL == value || 0 == n) {
            value_.clear();
        }
        else if (n > 0) {
            value_.assign(value, n);
        }
        else {
            value_.assign(value);
        }
        is_null_ = false;
    }

    virtual void SetNull(bool value)
    {
        is_null_ = value;
        if (is_null_) {
            value_.clear();
        }
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

private:
    std::string value_;
};


class BinaryValue: public Value
{
public:
    BinaryValue() : Value(kBinaryType) {}
    BinaryValue(bson_subtype_t binary_subtype, const std::string &binary_data)
        : Value(kBinaryType)
        , binary_subtype_(binary_subtype)
        , binary_data_(binary_data)
    {
    }
    virtual ~BinaryValue()
    {
    }

    virtual void CopyFrom(const Value &other)
    {
        if (this == &other) {
            return;
        }

        Value::CopyFrom(other);
        if (!is_null_) {
            const BinaryValue &bin = dynamic_cast<const BinaryValue&>(other);
            binary_subtype_ = bin.binary_subtype_;
            binary_data_ = bin.binary_data_;
        }
    }

    virtual Value* Clone() const { return new BinaryValue(*this); }

    inline bson_subtype_t GetSubtype() const { return binary_subtype_; }
    inline const std::string& GetData() const { return binary_data_; }
    inline void SetValue(bson_subtype_t binary_subtype, const std::string &binary_data)
    {
        is_null_ = false;
        binary_subtype_ = binary_subtype;
        binary_data_ = binary_data;
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

private:
    bson_subtype_t binary_subtype_;
    std::string binary_data_;
};


class ObjectIdValue : public Value
{
public:
    ObjectIdValue() : Value(kObjectIdType) {}
    virtual ~ObjectIdValue() {}

    virtual void CopyFrom(const Value &other)
    {
        if (this == &other) {
            return;
        }

        Value::CopyFrom(other);
        if (!is_null_) {
            value_ = (dynamic_cast<const ObjectIdValue&>(other)).value_;
        }
    }

    virtual Value* Clone() const { return new ObjectIdValue(*this); }

    inline const std::string& GetValue() const { return value_; }
    inline void SetValue(const std::string &value)
    {
        value_ = value;
        is_null_ = false;
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bson_oid_t ToBsonValue() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

private:
    std::string value_;
};


class ArrayValue : public Value
{
public:
    ArrayValue() : Value(kArrayType) {}
    virtual ~ArrayValue() { Clear(); }

    ArrayValue(const ArrayValue &other)
        : Value(kArrayType)
    {
        CopyFrom(other);
    }

    ArrayValue& operator=(const ArrayValue &other)
    {
        CopyFrom(other);
        return *this;
    }

    virtual void CopyFrom(const Value &other);

    virtual Value* Clone() const { return new ArrayValue(*this); }

    void Clear();
    inline size_t GetSize() const { return members_.size(); }

    inline Value* GetMember(size_t index)
    {
        return index >= members_.size() ? NULL : members_[index];
    }

    inline const Value* GetMember(size_t index) const
    {
        return index >= members_.size() ? NULL : members_[index];
    }

    inline bool SetMember(size_t index, const Value *value, bool clone = true)
    {
        if (index >= members_.size() || NULL == value) {
            return false;
        }

        if (members_[index] != value) {
            if (members_[index] != NULL) {
                Value::SafeDelete(members_[index]);
            }
            members_[index] = (clone ? value->Clone() : const_cast<Value*>(value));
        }
        return true;
    }

    inline void AddMember(const Value *value, bool clone = true)
    {
        is_null_ = false;
        members_.push_back(clone ? value->Clone() : const_cast<Value*>(value));
    }

    inline bool DelMember(size_t index)
    {
        if (index >= members_.size()) {
            return false;
        }
        ValueVector::iterator it = members_.begin() + index;
        Value::SafeDelete(*it);
        members_.erase(it);
        return true;
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
    virtual std::string ToJsonString() const;
    virtual bool BuildBson(bson_t *parent, const std::string &name) const;

protected:
    typedef std::vector<Value*> ValueVector;
    ValueVector members_;
};  // class ArrayValue


template<typename T_Value = Value>
class GenericArrayValue : public ArrayValue
{
public:
    GenericArrayValue() {}
    virtual ~GenericArrayValue() {}

    GenericArrayValue(const GenericArrayValue &other)
    {
        ArrayValue::CopyFrom(other);
    }

    GenericArrayValue& operator=(const GenericArrayValue &other)
    {
        ArrayValue::CopyFrom(other);
        return *this;
    }

    virtual Value* Clone() const { return new GenericArrayValue(*this); }

    T_Value* GetMember(size_t index)
    {
        return index >= members_.size() ? NULL : (T_Value*)(members_[index]);
    }
    const T_Value* GetMember(size_t index) const
    {
        return index >= members_.size() ? NULL : members_[index];
    }

    virtual bool FromJsonValue(const rapidjson::Value &json_value, bool strict = false);
};  // class GenericArrayValue


// TODO
//class ReferenceValue : public Value
//{
//};  // class ReferenceValue

}  // namespace mongoodm

#include "detail/value_impl.h"

#endif  // MONGOODM_VALUE_H_

