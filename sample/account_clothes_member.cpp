#include "account_clothes_member.h"

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

Account_clothes_Member::Account_clothes_Member()
    : name_(NULL)
	, colors_(NULL)
    , _has_bits_(0)
{
}

Account_clothes_Member::~Account_clothes_Member()
{
    Clear();
}

void Account_clothes_Member::CopyFrom(const Value &other)
{
    if (this == &other) {
        return;
    }

    Clear();

    Document::CopyFrom(other);
    if (is_null_) {
        return;
    }

    const Account_clothes_Member &doc = dynamic_cast<const Account_clothes_Member&>(other);
    _has_bits_ = doc._has_bits_;
    FIX_FIELD(name, mongoodm::StringField);
    FIX_FIELD(colors, mongoodm::ArrayField<Color>);
}

void Account_clothes_Member::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
	name_ = NULL;
	colors_ = NULL;
}

int Account_clothes_Member::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(name, mongoodm::StringField);
    HANDLE_FIELD(colors, mongoodm::ArrayField<Color>);
    return 0;
}

void Account_clothes_Member::clear_name()
{
    if (has_name()) {
        clear_has_name();
        DelField("name");
        name_ = NULL;
    }
}

const mongoodm::StringValue* Account_clothes_Member::name() const
{
    if (!has_name()) {
        return NULL;
    }
    return &(name_->GetValue());
}

mongoodm::StringValue& Account_clothes_Member::mutable_name()
{
    if (!has_name()) {
        set_has_name();
        name_= new mongoodm::StringField("name");
        AddField(name_, false);
    }
    return name_->GetValue();
}

int Account_clothes_Member::colors_size() const
{
    return has_colors() ? colors_->GetValue().GetSize() : 0;
}

void Account_clothes_Member::clear_colors()
{
    if (has_colors()) {
        clear_has_colors();
        DelField("colors");
        colors_ = NULL;
    }
}

const Color* Account_clothes_Member::colors(size_t index) const
{
    if (!has_colors()) {
        return NULL;
    }
    return dynamic_cast<const Color*>(colors_->GetValue().GetMember(index));
}

Color* Account_clothes_Member::mutable_colors(size_t index)
{
    if (!has_colors()) {
        return NULL;
    }
    return dynamic_cast<Color*>(colors_->GetValue().GetMember(index));
}

Color& Account_clothes_Member::add_colors_member()
{
    if (!has_colors()) {
        set_has_colors();
        colors_ = new mongoodm::ArrayField<Color>("colors");
        AddField(colors_, false);
    }
    Color *value = new Color();
    colors_->GetValue().AddMember(value, false);
    return *value;
}

bool Account_clothes_Member::del_colors_member(size_t index)
{
    if (!has_colors()) {
        return false;
    }
    return colors_->GetValue().DelMember(index);
}

mongoodm::GenericArrayValue<Color>* Account_clothes_Member::colors()
{
    if (!has_colors()) {
        return NULL;
    }
    return &(colors_->GetValue());
}

mongoodm::GenericArrayValue<Color>& Account_clothes_Member::mutable_colors()
{
    if (!has_colors()) {
        set_has_colors();
        colors_ = new mongoodm::ArrayField<Color>("colors");
        AddField(colors_, false);
    }
    return colors_->GetValue();
}


}  // namespace entity

