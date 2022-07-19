#include <mega32.h>
#include <io.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>

// Declare your global variables here 
unsigned int adcout; // menggunakan unsigned supaya tidak bisa memasukan nilai negatif
char temp[16]; //deklarasi variabel temp menggunakan 

// Referensi Tegangan dari AVCC pin
#define ADC_VREF_TYPE 0x00

// Membaca hasil konversi AD
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}
void main(void)
{
    //Tampilan Pertama
    //adc_init();
    lcd_init(16);  //menentukan ukuran lcd dengan ukuran 16x2
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_putsf("FINAL PROYEK");
    lcd_gotoxy(0,1);
    lcd_putsf("Kelompok 3");
    delay_ms(200);


    // Inisialisasi Port D Untuk LED 
    DDRD = 0b11111110;
    
    PORTD = 0b00000001;
    
    // Inisialisasi Port B mengatur driver Motor DC
    // Fungsi: Bit7-In Bit6-In Bits-In Bit4-In Bit3-Out Bit2-In Bitl-Out Bite-Out
    DDRB=(0<<DDB7)|(0<<DDB6)|(0<<DDB5)|(0<<DDB4)|(1<<DDB3)|(0<<DDB2)|(1<<DDB1)|(1<<DDB0);
     
    // Kondisi: Bit7=1 Bit6=T Bit5=T Bit4=7 Bit3=0 Bit2=T Bitl=0 Bite=1
    PORTB=(0<<PORTB7)|(0<<PORTB6)|(0<<PORTB5)|(0<<PORTB4)|(0<<PORTB3)|(0<<PORTB2)|(0<<PORTB1)|(1<<PORTB0);
          
    // Digunakan untuk mengatur kecepatan Motor-DC (PWM)
    TCCR0=(1<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
    TCNT0=0X00;
    OCR0=0X00;  
    
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 15,625 kHz
    // Mode: Fast PWM top-0xFF
    // OC0 output: Non-Inverted PWM
    // Timer Period: 16,384 ms
    // Output Pulse(s):
    // OC0 Period: 16,384 ms Width: 0 us  
    // ADC initialization
    // ADC Clock frequency: 250,000 kHz
    // ADC Voltage Reference: AVCC pin 
    // ADC Auto Trigger Source: ADC Stopped
    //Menentukan tegangan referensi ADC
    ADMUX=ADC_VREF_TYPE;
    //Merupakan bit penanda mulainya konversi ADC.
    ADCSRA=(1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
    // Pengatur sumber konversi ADC,
    SFIOR=(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); 
    
        //mulai
        while (1)
        {   if (PIND.0 == 0){ //jika saklar dihubungkan maka
            // Place your code here  
            lcd_gotoxy(0,0);
            lcd_puts("Suhu : \nSpeed: ");
            lcd_gotoxy(7,0);   // 
            adcout=read_adc(0)/2-1; //untuk pembacaan hasil dari adc pada PA.0
            sprintf(temp, "%03i%cC", adcout, 0xdf);
            lcd_puts(temp);
            //jika suhu lebih dari 35 LED Merah Menyala
            if(adcout>35)
            {

                lcd_gotoxy(7,1); 
                lcd_puts("4     ");
                OCR0=255;
                PORTD = 0b00010001;
            } 
            // jika suhu lebih dari 30 LED Kuning Menyala
            else if(adcout>30)
            {
                lcd_gotoxy(7,1);
                lcd_puts("3     ");
                //PORTC=(1<<PINC0);
                OCR0=189;
                PORTD = 0b00001001;

            }
              // jika suhu lebih dari 25 LED Hijau Menyala
            else if(adcout>25)
            {
                lcd_gotoxy(7,1);
                lcd_puts("2     ");
                OCR0=124;
                PORTD = 0b00000101;
            }
            // jika suhu lebih dari 20 LED Biru Menyala
            else if(adcout>20)
            {
                lcd_gotoxy(7,1); 
                lcd_puts("1     "); 
                OCR0=63;
                PORTD = 0b00000011;
            }    
            //jika tidak maka Matikan Kipas dan LED 
            else 
            {
                lcd_gotoxy(7,1); 
                lcd_puts("0     ");
                OCR0=0x00;
                PORTD = 0b00000001;
            } 
        } else{ //jika saklar tidak dihubungkan
        lcd_gotoxy(0,0);
        lcd_puts(" KIPAS MATI   ");
        lcd_gotoxy(0,1);
        lcd_puts("            ");
        OCR0=0x00;
        PORTD = 0b00000001;
        }
      }   
}            