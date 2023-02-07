#include <Tsunami.h>            // Include the Tsunami library header

Tsunami tsunami;                // Our Tsunami object

int numberOfSwitches = 16;

int  gSeqState = 0;             // Main program sequencer state
int  gNumTracks;                // Number of tracks on SD card

char gTsunamiVersion[VERSION_STRING_LEN];    // Tsunami version string


// ****************************************************************************
void setup() {
  
  // Serial monitor
  Serial.begin(9600);

  // We should wait for the Tsunami to finish reset before trying to send
  // commands.
  delay(1000);

  // Tsunami startup at 57600
  tsunami.start();
  delay(10);
  
  // Send a stop-all command and reset the sample-rate offset, in case we have
  //  reset while the Tsunami was already playing.
  tsunami.stopAllTracks();
  tsunami.samplerateOffset(0, 0);
  
  // Enable track reporting from the Tsunami
  tsunami.setReporting(true);
  
  // Allow time for the Tsunami to respond with the version string and
  //  number of tracks.
  delay(100); 

  
}


void loop() {

    tsunami.update();
    
    switch (gSeqState) {
  
    // State 0: Demonstrates how to fade in a music track 
    case 0:
        // First retrieve and print the version and number of tracks        
        if (tsunami.getVersion(gTsunamiVersion, VERSION_STRING_LEN)) {
            Serial.print(gTsunamiVersion);
            Serial.print("\n");
            gNumTracks = tsunami.getNumTracks();
            Serial.print("Number of tracks = ");
            Serial.print(gNumTracks);
            Serial.print("\n");
        }
        else{
            Serial.print("WAV Trigger response not available");
        }
            
        
        for(byte i = 1; i <= numberOfSwitches; i++){
            Serial.println(i);
            tsunami.trackLoad(i, 0, true);
            tsunami.trackLoop(i, true);
            tsunami.trackGain(i, -10);
        }
        tsunami.resumeAllInSync();
  
        tsunami.masterGain(0, 0);              // Reset the master gain to 0dB
                
        gSeqState = 1;                         // Advance to state 1
    break;

     } // switch

}