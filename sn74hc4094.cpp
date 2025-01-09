#include "sn74hc4094.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sn74hc4094 {

static const char *const TAG = "sn74hc4094";

void SN74HC4094Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SN74HC4094...");

  if (this->oe_pin_!=nullptr) {  // disable output
    this->oe_pin_->setup();
    this->oe_pin_->digital_write(false);
  }

  // initialize output pins
  this->clock_pin_->setup();
  this->data_pin_->setup();
  this->clock_pin_->digital_write(false);
  this->data_pin_->digital_write(false);
  this->stb_pin_->setup();
  this->stb_pin_->digital_write(true);

  // send state to shift register
  this->do_write=true;
  this->write_gpio_();

}

void SN74HC4094Component::dump_config() { ESP_LOGCONFIG(TAG, "SN74HC4094 (device(s) %d):", this->sr_count_); }

void SN74HC4094Component::digital_write_(uint16_t pin, bool value) {
  if (pin >= this->sr_count_ * 8) {
    ESP_LOGE(TAG, "Pin %u is out of range! Maximum pin number with %u chips in series is %u", pin, this->sr_count_,
             (this->sr_count_ * 8) - 1);
    return;
  }
  this->output_bits_[pin] = value;
  if(this->do_write) this->write_gpio_();
}

std::string SN74HC4094Component::strState_() {
  std::string buffer="";
  for (auto bit = this->output_bits_.rbegin(); bit != this->output_bits_.rend(); bit++) {
     if(*bit){
         buffer+='1';
     } else {
         buffer+='0';
     }
  }
  return buffer;
}

void SN74HC4094Component::write_gpio_() {
  for (auto bit = this->output_bits_.rbegin(); bit != this->output_bits_.rend(); bit++) {
     this->clock_pin_->digital_write(false);
     this->data_pin_->digital_write(*bit);
     this->clock_pin_->digital_write(true);
  }
  // pulse strobe to activate new values
  this->stb_pin_->digital_write(false);
  this->stb_pin_->digital_write(true);
  // enable output if configured
  if (this->oe_pin_!=nullptr) {
    this->oe_pin_->digital_write(true);
  }
  ESP_LOGD(TAG,"State: %s",strState_().c_str());
}

float SN74HC4094Component::get_setup_priority() const {
    return setup_priority::HARDWARE; 
}

void SN74HC4094GPIOPin::digital_write(bool value) {
  this->parent_->digital_write_(this->pin_, value != this->inverted_);
  this->state_ = value;
}
std::string SN74HC4094GPIOPin::dump_summary() const {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%u via SN74HC4094", pin_);
  return buffer;
}
void SN74HC4094GPIOPin::setup() {
  this->parent_->digital_write_(this->pin_, this->state_);
}



}  // namespace sn74hc4094
}  // namespace esphome