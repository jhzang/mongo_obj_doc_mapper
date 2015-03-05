#include "account_name.h"

#define FIX_FIELD(field_name, field_class) \
    do {\
        if (has_##field_name()) {\
            field_name##_ = (field_class*)GetField(#field_name);\
        }\
        else {\
            field_name##_ = NULL;\
        }\
    } while (0)

#define HANDLE_FIELD(field_name, field_class) \
    do {\
        if (name.compare(#field_name) == 0) {\
            field_name##_ = new field_class(name);\
            if (field_name##_->FromJsonValue(json_value)) {\
                fields_.push_back(field_name##_);\
                set_has_##field_name();\
                return 1;\
            }\
            else {\
                delete field_name##_;\
                field_name##_ = NULL;\
                return -1;\
            }\
        }\
    } while (0)


namespace entity {

Account_name::Account_name()
    : first_name_(NULL)
	, last_name_(NULL)
    , _has_bits_(0)
{
}

Account_name::~Account_name()
{
    Clear();
}

void Account_name::CopyFrom(const Value &other)
{
    if (this == &other) {
        return;
    }

    Clear();

    Document::CopyFrom(other);
    if (is_null_) {
        return;
    }

    const Account_name &doc = dynamic_cast<const Account_name&>(other);
    _has_bits_ = doc._has_bits_;
    FIX_FIELD(first_name, mongoodm::StringField);
    FIX_FIELD(last_name, mongoodm::StringField);
}

void Account_name::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
	first_name_ = NULL;
	last_name_ = NULL;
}

int Account_name::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(first_name, mongoodm::StringField);
    HANDLE_FIELD(last_name, mongoodm::StringField);
    return 0;
}

void Account_name::clear_first_name()
{
    if (has_first_name()) {
        clear_has_first_name();
        DelField("first_name");
        first_name_ = NULL;
    }
}

const mongoodm::StringValue* Account_name::first_name() const
{
    if (!has_first_name()) {
        return NULL;
    }
    return &(first_name_->GetValue());
}

mongoodm::StringValue& Account_name::mutable_first_name()
{
    if (!has_first_name()) {
        set_has_first_name();
        first_name_= new mongoodm::StringField("first_name");
        AddField(first_name_, false);
    }
    return first_name_->GetValue();
}

void Account_name::clear_last_name()
{
    if (has_last_name()) {
        clear_has_last_name();
        DelField("last_name");
        last_name_ = NULL;
    }
}

const mongoodm::StringValue* Account_name::last_name() const
{
    if (!has_last_name()) {
        return NULL;
    }
    return &(last_name_->GetValue());
}

mongoodm::StringValue& Account_name::mutable_last_name()
{
    if (!has_last_name()) {
        set_has_last_name();
        last_name_= new mongoodm::StringField("last_name");
        AddField(last_name_, false);
    }
    return last_name_->GetValue();
}


}  // namespace entity

