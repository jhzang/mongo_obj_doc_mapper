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
                return true;\
            }\
            else {\
                delete field_name##_;\
                field_name##_ = NULL;\
                return false;\
            }\
        }\
    } while (false)

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

Color::Color(const Color &other)
{
    CopyFrom(other);
}

Color& Color::operator=(const Color &other)
{
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

void Color::CopyFrom(const Color &other)
{
    Clear();
    Document::CopyFrom(other);
    _has_bits_ = other._has_bits_;
    FIX_FIELD(red, Int32Field);
    FIX_FIELD(green, Int32Field);
    FIX_FIELD(blue, Int32Field);
}

void Color::Clear()
{
    Document::Clear();

    _has_bits_ = 0;
    red_ = NULL;
    green_ = NULL;
    blue_ = NULL;
}

bool Color::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(red, Int32Field);
    HANDLE_FIELD(green, Int32Field);
    HANDLE_FIELD(blue, Int32Field);
    return false;
}

void Color::clear_red()
{
    if (has_red()) {
        clear_has_red();
        DelField("red");
        red_ = NULL;
    }
}

const Int32Value* Color::red() const
{
    if (!has_red()) {
        return NULL;
    }
    return &(red_->GetValue());
}

Int32Value& Color::mutable_red()
{
    if (!has_red()) {
        set_has_red();
        red_= new Int32Field("red");
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

const Int32Value* Color::green() const
{
    if (!has_green()) {
        return NULL;
    }
    return &(green_->GetValue());
}

Int32Value& Color::mutable_green()
{
    if (!has_green()) {
        set_has_green();
        green_ = new Int32Field("green");
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

const Int32Value* Color::blue() const
{
    if (!has_blue()) {
        return NULL;
    }
    return &(blue_->GetValue());
}

Int32Value& Color::mutable_blue()
{
    if (!has_blue()) {
        set_has_blue();
        blue_ = new Int32Field("blue");
        AddField(blue_, false);
    }
    return blue_->GetValue();
}

