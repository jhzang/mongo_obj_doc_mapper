#ifndef ENTITY_ACCOUNT_CLOTHES_MEMBER_H_
#define ENTITY_ACCOUNT_CLOTHES_MEMBER_H_

#include <mongoodm/mongoodm.h>
#include "color.h"

namespace entity {

class Account_clothes_Member : public mongoodm::Document
{
private:
    enum {
        kNameFieldNumber = 0,
        kColorsFieldNumber = 1,
    };

public:
    Account_clothes_Member();
    virtual ~Account_clothes_Member();

    Account_clothes_Member(const Account_clothes_Member &other) { CopyFrom(other); }
    Account_clothes_Member& operator=(const Account_clothes_Member &other) { CopyFrom(other); return *this; }
    virtual void CopyFrom(const Value &other);
    virtual Value* Clone() const { return new Account_clothes_Member(*this); }
    void Clear();

    virtual int ParseField(const std::string &name, const rapidjson::Value &json_value);

    // name
    inline bool has_name() const { return has_bit(kNameFieldNumber); }
    void clear_name();
    const mongoodm::StringValue* name() const;
    mongoodm::StringValue& mutable_name();
    inline std::string name_value() const { return name_->GetValue().GetValue(); }
    inline void set_name(const std::string &value) { mutable_name().SetValue(value); }

    // colors
    inline bool has_colors() const { return has_bit(kColorsFieldNumber); }
    void clear_colors();
    int colors_size() const;
    const Color* colors(size_t index) const;
    Color* mutable_colors(size_t index);
    Color& add_colors_member();
    bool del_colors_member(size_t index);
    mongoodm::GenericArrayValue<Color>* colors();
    mongoodm::GenericArrayValue<Color>& mutable_colors();

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
    inline void set_has_name() { set_has_bit(kNameFieldNumber); }
    inline void clear_has_name() { clear_has_bit(kNameFieldNumber); }
    inline void set_has_colors() { set_has_bit(kColorsFieldNumber); }
    inline void clear_has_colors() { clear_has_bit(kColorsFieldNumber); }

private:
    mongoodm::StringField *name_;
    mongoodm::ArrayField<Color> *colors_;

    unsigned long long _has_bits_;
};

}  // namespace entity

#endif  // ENTITY_ACCOUNT_CLOTHES_MEMBER_H_
