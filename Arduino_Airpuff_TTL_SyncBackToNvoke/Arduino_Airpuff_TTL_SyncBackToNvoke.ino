/*
   Modified from "Simple Arduino 4 Channel TTL Closed Loop Generator" written by:

   Optogenetics and Neural Engineering Core ONE Core
   University of Colorado
 

   A TTL input is received by the Arduino, and then a series of signal is sent out on a separate channel to activate airpuff machine.
*/

//This section defines the pins on the Arduino. We specify the intigers (int) TTL1, TTL2, and TTL3.
//If you expanding this to require more than two channels, uncomment (remove "//") below.
int TTL1 = 2;    //Input from nVoke2: If HIGH, Arduino will send varied-duration signal to airpuff machine
int TTL2 = 4;   //The TTL signal to control the airpuff machine will come from here
int TTL3 = 7;   //Input from nVoke2: If HIGH, Arduino will send one signal (con_single) to airpuff machine 
// int TTL3 = 7;       //The IR sensors will get powered from here
//int TTL4 = 8;     //Not used in this example
//int TTL5 = 12;    //Can be set as a TTL
//int TTL6 = 13;    //Can be set as a TTL, though, not recommend, as there are slight differences in this pin

//This section defines variables that the Arduino will use to define the state of the sensor
// int sensorState = LOW, i = 0;
int airpuff_varied = LOW;
int airpuff_single = LOW;

//Now we write the pulse train for the optogenetic stimulation.
//Draw out the desired waves for the stimulation. The condition (or state) of the Arduino changes any time any of the channels change. See the write up for more information.
//The number of conditions is dependent on how many times the Arduino changes. For the given example, the laser is to be pulsed on for 25 ms, off for 25, on for 25, then off for 75. This pulse train is to be delivered twice for a given nose poke.
//Define the number of conditions. Define the length of time (in ms) for each condition. [250 500 750 1000] ms
//Define the number of times to repeat the pulse train

int con1 = 50;
int con2 = 100;
int con3 = 250;
int con4 = 500;
int conWait = 10000; // from the end of one output to the start of the next output

int con_single = con2; // 

// int  con[] = {100, 250, 500, 1000}; // conditions in ms
int  con[] = {con1, con2, con3, con4, con1, con2, con3, con4}; // conditions in ms
int num_con = 8;

// int numrepeat = 2;
// int static_var_on = 1;
// int static_var_off = 0;

// Swap two elements in an array
void swapEntries(int a, int b)
{
  int temp = con[a];
  con[a] = con[b];
  con[b] = temp;
}

// Print the content in 'con'
void printData()
{
  for (const auto& v : con) {
    Serial.print(v);
    Serial.write(' ');
  }
  Serial.println();
}

//This section sets up the Arduino to treat the pins as output or input. With this example, Channel 1 (TTL1) is input (from an IR Beam) and Channel 2 is a TTL ouput (to a laser).
void setup() 
{
  pinMode(TTL1, INPUT);
  pinMode(TTL2, OUTPUT);
  pinMode(TTL3, INPUT);
  // pinMode(TTL3, OUTPUT);
  digitalWrite(TTL1, LOW);   //
  digitalWrite(TTL2, LOW);     //The initial state of the airpuff is off
  digitalWrite(TTL3, LOW);     //
  Serial.begin(115200); 
}

//This is the code that Arduino send TTL signal to airpuff machine. The airpuff will follow the time and duration of the TTL signal
//For each output, the duration of will increase from con1 to con4 and then go back to con1
//Total 8 output with interval (from the end of one output to the start of the next one) of "conWait"

void loop() 
{
  airpuff_varied = digitalRead(TTL1); // Read TTL1, if HIGH, start the stimulation
  airpuff_single = digitalRead(TTL3); // Read TTL1, if HIGH, start the stimulation

  // Apply varied duration airpuff stimulations
  if (airpuff_varied == HIGH)
  {
    printData();
    Serial.print("num_con = ");
    Serial.print(num_con, DEC);
    Serial.println();
    // Shuffle the elements in 'con'
    for (int i = 0; i < num_con; i++) 
    {
      swapEntries(random(0, num_con), random(0, num_con)); // perform swaps
    }
    printData();

    // Start to send stimulation trigger signal via TTL2
    for (int i = 0; i < num_con; i++)
    {
      // Print the stimulation duration to serial port
      Serial.print(i, DEC);
      Serial.print(" = ");
      Serial.print(con[i], DEC);
      Serial.print(" ms");
      Serial.println();

      // Output the stimulation trigger signal
      digitalWrite(TTL2, HIGH);
      delay(con[i]);

      // Print the duration of stimulation interval (rest). 
      // From the end of stimulation i to the start of stimulation i+1
      Serial.print("break: ");
      Serial.print(conWait, DEC);
      Serial.print(" ms");
      Serial.println();

      // Rest before sending the next stimulation trigger to allow neurons to recorver
      digitalWrite(TTL2, LOW);
      delay(conWait);
    }
  }

  // Apply one airpuff. Arduino follows the trigger signal from the nVoke2. one trig=one_airpuff 
  if (airpuff_single == HIGH)
  {
    digitalWrite(TTL2, HIGH);
    delay(con_single);
    digitalWrite(TTL2, LOW);
  }
}