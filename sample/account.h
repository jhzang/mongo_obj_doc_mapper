#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include <mongoodm/mongoodm.h>

using namespace mongoodm;

class Account_Name : public Document
{
private:
    enum {
        kFirstNameFieldNumber = 0,
        kLastNameFieldNumber = 1,
    };

public:
    Account_Name();
    ~Account_Name();

    Account_Name(const Account_Name &other);
    Account_Name& operator=(const Account_Name &other);
    void CopyFrom(const Account_Name &other);
    void Clear();

    virtual bool ParseField(const std::string &name, const rapidjson::Value &json_value);

    // first_name
    bool has_first_name() const { return has_bit(kFirstNameFieldNumber); }
    void clear_first_name();
    const StringValue* first_name() const;
    StringValue& mutable_first_name();

    // last_name
    bool has_last_name() const { return has_bit(kLastNameFieldNumber); }
    void clear_last_name();
    const StringValue* last_name() const;
    StringValue& mutable_last_name();

private:
    bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
    void set_has_first_name() { set_has_bit(kFirstNameFieldNumber); }
    void clear_has_first_name() { clear_has_bit(kFirstNameFieldNumber); }
    void set_has_last_name() { set_has_bit(kLastNameFieldNumber); }
    void clear_has_last_name() { clear_has_bit(kLastNameFieldNumber); }

private:
    StringField *first_name_;
    StringField *last_name_;

    unsigned long long _has_bits_;
};


class Color;

class Account_Clothes_Member : public Document
{
private:
    enum {
        kNameFieldNumber = 0,
        kColorsFieldNumber = 1,
    };

public:
    Account_Clothes_Member();
    ~Account_Clothes_Member();

    Account_Clothes_Member(const Account_Clothes_Member &other);
    Account_Clothes_Member& operator=(const Account_Clothes_Member &other);
    void CopyFrom(const Account_Clothes_Member &other);
    void Clear();

    virtual bool ParseField(const std::string &name, const rapidjson::Value &json_value);

    // name
    bool has_name() const { return has_bit(kNameFieldNumber); }
    void clear_name();
    const StringValue* name() const;
    StringValue& mutable_name();

    // colors
    bool has_colors() const { return has_bit(kColorsFieldNumber); }
    void clear_colors();
    bool colors_size() const;
    const Color* colors(size_t index) const;
    Color* mutable_colors(size_t index);
    void add_colors_member(const Color &value);
    bool del_colors_member(size_t index);
    bool set_colors_member(size_t index, const Color &value);
    GenericArrayValue<Color>& mutable_colors();

private:
    bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
    void set_has_name() { set_has_bit(kNameFieldNumber); }
    void clear_has_name() { clear_has_bit(kNameFieldNumber); }
    void set_has_colors() { set_has_bit(kColorsFieldNumber); }
    void clear_has_colors() { clear_has_bit(kColorsFieldNumber); }

private:
    StringField *name_;
    ArrayField<Color> *colors_;

    unsigned long long _has_bits_;
};


class Account : public Document
{
private:
    enum {
        k_idFieldNumber = 0,
        kUseridFieldNumber = 1,
        kUsernameFieldNumber = 2,
        kNameFieldNumber = 3,
        kMaleFieldNumber = 4,
        kHeightFieldNumber = 5,
        kAgeFieldNumber = 6,
        kBirthDateFieldNumber = 7,
        kRetireDateFieldNumber = 8,
        kDeathDateFieldNumber = 9,
        kPhotoFieldNumber = 10,
        kTitlesFieldNumber = 11,
        kClothesFieldNumber = 12,
        //kCompanyFieldNumber = 13,
    };

public:
    Account();
    ~Account();

    Account(const Account &other);
    Account& operator=(const Account &other);
    void CopyFrom(const Account &other);
    void Clear();

    virtual bool ParseField(const std::string &name, const rapidjson::Value &json_value);

    // _id
    bool has__id() const { return has_bit(k_idFieldNumber); }
    void clear__id();
    const ObjectIdValue* _id() const;
    ObjectIdValue& mutable__id();

    // userid
    bool has_userid() const { return has_bit(kUseridFieldNumber); }
    void clear_userid();
    const UInt64Value* userid() const;
    UInt64Value& mutable_userid();

    // username
    bool has_username() const { return has_bit(kUsernameFieldNumber); }
    void clear_username();
    const StringValue* username() const;
    StringValue& mutable_username();

    // name
    bool has_name() const { return has_bit(kNameFieldNumber); }
    void clear_name();
    const Account_Name* name() const;
    Account_Name& mutable_name();

    // male
    bool has_male() const { return has_bit(kMaleFieldNumber); }
    void clear_male();
    const BoolValue* male() const;
    BoolValue& mutable_male();

    // height
    bool has_height() const { return has_bit(kHeightFieldNumber); }
    void clear_height();
    const DoubleValue* height() const;
    DoubleValue& mutable_height();

    // age
    bool has_age() const { return has_bit(kAgeFieldNumber); }
    void clear_age();
    const UInt32Value* age() const;
    UInt32Value& mutable_age();

    // birth_date
    bool has_birth_date() const { return has_bit(kBirthDateFieldNumber); }
    void clear_birth_date();
    const DateTimeValue* birth_date() const;
    DateTimeValue& mutable_birth_date();

    // retire_date
    bool has_retire_date() const { return has_bit(kRetireDateFieldNumber); }
    void clear_retire_date();
    const DateTimeValue* retire_date() const;
    DateTimeValue& mutable_retire_date();

    // death_date
    bool has_death_date() const { return has_bit(kDeathDateFieldNumber); }
    void clear_death_date();
    const DateTimeValue* death_date() const;
    DateTimeValue& mutable_death_date();

    // photo
    bool has_photo() const { return has_bit(kPhotoFieldNumber); }
    void clear_photo();
    const BinaryValue* photo() const;
    BinaryValue& mutable_photo();

    // titles
    bool has_titles() const { return has_bit(kTitlesFieldNumber); }
    bool titles_size() const;
    void clear_titles();
    const StringValue* titles(size_t index) const;
    StringValue* mutable_titles(size_t index);
    void add_titles_member(const StringValue &value);
    bool del_titles_member(size_t index);
    bool set_titles_member(size_t index, const StringValue &value);
    GenericArrayValue<StringValue>& mutable_titles();

    // clothes
    bool has_clothes() const { return has_bit(kClothesFieldNumber); }
    bool clothes_size() const;
    void clear_clothes();
    const Account_Clothes_Member* clothes(size_t index) const;
    Account_Clothes_Member* mutable_clothes(size_t index);
    void add_clothes_member(const Account_Clothes_Member &value);
    bool del_clothes_member(size_t index);
    bool set_clothes_member(size_t index, const Account_Clothes_Member &value);
    GenericArrayValue<Account_Clothes_Member>& mutable_clothes();

    //// company: ref?
    //bool has_company() const { return has_bit(kCompanyFieldNumber); }
    //void clear_company();
    //void set_company(const std::string &value);

private:
    bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
    void set_has__id() { set_has_bit(k_idFieldNumber); }
    void clear_has__id() { clear_has_bit(k_idFieldNumber); }
    void set_has_userid() { set_has_bit(kUseridFieldNumber); }
    void clear_has_userid() { clear_has_bit(kUseridFieldNumber); }
    void set_has_username() { set_has_bit(kUsernameFieldNumber); }
    void clear_has_username() { clear_has_bit(kUsernameFieldNumber); }
    void set_has_name() { set_has_bit(kNameFieldNumber); }
    void clear_has_name() { clear_has_bit(kNameFieldNumber); }
    void set_has_male() { set_has_bit(kMaleFieldNumber); }
    void clear_has_male() { clear_has_bit(kMaleFieldNumber); }
    void set_has_height() { set_has_bit(kHeightFieldNumber); }
    void clear_has_height() { clear_has_bit(kHeightFieldNumber); }
    void set_has_age() { set_has_bit(kAgeFieldNumber); }
    void clear_has_age() { clear_has_bit(kAgeFieldNumber); }
    void set_has_birth_date() { set_has_bit(kBirthDateFieldNumber); }
    void clear_has_birth_date() { clear_has_bit(kBirthDateFieldNumber); }
    void set_has_retire_date() { set_has_bit(kRetireDateFieldNumber); }
    void clear_has_retire_date() { clear_has_bit(kRetireDateFieldNumber); }
    void set_has_death_date() { set_has_bit(kDeathDateFieldNumber); }
    void clear_has_death_date() { clear_has_bit(kDeathDateFieldNumber); }
    void set_has_photo() { set_has_bit(kPhotoFieldNumber); }
    void clear_has_photo() { clear_has_bit(kPhotoFieldNumber); }
    void set_has_titles() { set_has_bit(kTitlesFieldNumber); }
    void clear_has_titles() { clear_has_bit(kTitlesFieldNumber); }
    void set_has_clothes() { set_has_bit(kClothesFieldNumber); }
    void clear_has_clothes() { clear_has_bit(kClothesFieldNumber); }
    //void set_has_company() { set_has_bit(kCompanyFieldNumber); }
    //void clear_has_company() { clear_has_bit(kCompanyFieldNumber); }

private:
    ObjectIdField *_id_;
    UInt64Field *userid_;
    StringField *username_;
    GenericField<Account_Name> *name_;
    BoolField *male_;
    DoubleField *height_;
    UInt32Field *age_;
    DateTimeField *birth_date_;
    DateTimeField *retire_date_;
    DateTimeField *death_date_;
    BinaryField *photo_;
    ArrayField<StringValue> *titles_;
    ArrayField<Account_Clothes_Member> *clothes_;
    //ObjectIdField *company_;  // reference field

    unsigned long long _has_bits_;
};

#endif  // ACCOUNT_H_
