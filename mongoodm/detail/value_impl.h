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
#ifndef MONGOODM_DETAIL_VALUE_IMPL_H_
#define MONGOODM_DETAIL_VALUE_IMPL_H_

#include <sstream>

namespace mongoodm {

template<typename T_Value>
T_Value* Value::CreateTypedValue(const rapidjson::Value &json_value)
{
    T_Value *v = new T_Value();
    if (!v->FromJsonValue(json_value)) {
        delete v;
        v = NULL;
    }
    return v;
}

template<typename T_Value>
void Value::SafeDelete(T_Value *&value)
{
    if (value != NULL && value->GetType() != kBoolType) {
        delete value;
        value = NULL;
    }
}


template <typename T_Number, ValueType value_type, bson_type_t bson_value_type>
bool NumberValue<T_Number, value_type, bson_value_type>::FromJsonValue(const rapidjson::Value &json_value)
{
    is_null_ = false;
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }

    bool retflag = false;
    switch (value_type) {
        case kBoolType:
            if (json_value.IsBool()) {
                value_ = json_value.GetBool();
                retflag = true;
            }
            break;
        case kInt32Type:
            if (json_value.IsInt()) {
                value_ = json_value.GetInt();
                retflag = true;
            }
            break;
        case kUInt32Type:
            if (json_value.IsUint()) {
                value_ = json_value.GetUint();
                retflag = true;
            }
            break;
        case kInt64Type:
            if (json_value.IsInt64()) {
                value_ = json_value.GetInt64();
                retflag = true;
            }
            break;
        case kUInt64Type:
            if (json_value.IsUint64()) {
                value_ = json_value.GetUint64();
                retflag = true;
            }
            break;
        case kDoubleType:
            if (json_value.IsDouble()) {
                value_ = json_value.GetDouble();
                retflag = true;
            }
            break;
        case kDateTimeType:
            if (json_value.IsInt64()) {
                value_ = json_value.GetInt64();
                retflag = true;
            }
            break;
        default:
            assert(false);
            retflag = false;
    }
    return retflag;
}

template <typename T_Number, ValueType value_type, bson_type_t bson_value_type>
std::string NumberValue<T_Number, value_type, bson_value_type>::ToJsonString() const
{
    // sample: 
    // "future_date" : { "$date" : 60526227600000 }, 
    if (is_null_) {
        return "null";
    }
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}

template <typename T_Number, ValueType value_type, bson_type_t bson_value_type>
bool NumberValue<T_Number, value_type, bson_value_type>::BuildBson(bson_t *parent, const std::string &name) const
{
    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        bool retflag = false;
        switch (bson_value_type) {
            case BSON_TYPE_BOOL:
                retflag = bson_append_bool(parent, name.c_str(), name.size(), value_);
                break;
            case BSON_TYPE_INT32:
                retflag = bson_append_int32(parent, name.c_str(), name.size(), value_);
                break;
            case BSON_TYPE_INT64:
                retflag = bson_append_int64(parent, name.c_str(), name.size(), value_);
                break;
            case BSON_TYPE_DOUBLE:
                retflag = bson_append_double(parent, name.c_str(), name.size(), value_);
                break;
            default:
                assert(false);
        }
        return retflag;
    }
}


template<typename T_Value>
bool GenericArrayValue<T_Value>::FromJsonValue(const rapidjson::Value &json_value)
{
    is_null_ = false;
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    else if (json_value.IsArray()) {
        rapidjson::Value::ConstValueIterator it;
        for (it = json_value.Begin(); it != json_value.End(); ++it) {
            T_Value *v = Value::CreateTypedValue<T_Value>(*it);
            if (v == NULL) {
                return false;
            }
            members_.push_back(v);
        }
        return true;
    }
    else {
        return false;
    }
}

}  // namespace mongoodm

#endif  // MONGOODM_DETAIL_VALUE_IMPL_H_

