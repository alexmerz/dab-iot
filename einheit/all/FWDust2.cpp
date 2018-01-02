#ifndef FWDUST2_cpp
#define FWDUST2_cpp

#include "FWDust2.h"


typedef struct _Exinterrupts_Struct
{
  VM_DCL_HANDLE handle;
  uint32_t pin;
       uint32_t eint;
       uint32_t first;
  voidFuncPtr cb;
} Exinterrupts_Struct;

static Exinterrupts_Struct gExinterruptsPio[EXTERNAL_NUM_INTERRUPTS] = {
    {VM_DCL_HANDLE_INVALID, 2, 20, 0, NULL}, // Pin D2
    {VM_DCL_HANDLE_INVALID, 3, 11, 0, NULL}  // Pin D3
};

volatile int __fwdust2_state = 0;
unsigned int __fwdust2_pin = 1; // = Pin D3

#ifdef __cplusplus
extern "C" {
#endif

static void onLow(void* parameter, VM_DCL_EVENT event, VM_DCL_HANDLE device_handle) {
  if(gExinterruptsPio[__fwdust2_pin].handle == device_handle)
    __fwdust2_state = !__fwdust2_state;  
}

#ifdef __cplusplus
}
#endif

FWDust2::FWDust2():
Framework(30000)
{
  lowpulseoccupancy = 0;
  old_state = 0;
  sampletime_ms = 30000;    
}

String FWDust2::getData() 
{
  String data = "\"dust\":\"";
  // Erst Messwerte ausgeben, wenn sich das Gerät beruhigt hat
  if(millis() > wait_before_measurement && concentration > 0.63 && concentration < 1114000L) {
    data += concentration;  
  } 
  // ansonsten gib diesen Wert aus
  else {
    data += "0"; 
  }
  data += "\"";
  return data;
}

void FWDust2::check() 
{
  if(__fwdust2_state != old_state) { // Statusaenderungen durch Interrupt immer testen
    switchEINT(__fwdust2_pin, __fwdust2_state); // Polarität der Erkennung aendern
    if(__fwdust2_state == 1) { // Signal jetzt LOW
        eint_starttime = micros();
    } else if(__fwdust2_state == 0) { // Signal wieder HIGH
      lowpulseoccupancy = lowpulseoccupancy + (micros() - eint_starttime);
      eint_starttime = 0;    
    }
    old_state = __fwdust2_state;     
  } 
  if((millis() - sensor_starttime) >= sampletime_ms) {
    // Serial.println(1.1*pow(1,3)-3.8*pow(1,2)+520.0+0.62); // pow Werte mit 100 multiplizieren, max Wert: 1114000.62
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio, 3)-3.8*pow(ratio,2)+520.0*ratio+0.62;
    lowpulseoccupancy = 0;
    sensor_starttime = millis();  
    _callback(*this);      
  }  
}

void FWDust2::init()
{
  enableEINT(__fwdust2_pin);
  wait_before_measurement = millis() + 30000L; // Warte 30 Sekunden bis sich das Geraet entscheidet anstaendige Werte auszuspucken
  sensor_starttime = millis();
  eint_starttime = micros(); // Achtung: Mikrosekunden fuer Interrupts!  
}

void FWDust2::switchEINT(int pin, int state) 
{
  VM_DCL_HANDLE eint_handle;
  changePinType(gExinterruptsPio[pin].pin, PIO_EINT, &eint_handle);
  vm_eint_ctrl_config_t eint_config;  
  memset(&eint_config,0, sizeof(vm_eint_ctrl_config_t));
  eint_config.act_polarity = __fwdust2_state;
  eint_config.auto_unmask = 1;    
  eint_config.debounce_en = 1;        
  vm_dcl_control(eint_handle ,VM_EINT_CMD_MASK,NULL);                                            
  vm_dcl_control(eint_handle ,VM_EINT_CMD_CONFIG,(void *)&eint_config);   
}

void FWDust2::enableEINT(int pin) {
  VM_DCL_HANDLE eint_handle;
  vm_eint_ctrl_config_t eint_config;
  vm_eint_ctrl_set_sensitivity_t sens_data;
  vm_eint_ctrl_set_hw_deounce_t deboun_time;

  detachInterrupt(pin);
  changePinType(gExinterruptsPio[pin].pin, PIO_EINT, &eint_handle);
  memset(&eint_config,0, sizeof(vm_eint_ctrl_config_t));
  memset(&sens_data,0, sizeof(vm_eint_ctrl_set_sensitivity_t));
  memset(&deboun_time,0, sizeof(vm_eint_ctrl_set_hw_deounce_t));
  eint_handle = vm_dcl_open(VM_DCL_EINT,gExinterruptsPio[pin].eint);
  setPinHandle(gExinterruptsPio[pin].pin, eint_handle);
  gExinterruptsPio[pin].handle = eint_handle;
  vm_dcl_control(eint_handle ,VM_EINT_CMD_MASK,NULL);
  vm_dcl_registercallback(eint_handle , VM_EVENT_EINT_TRIGGER,(VM_DCL_CALLBACK)onLow,(void*)NULL );
  sens_data.sensitivity = 0;
  eint_config.act_polarity = __fwdust2_state;
  eint_config.auto_unmask = 1;    
  vm_dcl_control(eint_handle ,VM_EINT_CMD_SET_SENSITIVITY,(void *)&sens_data); 
  deboun_time.debounce_time = 1;  
  vm_dcl_control(eint_handle ,VM_EINT_CMD_SET_HW_DEBOUNCE,(void *)&deboun_time);
  vm_dcl_control(eint_handle ,VM_EINT_CMD_MASK,NULL);                                        
  eint_config.debounce_en = 1;    
  vm_dcl_control(eint_handle ,VM_EINT_CMD_CONFIG,(void *)&eint_config);   
}


const char* FWDust2::getType()
{
  return FWDUST2TYPE;
}

#endif

