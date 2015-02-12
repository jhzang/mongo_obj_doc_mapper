#include "value.h"
#include <sstream>
#include "document.h"

namespace mongoodm {

const BoolValue TrueValue(true);
const BoolValue FalseValue(false);

std::string kEmptyString;

Value* Value::Create(const rapidjson::Value &json_value)
{
    Value *value = NULL;
    bool flag = true;
    if (json_value.IsNull()) {
        //value = dynamic_cast<Value*>(const_cast<internal::NullValue*>(&NullValue));
        value = NULL;
    }
    else if (json_value.IsBool()) {
        if (json_value.IsTrue()) {
            value = const_cast<Value*>(dynamic_cast<const Value*>(&TrueValue));
        }
        else {
            value = const_cast<Value*>(dynamic_cast<const Value*>(&FalseValue));
        }
    }
    else if (json_value.IsInt()) {
        value = new Int32Value(json_value.GetInt());
    }
    else if (json_value.IsUint()) {
        value = new UInt32Value(json_value.GetUint());
    }
    else if (json_value.IsInt64()) {
        value = new Int64Value(json_value.GetInt64());
    }
    else if (json_value.IsUint64()) {
        value = new UInt64Value(json_value.GetUint64());
    }
    else if (json_value.IsDouble()) {
        value = new DoubleValue(json_value.GetDouble());
    }
    else if (json_value.IsString()) {
        value = new StringValue(json_value.GetString(), json_value.GetStringLength());
    }
    else if (json_value.IsObject()) {
        if (json_value.HasMember("$oid")) {
            value = new ObjectIdValue();
        }
        else if (json_value.HasMember("$date")) {
            value = new DateTimeValue();
        }
        else if (json_value.HasMember("$binary")) {
            value = new BinaryValue();
        }
        else {
            value = new Document();
        }
        flag = value->FromJsonValue(json_value);
    }
    else if (json_value.IsArray()) {
        value = new ArrayValue();
        flag = value->FromJsonValue(json_value);
    }

    if (value != NULL && !flag) {
        delete value;
        value = NULL;
    }
    return value;
}

bool NumberValue<bool, kBoolType, BSON_TYPE_BOOL>::FromJsonValue(const rapidjson::Value &json_value)
{
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    else if (json_value.IsBool()) {
        value_ = json_value.GetBool();
        is_null_ = false;
        return true;
    }
    else {
        return false;
    }
}

std::string NumberValue<bool, kBoolType, BSON_TYPE_BOOL>::ToJsonString() const
{
    if (is_null_) {
        return "null";
    }
    else if (value_) {
        return "true";
    }
    else {
        return "false";
    }
}

bool NumberValue<bool, kBoolType, BSON_TYPE_BOOL>::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        return bson_append_bool(parent, name.c_str(), name.size(), value_);
    }
}


bool DateTimeValue::FromJsonValue(const rapidjson::Value &json_value)
{
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    else if (json_value.IsObject()) {
        rapidjson::Value::ConstMemberIterator it_member;
        it_member = json_value.FindMember("$date");
        if (it_member == json_value.MemberEnd()) {
            return false;
        }
        assert(it_member->value.IsInt64());
        value_ = it_member->value.GetInt64();
        return true;
    }
    else {
        return false;
    }
}

std::string DateTimeValue::ToJsonString() const
{
    if (is_null_) {
        return "null";
    }
    std::ostringstream oss;
    oss << "{\"$date\":" << value_ << "}";
    return oss.str();
}

bool DateTimeValue::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        return bson_append_date_time(parent, name.c_str(), name.size(), value_);
    }
}


StringValue::StringValue(const char *str, size_t size/* = 0*/)
    : Value(kStringType)
{
    if (str != NULL) {
        if (size > 0) {
            value_.assign(str, size);
        }
        else {
            value_.assign(str);
        }
    }
}
 
bool StringValue::FromJsonValue(const rapidjson::Value &json_value)
{
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    else if (json_value.IsString()) {
        value_.assign(json_value.GetString(), json_value.GetStringLength());
        is_null_ = false;
        return true;
    }
    else {
        return false;
    }
}

std::string StringValue::ToJsonString() const
{
    if (is_null_) {
        return "null";
    }
    std::ostringstream oss;
    oss << "\"" << value_ << "\"";
    return oss.str();
}

bool StringValue::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        return bson_append_utf8(parent, name.c_str(), name.size(), value_.c_str(), value_.size());
    }
}


bool BinaryValue::FromJsonValue(const rapidjson::Value &json_value)
{
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    else if (json_value.IsObject()) {
        rapidjson::Value::ConstMemberIterator it_member;

        // $type
        it_member = json_value.FindMember("$type");
        if (it_member == json_value.MemberEnd()) {
            return false;
        }
        assert(it_member->value.IsString());
        binary_subtype_ = (bson_subtype_t)atoi(it_member->value.GetString());
        // $binary
        it_member = json_value.FindMember("$binary");
        if (it_member == json_value.MemberEnd()) {
            return false;
        }
        assert(it_member->value.IsString());
        binary_data_ = it_member->value.GetString();
        is_null_ = false;
        return true;
    }
    else {
        return false;
    }
}

std::string BinaryValue::ToJsonString() const
{
    if (is_null_) {
        return "null";
    }
    std::ostringstream oss;
    oss << "{\"$type\":\"" << std::hex << binary_subtype_ << "\",\"$binary\":\"" << binary_data_ << "\"}";
    return oss.str();
}

bool BinaryValue::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        return bson_append_binary(parent, name.c_str(), name.size(), 
                binary_subtype_, (const uint8_t*)binary_data_.c_str(), binary_data_.size());
    }
}


bool ObjectIdValue::FromJsonValue(const rapidjson::Value &json_value)
{
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    if (json_value.IsObject()) {
        rapidjson::Value::ConstMemberIterator it_member;
        it_member = json_value.FindMember("$oid");
        if (it_member == json_value.MemberEnd()) {
            return false;
        }
        assert(it_member->value.IsString());
        value_ = it_member->value.GetString();
        is_null_ = false;
        return true;
    }
    else {
        return false;
    }
}

std::string ObjectIdValue::ToJsonString() const
{
    if (is_null_) {
        return "null";
    }
    std::ostringstream oss;
    oss << "{\"$oid\":\"" << value_ << "\"}";
    return oss.str();
}

bson_oid_t ObjectIdValue::ToBsonValue() const
{
    bson_oid_t oid;
    bson_oid_init_from_string(&oid, value_.c_str());
    return oid;
}

bool ObjectIdValue::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        bson_oid_t oid;
        bson_oid_init_from_string(&oid, value_.c_str());
        return bson_append_oid(parent, name.c_str(), name.size(), &oid);
    }
}


void ArrayValue::Clear()
{
    for (ValueVector::iterator it = members_.begin(); it != members_.end(); ++it) {
        if (*it != NULL) {
            Value::SafeDelete(*it);
        }
    }
    members_.clear();
}

void ArrayValue::CopyFrom(const ArrayValue &other)
{
    Clear();

    is_null_ = other.is_null_;
    if (!is_null_) {
        for (ValueVector::const_iterator it = other.members_.begin(); it != other.members_.end(); ++it) {
            members_.push_back((*it)->Clone());
        }
    }
}

bool ArrayValue::FromJsonValue(const rapidjson::Value &json_value)
{
    is_null_ = false;
    if (json_value.IsNull()) {
        is_null_ = true;
        return true;
    }
    else if (json_value.IsArray()) {
        rapidjson::Value::ConstValueIterator it;
        for (it = json_value.Begin(); it != json_value.End(); ++it) {
            Value *v = Value::Create(*it);
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

std::string ArrayValue::ToJsonString() const
{
    if (is_null_) {
        return "null";
    }
    std::ostringstream oss;
    oss << "[";
    for (ValueVector::const_iterator it = members_.begin(); it != members_.end(); ++it) {
        oss << (*it)->ToJsonString();
        if (it + 1 != members_.end()) {
            oss << ",";
        }
    }
    oss << "]";
    return oss.str();
}

bool ArrayValue::BuildBson(bson_t *parent, const std::string &name) const
{
    assert(parent != NULL);

    if (is_null_) {
        return bson_append_null(parent, name.c_str(), name.size());
    }
    else {
        bson_t arr;
        if (!bson_append_array_begin(parent, name.c_str(), name.size(), &arr)) {
            return false;
        }
        int index = 0;
        char buf[32] = {0};
        for (ValueVector::const_iterator it = members_.begin(); it != members_.end(); ++it) {
            if (*it != NULL) {
                snprintf(buf, sizeof(buf), "%d", index);
                (*it)->BuildBson(&arr, buf);
                ++index;
            }
        }
        return bson_append_array_end(parent, &arr);
    }
}

}  // namespace mongoodm

