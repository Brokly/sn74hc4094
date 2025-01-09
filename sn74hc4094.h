#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

#include <vector>

namespace esphome {
namespace sn74hc4094 {

class SN74HC4094Component : public Component {
 public:
  SN74HC4094Component() = default;

  void setup() override;
  float get_setup_priority() const override;
  void dump_config() override;

  void check_max_GPIOnum(uint16_t num) { 
	 num++;
	 if(num%8 > 0){ // каждый корпус - 8 выходов
	    num = num/8 + 1;
	 } else {
	    num/=8;
	 }
	 if(sr_count_ < num){ // количество корпусов включенных последовательно 
	    sr_count_ = num;
		this->output_bits_.resize(num * 8);
	 }
  }
  
  void set_data_pin(GPIOPin *pin) { data_pin_ = pin; }
  void set_clock_pin(GPIOPin *pin) { clock_pin_ = pin; }
  void set_stb_pin(GPIOPin *pin) { stb_pin_ = pin; }
  void set_oe_pin(GPIOPin *pin) { oe_pin_ = pin; }

 protected:
  friend class SN74HC4094GPIOPin;
  void digital_write_(uint16_t pin, bool value);
  void write_gpio_();
  std::string strState_();

  GPIOPin *data_pin_{0};
  GPIOPin *clock_pin_{0};
  GPIOPin *stb_pin_{0};
  GPIOPin *oe_pin_{nullptr};
  uint8_t sr_count_{0};
  std::vector<bool> output_bits_;
  bool do_write=false;
  
};

/// Helper class to expose a SC74HC595 pin as an internal output GPIO pin.
class SN74HC4094GPIOPin : public GPIOPin, public Parented<SN74HC4094Component> {
 public:
  void setup() override;
  void pin_mode(gpio::Flags flags) override {}
  bool digital_read() override { return state_; }
  void digital_write(bool value) override;
  std::string dump_summary() const override;
  void set_pin(uint16_t pin) { 
     this->parent_->check_max_GPIOnum(pin);
	 pin_ = pin; 
  }
  uint16_t get_pin_num(){ return pin_; }
  void set_inverted(bool inverted) { 
     inverted_ = inverted; 
     state_ = false;
  }

 protected:
  uint16_t pin_;
  bool inverted_;
  bool state_;
};

}  // namespace sn74hc4094
}  // namespace esphome