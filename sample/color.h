#ifndef ENTITY_COLOR_H_
#define ENTITY_COLOR_H_

#include <mongoodm/mongoodm.h>


namespace entity {

class Color : public mongoodm::Document
{
private:
    enum {
        kRedFieldNumber = 0,
        kGreenFieldNumber = 1,
        kBlueFieldNumber = 2,
    };

public:
    Color();
    virtual ~Color();

    Color(const Color &other) { CopyFrom(other); }
    Color& operator=(const Color &other) { CopyFrom(other); return *this; }
    virtual void CopyFrom(const Value &other);
    virtual Value* Clone() const { return new Color(*this); }
    void Clear();

    virtual int ParseField(const std::string &name, const rapidjson::Value &json_value);

    // red
    inline bool has_red() const { return has_bit(kRedFieldNumber); }
    void clear_red();
    const mongoodm::UInt32Value* red() const;
    mongoodm::UInt32Value& mutable_red();
    inline uint32_t red_value() const { return red_->GetValue().GetValue(); }
    inline void set_red(uint32_t value) { mutable_red().SetValue(value); }

    // green
    inline bool has_green() const { return has_bit(kGreenFieldNumber); }
    void clear_green();
    const mongoodm::UInt32Value* green() const;
    mongoodm::UInt32Value& mutable_green();
    inline uint32_t green_value() const { return green_->GetValue().GetValue(); }
    inline void set_green(uint32_t value) { mutable_green().SetValue(value); }

    // blue
    inline bool has_blue() const { return has_bit(kBlueFieldNumber); }
    void clear_blue();
    const mongoodm::UInt32Value* blue() const;
    mongoodm::UInt32Value& mutable_blue();
    inline uint32_t blue_value() const { return blue_->GetValue().GetValue(); }
    inline void set_blue(uint32_t value) { mutable_blue().SetValue(value); }

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
    inline void set_has_red() { set_has_bit(kRedFieldNumber); }
    inline void clear_has_red() { clear_has_bit(kRedFieldNumber); }
    inline void set_has_green() { set_has_bit(kGreenFieldNumber); }
    inline void clear_has_green() { clear_has_bit(kGreenFieldNumber); }
    inline void set_has_blue() { set_has_bit(kBlueFieldNumber); }
    inline void clear_has_blue() { clear_has_bit(kBlueFieldNumber); }

private:
    mongoodm::UInt32Field *red_;
    mongoodm::UInt32Field *green_;
    mongoodm::UInt32Field *blue_;

    unsigned long long _has_bits_;
};

}  // namespace entity

#endif  // ENTITY_COLOR_H_
