#include "color.h"

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

Color::Color()
    : red_(NULL)
	, green_(NULL)
	, blue_(NULL)
    , _has_bits_(0)
{
}

Color::~Color()
{
    Clear();
}

void Color::CopyFrom(const Value &other)
{
    if (this == &other) {
        return;
    }

    Clear();

    Document::CopyFrom(other);
    if (is_null_) {
        return;
    }

    const Color &doc = dynamic_cast<const Color&>(other);
    _has_bits_ = doc._has_bits_;
    FIX_FIELD(red, mongoodm::UInt32Field);
    FIX_FIELD(green, mongoodm::UInt32Field);
    FIX_FIELD(blue, mongoodm::UInt32Field);
}

void Color::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
	red_ = NULL;
	green_ = NULL;
	blue_ = NULL;
}

int Color::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(red, mongoodm::UInt32Field);
    HANDLE_FIELD(green, mongoodm::UInt32Field);
    HANDLE_FIELD(blue, mongoodm::UInt32Field);
    return 0;
}

void Color::clear_red()
{
    if (has_red()) {
        clear_has_red();
        DelField("red");
        red_ = NULL;
    }
}

const mongoodm::UInt32Value* Color::red() const
{
    if (!has_red()) {
        return NULL;
    }
    return &(red_->GetValue());
}

mongoodm::UInt32Value& Color::mutable_red()
{
    if (!has_red()) {
        set_has_red();
        red_= new mongoodm::UInt32Field("red");
        AddField(red_, false);
    }
    return red_->GetValue();
}

void Color::clear_green()
{
    if (has_green()) {
        clear_has_green();
        DelField("green");
        green_ = NULL;
    }
}

const mongoodm::UInt32Value* Color::green() const
{
    if (!has_green()) {
        return NULL;
    }
    return &(green_->GetValue());
}

mongoodm::UInt32Value& Color::mutable_green()
{
    if (!has_green()) {
        set_has_green();
        green_= new mongoodm::UInt32Field("green");
        AddField(green_, false);
    }
    return green_->GetValue();
}

void Color::clear_blue()
{
    if (has_blue()) {
        clear_has_blue();
        DelField("blue");
        blue_ = NULL;
    }
}

const mongoodm::UInt32Value* Color::blue() const
{
    if (!has_blue()) {
        return NULL;
    }
    return &(blue_->GetValue());
}

mongoodm::UInt32Value& Color::mutable_blue()
{
    if (!has_blue()) {
        set_has_blue();
        blue_= new mongoodm::UInt32Field("blue");
        AddField(blue_, false);
    }
    return blue_->GetValue();
}


}  // namespace entity

