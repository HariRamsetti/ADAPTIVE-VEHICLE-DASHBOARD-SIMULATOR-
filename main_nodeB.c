//main nodeB 
 
#include "header1.h" 
 
CAN1 r1; 
volatile u32 flag=0; 
 
int main(){ 
    float temp,speed,fuel; 
    can1_init(); 
    uart0_init(9600); 
	  config_vic_for_can1();
    IODIR0|=(1<<19)|(1<<20)|(1<<21); 
    IOSET0=(1<<19)|(1<<20)|(1<<21); 
 
    while(1){ 
        if(flag){ 
            flag=0; 
            // byteA 
            temp=adc_to_temp(r1.byteA&0x3FF); 
            speed=adc_to_speed((r1.byteA>>10)&0x3FF); 
            fuel=adc_to_fuel((r1.byteA>>20)&0x3FF); 
 
            // byteB 
            left_indi=(r1.byteB>>19)&1; 
            right_indi=(r1.byteB>>20)&1; 
            head_light=(r1.byteB>>21)&1; 
 
//uart0_float(left_indi); 
//uart0_float(right_indi); 
//uart0_float(head_light); 
            // temp 
            uart0_tx_string("TEMPERATURE: "); 
            uart0_float(temp); 
            uart0_tx_string("C\r\n"); 
 
            // speed 
            uart0_tx_string("SPEED: "); 
            uart0_float(speed); 
            uart0_tx_string("km/h\r\n"); 
 
            // fuel 
            uart0_tx_string("FUEL LEVEL: "); 
            uart0_float(fuel); 
            uart0_tx_string("L\r\n"); 
 
            // left indi 
            if(left_indi) 
                uart0_tx_string("<-"); 
            else 
                uart0_tx_string("  "); 
 
            // head light 
            if(head_light) 
                uart0_tx_string(" . "); 
            else 
                uart0_tx_string("   "); 
 
            // right indi 
            if(right_indi) 
                uart0_tx_string("->"); 
            else 
                uart0_tx_string("  "); 
 
                                                uart0_tx_string("\r\n"); 
        } 
 
                                // headlight control 
        if(head_light) 
            IOCLR0=1<<21; 
        else 
            IOSET0=1<<21; 
 
        // indi control 
        if(left_indi){ 
            IOCLR0=1<<19; 
            delay_ms(100); 
            IOSET0=1<<19; 
            delay_ms(100); 
 
        } 
        else if(right_indi){ 
            IOCLR0=1<<20; 
            delay_ms(100); 
            IOSET0=1<<20; 
            delay_ms(100); 
        } 
 
        //      can1_rx(&r1); 
    } 
}