/*
   Updated code to handle specific airpuff stimulation timings
   Stimulations occur at the absolute times: 35s, 45s, 60.3s, 70.4s, 78s, 88s, 100.7s, 110.8s
*/

// Define the pins
int TTL1 = 2;    // Input from nVoke2: If HIGH, Arduino will start the timing protocol
int TTL2 = 4;    // Output TTL signal to control the airpuff machine

// Define stimulation timings (in milliseconds)
float stim_times[] = {35000, 45000, 60300, 70400, 78000, 88000, 100700, 110800};
int num_stim = sizeof(stim_times) / sizeof(stim_times[0]);

// Variables for tracking the state of the stimulation
unsigned long start_time = 0; // Start time of the stimulation protocol
int stim_index = 0;           // Current stimulation index
bool protocol_running = false;

void setup() 
{
  pinMode(TTL1, INPUT);
  pinMode(TTL2, OUTPUT);
  digitalWrite(TTL2, LOW);    // Initial state of the airpuff machine is OFF
  // Serial.begin(115200);
}

void loop() 
{
  // int trigger_signal = HIGH;  // Automatically start the protocol for testing
  int trigger_signal = digitalRead(TTL1); // Read TTL1 to start the stimulation protocol

  // Start the protocol when TTL1 is HIGH
  if (trigger_signal == HIGH && !protocol_running) 
  {
    protocol_running = true;
    start_time = millis();  // Record the start time
    stim_index = 0;         // Reset the stimulation index
    Serial.println("Stimulation protocol started!");
  }

  if (protocol_running) 
  {
    unsigned long current_time = millis(); // Get the current elapsed time
    
    // Check if the current stimulation index is within bounds
    if (stim_index < num_stim) 
    {
      // Check if it's time to trigger the next stimulation
      if (current_time - start_time >= stim_times[stim_index]) 
      {
        // Trigger airpuff stimulation
        Serial.print("Stimulation ");
        Serial.print(stim_index + 1);
        Serial.print(" triggered at time ");
        Serial.print(stim_times[stim_index] / 1000.0, 3);
        Serial.println(" seconds");

        digitalWrite(TTL2, HIGH);
        // Serial.println("Simulating airpuff ON...");
        delay(100); // Simulate airpuff duration (100 ms)
        digitalWrite(TTL2, LOW);
        // Serial.println("Simulating airpuff OFF...");


        stim_index++; // Move to the next stimulation
      }
    } 
    else 
    {
      // Protocol complete
      Serial.println("Stimulation protocol complete!");
      protocol_running = false; // Stop the protocol
    }
  }
}
