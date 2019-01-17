//Get FFT, Heading
#define floatorfixed 0
int FFT_dispOn = 0;
int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.
const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256 
                                       // without running out of memory for buffers and other state.
const int AUDIO_INPUT_PIN = 20;        // Input ADC pin for audio data.
const int ANALOG_READ_RESOLUTION = 16; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 2;  // Number of samples to average with each ADC reading.
const int POWER_LED_PIN = 13;          // Output pin for power LED (pin 13 to use Teensy 3.0's onboard LED).

IntervalTimer samplingTimer;
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
//q15_t samples[FFT_SIZE*2];
//q15_t magnitudes[FFT_SIZE];
int sampleCounter = 0;
float window[FFT_SIZE*2];
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void get_FFT() {
  
  int max_bin=0, max_mag=0;
  
  // Set up ADC and audio input.
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);

  // Set up DAC
  analogWriteResolution(10);
  
  // Turn on the power indicator LED.
  //pinMode(POWER_LED_PIN, OUTPUT);
  //digitalWrite(POWER_LED_PIN, HIGH);

  // Create triangle window
  for(int i=0; i<FFT_SIZE/2; i+=2){
    window[i]=(float)i/(FFT_SIZE/2);
  }
  for(int i=FFT_SIZE/2; i<FFT_SIZE; i+=2){
    window[i]=(float)(FFT_SIZE/2-i+FFT_SIZE/2)/(FFT_SIZE/2);
  }
  // Begin sampling audio
  samplingBegin();
  delay(1);
      
    while(sampleCounter<512){Serial.println(sampleCounter);} 
    
    if (samplingIsDone()) {
      arm_cfft_radix4_instance_f32 fft_inst;
      arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
      arm_cfft_radix4_f32(&fft_inst, samples);
      // Calculate magnitude of complex numbers output by the FFT.
      arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
  
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      //analogWrite(A14,1024);
      for(int i = 1; i<(FFT_SIZE/2); i++){
        if(magnitudes[i]>max_mag && i>10  && magnitudes[i]>300) {max_mag=magnitudes[i]; max_bin = i;}
        //Serial.println(magnitudes[0]);
        //Serial.println(samples[i*2]);
        //Serial.println(window[i*2]);
        display.drawLine(i, 63, i, 63-(int)magnitudes[i]/1024, WHITE);
        //analogWrite(A14,magnitudes[i]/64);
      }
      //analogWrite(A14,1024);
      display.print(' '); display.print(max_bin*35);
      display.display();
            
    }
  //}
  ESP_Ser.print(max_mag); ESP_Ser.print(' ');
  ESP_Ser.print(max_bin*35); ESP_Ser.print(' ');
  ESP_Ser.println(getHeading());
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void samplingCallback() {
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
  //samples[sampleCounter] = ((float32_t)(analogRead(AUDIO_INPUT_PIN)-0))*window[sampleCounter];
  // samples[sampleCounter] = analogRead(AUDIO_INPUT_PIN);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter+1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled 
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE*2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}

boolean samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cont_FFT(){
  MySerial.read();
  while(MySerial.available()<1){
    //start turntable
    //
    //get FFT and heading
    get_FFT();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void wsplot_FFT() {
  
  // Set up ADC and audio input.
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);

  // Begin sampling audio
  samplingBegin();
  
  MySerial.read();
  while(MySerial.available()<1){
    
    if (samplingIsDone()) {
      arm_cfft_radix4_instance_f32 fft_inst;
      arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
      arm_cfft_radix4_f32(&fft_inst, samples);
      // Calculate magnitude of complex numbers output by the FFT.
      arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
  
      for(int i = 1; i<(FFT_SIZE/2); i++){
        ESP_Ser.print((int)magnitudes[i]/500); ESP_Ser.print(" ");
        //delay(1);
        //MySerial.print((int)magnitudes[i]/500); MySerial.print(" ");
        }
      ESP_Ser.print('\r');
      //MySerial.println();

      //delay(100);
      samplingBegin();
    }
  }
}
