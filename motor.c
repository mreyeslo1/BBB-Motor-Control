#include <stdlib.h>  
 #include <stdio.h>  
 #include <string.h>  
 #include <unistd.h>     //close()  
 #include <fcntl.h>     //define O_WONLY and O_RDONLY  

#define MAX_BUF 64     //This is plenty large  
 
int readADC()  
 {  
      int fd;          //file pointer  
      char buf[MAX_BUF];     //file buffer  
      char val[4];     //holds up to 4 digits for ADC value  
      
      //Create the file path by concatenating the ADC pin number to the end of the string  
      //Stores the file path name string into "buf"  
      snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/helper.15/AIN0");     //Concatenate ADC file name  
      
		fd = open(buf, O_RDONLY);      
      //Will trigger if the ADC is not enabled  
      if (fd < 0) {  
           perror("ADC - problem opening ADC");  
      }//end if  
      
      read(fd, &val, 4);     //read ADC ing val (up to 4 digits 0-1799)  
      close(fd);     //close file and stop reading  
      return atoi(val);     //returns an integer value (rather than ascii)  
 }//end read ADC()
 
 //write out to pwm using adc in 
int write_pwm() {
  int fd;
  int duty=10*readADC()*0.05503057254030016675931072818232;
 
  char d = duty;

  fd = open("/sys/devices/ocp.3/pwm_test_P9_21.16/duty", O_WRONLY);
  write (fd, &d, 4);
  
  close(fd);
  return duty;
} 

//button for manual mode 
int readBTN()  
 {  
      int fd;          //file pointer  
      char buf[MAX_BUF];     //file buffer  
      char val[1];     //holds up to1 in put
      

      snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio65/value");     //open value file 
      
      fd = open(buf, O_RDONLY);
      
      read(fd, &val, 1);     //readbtn in 
	  usleep(50000);
      close(fd);     //close file and stop reading  
      return atoi(val);     //returns an integer value (rather than ascii)  
 }

void mode1(){
	
	system("echo 1 > /sys/class/gpio/gpio46/value");
	printf("MODE 1 - FORWARD \n" );

}

void mode2(){
	system("echo 0 > /sys/class/gpio/gpio46/value");
	printf("MODE 2 - REVERSE \n" );
		
	}
void led_out(int adc_in){
	if (adc_in >1350 ) {
		system("echo 1 > /sys/class/gpio/gpio67/value");
		system("echo 1 > /sys/class/gpio/gpio68/value");
		system("echo 1 > /sys/class/gpio/gpio44/value");
		system("echo 1 > /sys/class/gpio/gpio26/value");
	}
	else if (adc_in >899 ) {
		system("echo 1 > /sys/class/gpio/gpio67/value");
		system("echo 1 > /sys/class/gpio/gpio68/value");
		system("echo 1 > /sys/class/gpio/gpio44/value");
		system("echo 0 > /sys/class/gpio/gpio26/value");
	}
	else if (adc_in >449 ) {
		system("echo 1 > /sys/class/gpio/gpio67/value");
		system("echo 1 > /sys/class/gpio/gpio68/value");
		system("echo 0 > /sys/class/gpio/gpio44/value");
		system("echo 0 > /sys/class/gpio/gpio26/value");
	}
	
	 else if (adc_in >0 ) {
		system("echo 1 > /sys/class/gpio/gpio67/value");
		system("echo 0 > /sys/class/gpio/gpio68/value");
		system("echo 0 > /sys/class/gpio/gpio44/value");
		system("echo 0 > /sys/class/gpio/gpio26/value");
	}
		
	}
int main(){
    
    system("echo BB-ADC > /sys/devices/bone_capemgr.9/slots");
    system("echo am33xx_pwm > $SLOTS");
    system("echo bone_pwm_P9_16 > $SLOTS");
    system("echo bone_pwm_P9_42 > $SLOTS");
	//gpio led setup
	system("echo 1000  > /sys/devices/ocp.3/pwm_test_P9_16.17/period");
	system("echo 250  > /sys/devices/ocp.3/pwm_test_P9_16.17/duty");
	system("echo 1  > /sys/devices/ocp.3/pwm_test_P9_16.17/run");
	
	system("echo 1000  > /sys/devices/ocp.3/pwm_test_P9_42.19/period");
	system("echo 250  > /sys/devices/ocp.3/pwm_test_P9_42.19/duty");
	system("echo 1  > /sys/devices/ocp.3/pwm_test_P9_42.19/run");
	
	system("echo 67 > /sys/class/gpio/export");
	system("echo 68 > /sys/class/gpio/export");
	system("echo 44 > /sys/class/gpio/export");
	system("echo 26 > /sys/class/gpio/export");
	//hbridge output
	system("echo 46 > /sys/class/gpio/export");
    //btn init
    system("echo 65 > /sys/class/gpio/export");
    
    
	//cat direction
	system("echo out > /sys/class/gpio/gpio67/direction");
	system("echo out > /sys/class/gpio/gpio68/direction");
	system("echo out > /sys/class/gpio/gpio44/direction");
	system("echo out > /sys/class/gpio/gpio26/direction");
	//hbridge output

	system("echo out > /sys/class/gpio/gpio46/direction");
	//btn
	system("echo out > /sys/class/gpio/gpio65/direction");


	while(1){
	  //  while (readBTN() != 0);
	 //   sleep(1);
	 	int adc_in= readADC();
	 	printf("adc_ in = %d \n", adc_in );

	    mode1();
	    led_out(adc_in);
	    printf("pwm_ out = %d \n", write_pwm() );
	  //  while (readBTN() != 0);
	   // sleep(1);
	  //  mode2();
	    
        


	}
}


//http://www.techcrashcourse.com/2014/10/c-program-to-print-current-date-time.html
//http://dumb-looks-free.blogspot.com/2014/06/beaglebone-black-bbb-send-mail-via-gmail.html
//http://elinux.org/Beagleboard:BMP_on_the_Beagle_Bone_Black
//http://www.thebrokendesk.com/post/sending-tweets-from-the-beaglebone-black/
