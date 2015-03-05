#include "account.h"

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

Account::Account()
    : _id_(NULL)
	, userid_(NULL)
	, username_(NULL)
	, name_(NULL)
	, favorite_color_(NULL)
	, male_(NULL)
	, height_(NULL)
	, age_(NULL)
	, birth_date_(NULL)
	, retire_date_(NULL)
	, death_date_(NULL)
	, photo_(NULL)
	, titles_(NULL)
	, clothes_(NULL)
	, company_(NULL)
    , _has_bits_(0)
{
}

Account::~Account()
{
    Clear();
}

void Account::CopyFrom(const Value &other)
{
    if (this == &other) {
        return;
    }

    Clear();

    Document::CopyFrom(other);
    if (is_null_) {
        return;
    }

    const Account &doc = dynamic_cast<const Account&>(other);
    _has_bits_ = doc._has_bits_;
    FIX_FIELD(_id, mongoodm::ObjectIdField);
    FIX_FIELD(userid, mongoodm::UInt64Field);
    FIX_FIELD(username, mongoodm::StringField);
    FIX_FIELD(name, mongoodm::GenericField<Account_name>);
    FIX_FIELD(favorite_color, mongoodm::GenericField<Color>);
    FIX_FIELD(male, mongoodm::BoolField);
    FIX_FIELD(height, mongoodm::DoubleField);
    FIX_FIELD(age, mongoodm::UInt32Field);
    FIX_FIELD(birth_date, mongoodm::DateTimeField);
    FIX_FIELD(retire_date, mongoodm::DateTimeField);
    FIX_FIELD(death_date, mongoodm::DateTimeField);
    FIX_FIELD(photo, mongoodm::BinaryField);
    FIX_FIELD(titles, mongoodm::ArrayField<mongoodm::StringValue>);
    FIX_FIELD(clothes, mongoodm::ArrayField<Account_clothes_Member>);
    FIX_FIELD(company, mongoodm::ObjectIdField);
}

void Account::Clear()
{
    Document::Clear();
    _has_bits_ = 0;
	_id_ = NULL;
	userid_ = NULL;
	username_ = NULL;
	name_ = NULL;
	favorite_color_ = NULL;
	male_ = NULL;
	height_ = NULL;
	age_ = NULL;
	birth_date_ = NULL;
	retire_date_ = NULL;
	death_date_ = NULL;
	photo_ = NULL;
	titles_ = NULL;
	clothes_ = NULL;
	company_ = NULL;
}

int Account::ParseField(const std::string &name, const rapidjson::Value &json_value)
{
    HANDLE_FIELD(_id, mongoodm::ObjectIdField);
    HANDLE_FIELD(userid, mongoodm::UInt64Field);
    HANDLE_FIELD(username, mongoodm::StringField);
    HANDLE_FIELD(name, mongoodm::GenericField<Account_name>);
    HANDLE_FIELD(favorite_color, mongoodm::GenericField<Color>);
    HANDLE_FIELD(male, mongoodm::BoolField);
    HANDLE_FIELD(height, mongoodm::DoubleField);
    HANDLE_FIELD(age, mongoodm::UInt32Field);
    HANDLE_FIELD(birth_date, mongoodm::DateTimeField);
    HANDLE_FIELD(retire_date, mongoodm::DateTimeField);
    HANDLE_FIELD(death_date, mongoodm::DateTimeField);
    HANDLE_FIELD(photo, mongoodm::BinaryField);
    HANDLE_FIELD(titles, mongoodm::ArrayField<mongoodm::StringValue>);
    HANDLE_FIELD(clothes, mongoodm::ArrayField<Account_clothes_Member>);
    HANDLE_FIELD(company, mongoodm::ObjectIdField);
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

const mongoodm::ObjectIdValue* Account::_id() const
{
    if (!has__id()) {
        return NULL;
    }
    return &(_id_->GetValue());
}

mongoodm::ObjectIdValue& Account::mutable__id()
{
    if (!has__id()) {
        set_has__id();
        _id_= new mongoodm::ObjectIdField("_id");
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

const mongoodm::UInt64Value* Account::userid() const
{
    if (!has_userid()) {
        return NULL;
    }
    return &(userid_->GetValue());
}

mongoodm::UInt64Value& Account::mutable_userid()
{
    if (!has_userid()) {
        set_has_userid();
        userid_= new mongoodm::UInt64Field("userid");
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

const mongoodm::StringValue* Account::username() const
{
    if (!has_username()) {
        return NULL;
    }
    return &(username_->GetValue());
}

mongoodm::StringValue& Account::mutable_username()
{
    if (!has_username()) {
        set_has_username();
        username_= new mongoodm::StringField("username");
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

const Account_name* Account::name() const
{
    if (!has_name()) {
        return NULL;
    }
    return &(name_->GetValue());
}

Account_name& Account::mutable_name()
{
    if (!has_name()) {
        set_has_name();
        name_= new mongoodm::GenericField<Account_name>("name");
        AddField(name_, false);
    }
    return name_->GetValue();
}

void Account::clear_favorite_color()
{
    if (has_favorite_color()) {
        clear_has_favorite_color();
        DelField("favorite_color");
        favorite_color_ = NULL;
    }
}

const Color* Account::favorite_color() const
{
    if (!has_favorite_color()) {
        return NULL;
    }
    return &(favorite_color_->GetValue());
}

Color& Account::mutable_favorite_color()
{
    if (!has_favorite_color()) {
        set_has_favorite_color();
        favorite_color_= new mongoodm::GenericField<Color>("favorite_color");
        AddField(favorite_color_, false);
    }
    return favorite_color_->GetValue();
}

void Account::clear_male()
{
    if (has_male()) {
        clear_has_male();
        DelField("male");
        male_ = NULL;
    }
}

const mongoodm::BoolValue* Account::male() const
{
    if (!has_male()) {
        return NULL;
    }
    return &(male_->GetValue());
}

mongoodm::BoolValue& Account::mutable_male()
{
    if (!has_male()) {
        set_has_male();
        male_= new mongoodm::BoolField("male");
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

const mongoodm::DoubleValue* Account::height() const
{
    if (!has_height()) {
        return NULL;
    }
    return &(height_->GetValue());
}

mongoodm::DoubleValue& Account::mutable_height()
{
    if (!has_height()) {
        set_has_height();
        height_= new mongoodm::DoubleField("height");
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

const mongoodm::UInt32Value* Account::age() const
{
    if (!has_age()) {
        return NULL;
    }
    return &(age_->GetValue());
}

mongoodm::UInt32Value& Account::mutable_age()
{
    if (!has_age()) {
        set_has_age();
        age_= new mongoodm::UInt32Field("age");
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

const mongoodm::DateTimeValue* Account::birth_date() const
{
    if (!has_birth_date()) {
        return NULL;
    }
    return &(birth_date_->GetValue());
}

mongoodm::DateTimeValue& Account::mutable_birth_date()
{
    if (!has_birth_date()) {
        set_has_birth_date();
        birth_date_= new mongoodm::DateTimeField("birth_date");
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

const mongoodm::DateTimeValue* Account::retire_date() const
{
    if (!has_retire_date()) {
        return NULL;
    }
    return &(retire_date_->GetValue());
}

mongoodm::DateTimeValue& Account::mutable_retire_date()
{
    if (!has_retire_date()) {
        set_has_retire_date();
        retire_date_= new mongoodm::DateTimeField("retire_date");
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

const mongoodm::DateTimeValue* Account::death_date() const
{
    if (!has_death_date()) {
        return NULL;
    }
    return &(death_date_->GetValue());
}

mongoodm::DateTimeValue& Account::mutable_death_date()
{
    if (!has_death_date()) {
        set_has_death_date();
        death_date_= new mongoodm::DateTimeField("death_date");
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

const mongoodm::BinaryValue* Account::photo() const
{
    if (!has_photo()) {
        return NULL;
    }
    return &(photo_->GetValue());
}

mongoodm::BinaryValue& Account::mutable_photo()
{
    if (!has_photo()) {
        set_has_photo();
        photo_= new mongoodm::BinaryField("photo");
        AddField(photo_, false);
    }
    return photo_->GetValue();
}

int Account::titles_size() const
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

const mongoodm::StringValue* Account::titles(size_t index) const
{
    if (!has_titles()) {
        return NULL;
    }
    return dynamic_cast<const mongoodm::StringValue*>(titles_->GetValue().GetMember(index));
}

mongoodm::StringValue* Account::mutable_titles(size_t index)
{
    if (!has_titles()) {
        return NULL;
    }
    return dynamic_cast<mongoodm::StringValue*>(titles_->GetValue().GetMember(index));
}

mongoodm::StringValue& Account::add_titles_member()
{
    if (!has_titles()) {
        set_has_titles();
        titles_ = new mongoodm::ArrayField<mongoodm::StringValue>("titles");
        AddField(titles_, false);
    }
    mongoodm::StringValue *value = new mongoodm::StringValue();
    titles_->GetValue().AddMember(value, false);
    return *value;
}

bool Account::del_titles_member(size_t index)
{
    if (!has_titles()) {
        return false;
    }
    return titles_->GetValue().DelMember(index);
}

mongoodm::GenericArrayValue<mongoodm::StringValue>* Account::titles()
{
    if (!has_titles()) {
        return NULL;
    }
    return &(titles_->GetValue());
}

mongoodm::GenericArrayValue<mongoodm::StringValue>& Account::mutable_titles()
{
    if (!has_titles()) {
        set_has_titles();
        titles_ = new mongoodm::ArrayField<mongoodm::StringValue>("titles");
        AddField(titles_, false);
    }
    return titles_->GetValue();
}

int Account::clothes_size() const
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

const Account_clothes_Member* Account::clothes(size_t index) const
{
    if (!has_clothes()) {
        return NULL;
    }
    return dynamic_cast<const Account_clothes_Member*>(clothes_->GetValue().GetMember(index));
}

Account_clothes_Member* Account::mutable_clothes(size_t index)
{
    if (!has_clothes()) {
        return NULL;
    }
    return dynamic_cast<Account_clothes_Member*>(clothes_->GetValue().GetMember(index));
}

Account_clothes_Member& Account::add_clothes_member()
{
    if (!has_clothes()) {
        set_has_clothes();
        clothes_ = new mongoodm::ArrayField<Account_clothes_Member>("clothes");
        AddField(clothes_, false);
    }
    Account_clothes_Member *value = new Account_clothes_Member();
    clothes_->GetValue().AddMember(value, false);
    return *value;
}

bool Account::del_clothes_member(size_t index)
{
    if (!has_clothes()) {
        return false;
    }
    return clothes_->GetValue().DelMember(index);
}

mongoodm::GenericArrayValue<Account_clothes_Member>* Account::clothes()
{
    if (!has_clothes()) {
        return NULL;
    }
    return &(clothes_->GetValue());
}

mongoodm::GenericArrayValue<Account_clothes_Member>& Account::mutable_clothes()
{
    if (!has_clothes()) {
        set_has_clothes();
        clothes_ = new mongoodm::ArrayField<Account_clothes_Member>("clothes");
        AddField(clothes_, false);
    }
    return clothes_->GetValue();
}

void Account::clear_company()
{
    if (has_company()) {
        clear_has_company();
        DelField("company");
        company_ = NULL;
    }
}

const mongoodm::ObjectIdValue* Account::company() const
{
    if (!has_company()) {
        return NULL;
    }
    return &(company_->GetValue());
}

mongoodm::ObjectIdValue& Account::mutable_company()
{
    if (!has_company()) {
        set_has_company();
        company_= new mongoodm::ObjectIdField("company");
        AddField(company_, false);
    }
    return company_->GetValue();
}


}  // namespace entity

