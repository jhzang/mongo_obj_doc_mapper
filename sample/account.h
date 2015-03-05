#ifndef ENTITY_ACCOUNT_H_
#define ENTITY_ACCOUNT_H_

#include <mongoodm/mongoodm.h>
#include "account_name.h"
#include "color.h"
#include "account_clothes_member.h"

namespace entity {

class Account : public mongoodm::Document
{
private:
    enum {
        k_idFieldNumber = 0,
        kUseridFieldNumber = 1,
        kUsernameFieldNumber = 2,
        kNameFieldNumber = 3,
        kFavoriteColorFieldNumber = 4,
        kMaleFieldNumber = 5,
        kHeightFieldNumber = 6,
        kAgeFieldNumber = 7,
        kBirthDateFieldNumber = 8,
        kRetireDateFieldNumber = 9,
        kDeathDateFieldNumber = 10,
        kPhotoFieldNumber = 11,
        kTitlesFieldNumber = 12,
        kClothesFieldNumber = 13,
        kCompanyFieldNumber = 14,
    };

public:
    Account();
    virtual ~Account();

    Account(const Account &other) { CopyFrom(other); }
    Account& operator=(const Account &other) { CopyFrom(other); return *this; }
    virtual void CopyFrom(const Value &other);
    virtual Value* Clone() const { return new Account(*this); }
    void Clear();

    virtual int ParseField(const std::string &name, const rapidjson::Value &json_value);

    // _id
    inline bool has__id() const { return has_bit(k_idFieldNumber); }
    void clear__id();
    const mongoodm::ObjectIdValue* _id() const;
    mongoodm::ObjectIdValue& mutable__id();
    inline std::string _id_value() const { return _id_->GetValue().GetValue(); }
    inline void set__id(const std::string &value) { mutable__id().SetValue(value); }

    // userid
    inline bool has_userid() const { return has_bit(kUseridFieldNumber); }
    void clear_userid();
    const mongoodm::UInt64Value* userid() const;
    mongoodm::UInt64Value& mutable_userid();
    inline uint64_t userid_value() const { return userid_->GetValue().GetValue(); }
    inline void set_userid(uint64_t value) { mutable_userid().SetValue(value); }

    // username
    inline bool has_username() const { return has_bit(kUsernameFieldNumber); }
    void clear_username();
    const mongoodm::StringValue* username() const;
    mongoodm::StringValue& mutable_username();
    inline std::string username_value() const { return username_->GetValue().GetValue(); }
    inline void set_username(const std::string &value) { mutable_username().SetValue(value); }

    // name
    inline bool has_name() const { return has_bit(kNameFieldNumber); }
    void clear_name();
    const Account_name* name() const;
    Account_name& mutable_name();

    // favorite_color
    inline bool has_favorite_color() const { return has_bit(kFavoriteColorFieldNumber); }
    void clear_favorite_color();
    const Color* favorite_color() const;
    Color& mutable_favorite_color();

    // male
    inline bool has_male() const { return has_bit(kMaleFieldNumber); }
    void clear_male();
    const mongoodm::BoolValue* male() const;
    mongoodm::BoolValue& mutable_male();
    inline bool male_value() const { return male_->GetValue().GetValue(); }
    inline void set_male(bool value) { mutable_male().SetValue(value); }

    // height
    inline bool has_height() const { return has_bit(kHeightFieldNumber); }
    void clear_height();
    const mongoodm::DoubleValue* height() const;
    mongoodm::DoubleValue& mutable_height();
    inline double height_value() const { return height_->GetValue().GetValue(); }
    inline void set_height(double value) { mutable_height().SetValue(value); }

    // age
    inline bool has_age() const { return has_bit(kAgeFieldNumber); }
    void clear_age();
    const mongoodm::UInt32Value* age() const;
    mongoodm::UInt32Value& mutable_age();
    inline uint32_t age_value() const { return age_->GetValue().GetValue(); }
    inline void set_age(uint32_t value) { mutable_age().SetValue(value); }

    // birth_date
    inline bool has_birth_date() const { return has_bit(kBirthDateFieldNumber); }
    void clear_birth_date();
    const mongoodm::DateTimeValue* birth_date() const;
    mongoodm::DateTimeValue& mutable_birth_date();
    inline time_t birth_date_value() const { return birth_date_->GetValue().GetTime(); }
    inline void set_birth_date(time_t t) { mutable_birth_date().SetTime(t); }
    inline void set_birth_date(struct timeval &tv) { mutable_birth_date().SetTimeValue(tv); }

    // retire_date
    inline bool has_retire_date() const { return has_bit(kRetireDateFieldNumber); }
    void clear_retire_date();
    const mongoodm::DateTimeValue* retire_date() const;
    mongoodm::DateTimeValue& mutable_retire_date();
    inline time_t retire_date_value() const { return retire_date_->GetValue().GetTime(); }
    inline void set_retire_date(time_t t) { mutable_retire_date().SetTime(t); }
    inline void set_retire_date(struct timeval &tv) { mutable_retire_date().SetTimeValue(tv); }

    // death_date
    inline bool has_death_date() const { return has_bit(kDeathDateFieldNumber); }
    void clear_death_date();
    const mongoodm::DateTimeValue* death_date() const;
    mongoodm::DateTimeValue& mutable_death_date();
    inline time_t death_date_value() const { return death_date_->GetValue().GetTime(); }
    inline void set_death_date(time_t t) { mutable_death_date().SetTime(t); }
    inline void set_death_date(struct timeval &tv) { mutable_death_date().SetTimeValue(tv); }

    // photo
    inline bool has_photo() const { return has_bit(kPhotoFieldNumber); }
    void clear_photo();
    const mongoodm::BinaryValue* photo() const;
    mongoodm::BinaryValue& mutable_photo();
    inline void set_photo(bson_subtype_t subtype, const std::string &data) { mutable_photo().SetValue(subtype, data); }

    // titles
    inline bool has_titles() const { return has_bit(kTitlesFieldNumber); }
    void clear_titles();
    int titles_size() const;
    const mongoodm::StringValue* titles(size_t index) const;
    mongoodm::StringValue* mutable_titles(size_t index);
    mongoodm::StringValue& add_titles_member();
    bool del_titles_member(size_t index);
    mongoodm::GenericArrayValue<mongoodm::StringValue>* titles();
    mongoodm::GenericArrayValue<mongoodm::StringValue>& mutable_titles();

    // clothes
    inline bool has_clothes() const { return has_bit(kClothesFieldNumber); }
    void clear_clothes();
    int clothes_size() const;
    const Account_clothes_Member* clothes(size_t index) const;
    Account_clothes_Member* mutable_clothes(size_t index);
    Account_clothes_Member& add_clothes_member();
    bool del_clothes_member(size_t index);
    mongoodm::GenericArrayValue<Account_clothes_Member>* clothes();
    mongoodm::GenericArrayValue<Account_clothes_Member>& mutable_clothes();

    // company
    inline bool has_company() const { return has_bit(kCompanyFieldNumber); }
    void clear_company();
    const mongoodm::ObjectIdValue* company() const;
    mongoodm::ObjectIdValue& mutable_company();
    inline std::string company_value() const { return company_->GetValue().GetValue(); }
    inline void set_company(const std::string &value) { mutable_company().SetValue(value); }

private:
    inline bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    inline void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    inline void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
    inline void set_has__id() { set_has_bit(k_idFieldNumber); }
    inline void clear_has__id() { clear_has_bit(k_idFieldNumber); }
    inline void set_has_userid() { set_has_bit(kUseridFieldNumber); }
    inline void clear_has_userid() { clear_has_bit(kUseridFieldNumber); }
    inline void set_has_username() { set_has_bit(kUsernameFieldNumber); }
    inline void clear_has_username() { clear_has_bit(kUsernameFieldNumber); }
    inline void set_has_name() { set_has_bit(kNameFieldNumber); }
    inline void clear_has_name() { clear_has_bit(kNameFieldNumber); }
    inline void set_has_favorite_color() { set_has_bit(kFavoriteColorFieldNumber); }
    inline void clear_has_favorite_color() { clear_has_bit(kFavoriteColorFieldNumber); }
    inline void set_has_male() { set_has_bit(kMaleFieldNumber); }
    inline void clear_has_male() { clear_has_bit(kMaleFieldNumber); }
    inline void set_has_height() { set_has_bit(kHeightFieldNumber); }
    inline void clear_has_height() { clear_has_bit(kHeightFieldNumber); }
    inline void set_has_age() { set_has_bit(kAgeFieldNumber); }
    inline void clear_has_age() { clear_has_bit(kAgeFieldNumber); }
    inline void set_has_birth_date() { set_has_bit(kBirthDateFieldNumber); }
    inline void clear_has_birth_date() { clear_has_bit(kBirthDateFieldNumber); }
    inline void set_has_retire_date() { set_has_bit(kRetireDateFieldNumber); }
    inline void clear_has_retire_date() { clear_has_bit(kRetireDateFieldNumber); }
    inline void set_has_death_date() { set_has_bit(kDeathDateFieldNumber); }
    inline void clear_has_death_date() { clear_has_bit(kDeathDateFieldNumber); }
    inline void set_has_photo() { set_has_bit(kPhotoFieldNumber); }
    inline void clear_has_photo() { clear_has_bit(kPhotoFieldNumber); }
    inline void set_has_titles() { set_has_bit(kTitlesFieldNumber); }
    inline void clear_has_titles() { clear_has_bit(kTitlesFieldNumber); }
    inline void set_has_clothes() { set_has_bit(kClothesFieldNumber); }
    inline void clear_has_clothes() { clear_has_bit(kClothesFieldNumber); }
    inline void set_has_company() { set_has_bit(kCompanyFieldNumber); }
    inline void clear_has_company() { clear_has_bit(kCompanyFieldNumber); }

private:
    mongoodm::ObjectIdField *_id_;
    mongoodm::UInt64Field *userid_;
    mongoodm::StringField *username_;
    mongoodm::GenericField<Account_name> *name_;
    mongoodm::GenericField<Color> *favorite_color_;
    mongoodm::BoolField *male_;
    mongoodm::DoubleField *height_;
    mongoodm::UInt32Field *age_;
    mongoodm::DateTimeField *birth_date_;
    mongoodm::DateTimeField *retire_date_;
    mongoodm::DateTimeField *death_date_;
    mongoodm::BinaryField *photo_;
    mongoodm::ArrayField<mongoodm::StringValue> *titles_;
    mongoodm::ArrayField<Account_clothes_Member> *clothes_;
    mongoodm::ObjectIdField *company_;

    unsigned long long _has_bits_;
};

}  // namespace entity

#endif  // ENTITY_ACCOUNT_H_
