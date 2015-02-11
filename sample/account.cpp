#include "account.h"
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

Account_Name::Account_Name()
    : first_name_(NULL)
    , last_name_(NULL)
    , _has_bits_(0)
{
}

Account_Name::~Account_Name()
{
    Clear();
}

Account_Name::Account_Name(const Account_Name &other)
{
    CopyFrom(other);
}

Account_Name& Account_Name::operator=(const Account_Name &other)
{
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

void Account_Name::CopyFrom(const Account_Name &other)
{
    Clear();
    Document::CopyFrom(other);
    _has_bits_ = other._has_bits_;
    FIX_FIELD(first_name, StringField);
    FIX_FIELD(last_name, StringField);
}

void Account_Name::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
    first_name_ = NULL;
    last_name_ = NULL;
}

int Account_Name::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(first_name, StringField);
    HANDLE_FIELD(last_name, StringField);
    return 0;
}

void Account_Name::clear_first_name()
{
    if (has_first_name()) {
        clear_has_first_name();
        DelField("first_name");
        first_name_ = NULL;
    }
}

const StringValue* Account_Name::first_name() const
{
    if (!has_first_name()) {
        return NULL;
    }
    return &(first_name_->GetValue());
}

StringValue& Account_Name::mutable_first_name()
{
    if (!has_first_name()) {
        set_has_first_name();
        first_name_= new StringField("first_name");
        AddField(first_name_, false);
    }
    return first_name_->GetValue();
}

void Account_Name::clear_last_name()
{
    if (has_last_name()) {
        clear_has_last_name();
        DelField("last_name");
        last_name_ = NULL;
    }
}

const StringValue* Account_Name::last_name() const
{
    if (!has_last_name()) {
        return NULL;
    }
    return &(last_name_->GetValue());
}

StringValue& Account_Name::mutable_last_name()
{
    if (!has_last_name()) {
        set_has_last_name();
        last_name_= new StringField("last_name");
        AddField(last_name_, false);
    }
    return last_name_->GetValue();
}

Account_Clothes_Member::Account_Clothes_Member()
    : name_(NULL)
    , colors_(NULL)
    , _has_bits_(0)
{
}

Account_Clothes_Member::~Account_Clothes_Member()
{
    Clear();
}

Account_Clothes_Member::Account_Clothes_Member(const Account_Clothes_Member &other)
{
    CopyFrom(other);
}

Account_Clothes_Member& Account_Clothes_Member::operator=(const Account_Clothes_Member &other)
{
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

void Account_Clothes_Member::CopyFrom(const Account_Clothes_Member &other)
{
    Clear();
    Document::CopyFrom(other);
    _has_bits_ = other._has_bits_;
    FIX_FIELD(name, StringField);
    FIX_FIELD(colors, ArrayField<Color>);
}

void Account_Clothes_Member::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
    name_ = NULL;
    colors_ = NULL;
}

int Account_Clothes_Member::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(name, StringField);
    HANDLE_FIELD(colors, ArrayField<Color>);
    return 0;
}

void Account_Clothes_Member::clear_name()
{
    if (has_name()) {
        clear_has_name();
        DelField("name");
        name_ = NULL;
    }
}

const StringValue* Account_Clothes_Member::name() const
{
    if (!has_name()) {
        return NULL;
    }
    return &(name_->GetValue());
}

StringValue& Account_Clothes_Member::mutable_name()
{
    if (!has_name()) {
        set_has_name();
        name_ = new StringField("name");
        AddField(name_, false);
    }
    return name_->GetValue();
}

void Account_Clothes_Member::clear_colors()
{
    if (has_colors()) {
        clear_has_colors();
        DelField("colors");
        colors_ = NULL;
    }
}

bool Account_Clothes_Member::colors_size() const
{
    return has_colors() ? colors_->GetValue().GetSize() : 0;
}

const Color* Account_Clothes_Member::colors(size_t index) const
{
    if (!has_colors()) {
        return NULL;
    }
    return dynamic_cast<const Color*>(colors_->GetValue().GetMember(index));
}

Color* Account_Clothes_Member::mutable_colors(size_t index)
{
    if (!has_colors()) {
        return NULL;
    }
    return dynamic_cast<Color*>(colors_->GetValue().GetMember(index));
}

void Account_Clothes_Member::add_colors_member(const Color &value)
{
    if (!has_colors()) {
        set_has_colors();
        colors_ = new ArrayField<Color>("colors");
        AddField(colors_, false);
    }
    colors_->GetValue().AddMember(&value);
}

bool Account_Clothes_Member::del_colors_member(size_t index)
{
    if (!has_colors()) {
        return false;
    }
    return colors_->GetValue().DelMember(index);
}

bool Account_Clothes_Member::set_colors_member(size_t index, const Color &value)
{
    if (!has_colors()) {
        return false;
    }
    return colors_->GetValue().SetMember(index, &value);
}

GenericArrayValue<Color>& Account_Clothes_Member::mutable_colors()
{
    if (!has_colors()) {
        set_has_colors();
        colors_ = new ArrayField<Color>("colors");
        AddField(colors_, false);
    }
    return colors_->GetValue();
}


Account::Account()
    : _id_(NULL)
    , userid_(NULL)
    , username_(NULL)
    , name_(NULL)
    , male_(NULL)
    , height_(NULL)
    , age_(NULL)
    , birth_date_(NULL)
    , retire_date_(NULL)
    , death_date_(NULL)
    , photo_(NULL)
    , titles_(NULL)
    , clothes_(NULL)
    //, company_(NULL)
    , _has_bits_(0)
{
}

Account::~Account()
{
    Clear();
}

Account::Account(const Account &other)
{
    CopyFrom(other);
}

Account& Account::operator=(const Account &other)
{
    if (this != &other) {
        CopyFrom(other);
    }
    return *this;
}

void Account::CopyFrom(const Account &other)
{
    Clear();
    Document::CopyFrom(other);
    _has_bits_ = other._has_bits_;

    do {
        if (has__id()) {
            _id_ = dynamic_cast<ObjectIdField*>(GetField("_id"));
        }
        else {
            _id_ = NULL;
        }
    } while (0);


    FIX_FIELD(_id, ObjectIdField);
    FIX_FIELD(userid, UInt64Field);
    FIX_FIELD(username, StringField);
    FIX_FIELD(name, GenericField<Account_Name>);
    FIX_FIELD(male, BoolField);
    FIX_FIELD(height, DoubleField);
    FIX_FIELD(age, UInt32Field);
    FIX_FIELD(birth_date, DateTimeField);
    FIX_FIELD(retire_date, DateTimeField);
    FIX_FIELD(death_date, DateTimeField);
    FIX_FIELD(photo, BinaryField);
    FIX_FIELD(titles, ArrayField<StringValue>);
    FIX_FIELD(clothes, ArrayField<Account_Clothes_Member>);
    //FIX_FIELD(company, ReferenceField);
}

void Account::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
    _id_ = NULL;
    userid_ = NULL;
    username_ = NULL;
    name_ = NULL;
    male_ = NULL;
    height_ = NULL;
    age_ = NULL;
    birth_date_ = NULL;
    retire_date_ = NULL;
    death_date_ = NULL;
    photo_ = NULL;
    titles_ = NULL;
    clothes_ = NULL;
    //company_ = NULL;
}

int Account::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(_id, ObjectIdField);
    HANDLE_FIELD(userid, UInt64Field);
    HANDLE_FIELD(username, StringField);
    HANDLE_FIELD(name, GenericField<Account_Name>);
    HANDLE_FIELD(male, BoolField);
    HANDLE_FIELD(height, DoubleField);
    HANDLE_FIELD(age, UInt32Field);
    HANDLE_FIELD(birth_date, DateTimeField);
    HANDLE_FIELD(retire_date, DateTimeField);
    HANDLE_FIELD(death_date, DateTimeField);
    HANDLE_FIELD(photo, BinaryField);
    HANDLE_FIELD(titles, ArrayField<StringValue>);
    HANDLE_FIELD(clothes, ArrayField<Account_Clothes_Member>);
    //HANDLE_FIELD(company, ReferenceField);
    return 0;
}

void Account::clear__id()
{
    if (has__id()) {
        clear_has__id();
        DelField("_id");
        _id_ = NULL;
    }
}

const ObjectIdValue* Account::_id() const
{
    if (!has__id()) {
        return NULL;
    }
    return &(_id_->GetValue());
}

ObjectIdValue& Account::mutable__id()
{
    if (!has__id()) {
        set_has__id();
        _id_ = new ObjectIdField("_id");
        AddField(_id_, false);
    }
    return _id_->GetValue();
}

void Account::clear_userid()
{
    if (has_userid()) {
        clear_has_userid();
        DelField("userid");
        userid_ = NULL;
    }
}

const UInt64Value* Account::userid() const
{
    if (!has_userid()) {
        return NULL;
    }
    return &(userid_->GetValue());
}

UInt64Value& Account::mutable_userid()
{
    if (!has_userid()) {
        set_has_userid();
        userid_ = new UInt64Field("userid");
        AddField(userid_, false);
    }
    return userid_->GetValue();
}

void Account::clear_username()
{
    if (has_username()) {
        clear_has_username();
        DelField("username");
        username_ = NULL;
    }
}

const StringValue* Account::username() const
{
    if (!has_username()) {
        return NULL;
    }
    return &(username_->GetValue());
}

StringValue& Account::mutable_username()
{
    if (!has_username()) {
        set_has_username();
        username_ = new StringField("username");
        AddField(username_, false);
    }
    return username_->GetValue();
}

void Account::clear_name()
{
    if (has_name()) {
        clear_has_name();
        DelField("name");
        name_ = NULL;
    }
}

const Account_Name* Account::name() const
{
    if (!has_name()) {
        return NULL;
    }
    return &(name_->GetValue());
}

Account_Name& Account::mutable_name()
{
    if (!has_name()) {
        set_has_name();
        name_ = new GenericField<Account_Name>("name");
        AddField(name_, false);
    }
    return name_->GetValue();
}

void Account::clear_male()
{
    if (has_male()) {
        clear_has_male();
        DelField("male");
        male_ = NULL;
    }
}

const BoolValue* Account::male() const
{
    if (!has_male()) {
        return NULL;
    }
    return &(male_->GetValue());
}

BoolValue& Account::mutable_male()
{
    if (!has_male()) {
        set_has_male();
        male_ = new BoolField("male");
        AddField(male_, false);
    }
    return male_->GetValue();
}

void Account::clear_height()
{
    if (has_height()) {
        clear_has_height();
        DelField("height");
        height_ = NULL;
    }
}

const DoubleValue* Account::height() const
{
    if (!has_height()) {
        return NULL;
    }
    return &(height_->GetValue());
}

DoubleValue& Account::mutable_height()
{
    if (!has_height()) {
        set_has_height();
        height_ = new DoubleField("height");
        AddField(height_, false);
    }
    return height_->GetValue();
}

void Account::clear_age()
{
    if (has_age()) {
        clear_has_age();
        DelField("age");
        age_ = NULL;
    }
}

const UInt32Value* Account::age() const
{
    if (!has_age()) {
        return NULL;
    }
    return &(age_->GetValue());
}

UInt32Value& Account::mutable_age()
{
    if (!has_age()) {
        set_has_age();
        age_ = new UInt32Field("age");
        AddField(age_, false);
    }
    return age_->GetValue();
}

void Account::clear_birth_date()
{
    if (has_birth_date()) {
        clear_has_birth_date();
        DelField("birth_date");
        birth_date_ = NULL;
    }
}

const DateTimeValue* Account::birth_date() const
{
    if (!has_birth_date()) {
        return NULL;
    }
    return &(birth_date_->GetValue());
}

DateTimeValue& Account::mutable_birth_date()
{
    if (!has_birth_date()) {
        set_has_birth_date();
        birth_date_ = new DateTimeField("birth_date");
        AddField(birth_date_, false);
    }
    return birth_date_->GetValue();
}

void Account::clear_retire_date()
{
    if (has_retire_date()) {
        clear_has_retire_date();
        DelField("retire_date");
        retire_date_ = NULL;
    }
}

const DateTimeValue* Account::retire_date() const
{
    if (!has_retire_date()) {
        return NULL;
    }
    return &(retire_date_->GetValue());
}

DateTimeValue& Account::mutable_retire_date()
{
    if (!has_retire_date()) {
        set_has_retire_date();
        retire_date_ = new DateTimeField("retire_date");
        AddField(retire_date_, false);
    }
    return retire_date_->GetValue();
}

void Account::clear_death_date()
{
    if (has_death_date()) {
        clear_has_death_date();
        DelField("death_date");
        death_date_ = NULL;
    }
}

const DateTimeValue* Account::death_date() const
{
    if (!has_death_date()) {
        return NULL;
    }
    return &(death_date_->GetValue());
}

DateTimeValue& Account::mutable_death_date()
{
    if (!has_death_date()) {
        set_has_death_date();
        death_date_ = new DateTimeField("death_date");
        AddField(death_date_, false);
    }
    return death_date_->GetValue();
}

void Account::clear_photo()
{
    if (has_photo()) {
        clear_has_photo();
        DelField("photo");
        photo_ = NULL;
    }
}

const BinaryValue* Account::photo() const
{
    if (!has_photo()) {
        return NULL;
    }
    return &(photo_->GetValue());
}

BinaryValue& Account::mutable_photo()
{
    if (!has_photo()) {
        set_has_photo();
        photo_ = new BinaryField("photo");
        AddField(photo_, false);
    }
    return photo_->GetValue();
}

bool Account::titles_size() const
{
    return has_titles() ? titles_->GetValue().GetSize() : 0;
}

void Account::clear_titles()
{
    if (has_titles()) {
        clear_has_titles();
        DelField("titles");
        titles_ = NULL;
    }
}

const StringValue* Account::titles(size_t index) const
{
    if (!has_titles()) {
        return NULL;
    }
    return dynamic_cast<const StringValue*>(titles_->GetValue().GetMember(index));
}

StringValue* Account::mutable_titles(size_t index)
{
    if (!has_titles()) {
        return NULL;
    }
    return dynamic_cast<StringValue*>(titles_->GetValue().GetMember(index));
}

void Account::add_titles_member(const StringValue &value)
{
    if (!has_titles()) {
        set_has_titles();
        titles_ = new ArrayField<StringValue>("titles");
        AddField(titles_, false);
    }
    titles_->GetValue().AddMember(&value);
}

bool Account::del_titles_member(size_t index)
{
    if (!has_titles()) {
        return false;
    }
    return titles_->GetValue().DelMember(index);
}

bool Account::set_titles_member(size_t index, const StringValue &value)
{
    if (!has_titles()) {
        return false;
    }
    return titles_->GetValue().SetMember(index, &value);
}

GenericArrayValue<StringValue>& Account::mutable_titles()
{
    if (!has_titles()) {
        set_has_titles();
        titles_ = new ArrayField<StringValue>("titles");
        AddField(titles_, false);
    }
    return titles_->GetValue();
}

bool Account::clothes_size() const
{
    return has_clothes() ? clothes_->GetValue().GetSize() : 0;
}

void Account::clear_clothes()
{
    if (has_clothes()) {
        clear_has_clothes();
        DelField("clothes");
        clothes_ = NULL;
    }
}

const Account_Clothes_Member* Account::clothes(size_t index) const
{
    if (!has_clothes()) {
        return NULL;
    }
    return dynamic_cast<const Account_Clothes_Member*>(clothes_->GetValue().GetMember(index));
}

Account_Clothes_Member* Account::mutable_clothes(size_t index)
{
    if (!has_clothes()) {
        return NULL;
    }
    return dynamic_cast<Account_Clothes_Member*>(clothes_->GetValue().GetMember(index));
}

void Account::add_clothes_member(const Account_Clothes_Member &value)
{
    if (!has_clothes()) {
        set_has_clothes();
        clothes_ = new ArrayField<Account_Clothes_Member>("clothes");
        AddField(clothes_, false);
    }
    clothes_->GetValue().AddMember(&value);
}

bool Account::del_clothes_member(size_t index)
{
    if (!has_clothes()) {
        return false;
    }
    return clothes_->GetValue().DelMember(index);
}

bool Account::set_clothes_member(size_t index, const Account_Clothes_Member &value)
{
    if (!has_clothes()) {
        return false;
    }
    return clothes_->GetValue().SetMember(index, &value);
}

GenericArrayValue<Account_Clothes_Member>& Account::mutable_clothes()
{
    if (!has_clothes()) {
        set_has_clothes();
        clothes_ = new ArrayField<Account_Clothes_Member>("clothes");
        AddField(clothes_, false);
    }
    return clothes_->GetValue();
}

