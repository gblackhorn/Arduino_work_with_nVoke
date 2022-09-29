/*
   Modified from "Simple Arduino 4 Channel TTL Closed Loop Generator" written by:

   Optogenetics and Neural Engineering Core ONE Core
   University of Colorado
 

   A TTL input is received by the Arduino, and then a series of signal is sent out on a separate channel to activate airpuff machine.
*/

//This section defines the pins on the Arduino. We specify the intigers (int) TTL1, TTL2, and TTL3.
//If you expanding this to require more than two channels, uncomment (remove "//") below.
int TTL1 = 2;       //The input from the nVoke2 comes in here
int TTL2 = 4;       //The TTL signal to control the airpuff machine will come from here
// int TTL3 = 7;       //The IR sensors will get powered from here
//int TTL4 = 8;     //Not used in this example
//int TTL5 = 12;    //Can be set as a TTL
//int TTL6 = 13;    //Can be set as a TTL, though, not recommend, as there are slight differences in this pin

//This section defines variables that the Arduino will use to define the state of the sensor
// int sensorState = LOW, i = 0;
int nVoke_GPO = LOW;

//Now we write the pulse train for the optogenetic stimulation.
//Draw out the desired waves for the stimulation. The condition (or state) of the Arduino changes any time any of the channels change. See the write up for more information.
//The number of conditions is dependent on how many times the Arduino changes. For the given example, the laser is to be pulsed on for 25 ms, off for 25, on for 25, then off for 75. This pulse train is to be delivered twice for a given nose poke.
//Define the number of conditions. Define the length of time (in ms) for each condition. [250 500 750 1000] ms
//Define the number of times to repeat the pulse train

int con1 = 100;
int con2 = 250;
int con3 = 500;
int con4 = 1000;
int conWait = 10000; // from the end of one output to the start of the next output

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
  // pinMode(TTL3, OUTPUT);
  digitalWrite(TTL1, LOW);   //This defines the original state of the sensor (when it is open and detecting IR light) as high/on
  digitalWrite(TTL2,LOW);     //The initial state of the optogenetic stim should be off
  // digitalWrite(TTL3, LOW);   //This turns on TTL3. We do not change it in the rest of the code so that we can use this to power the IR light and sensor
  // nVoke_GPO = digitalRead (TTL1);
  Serial.begin(115200); 
}

//This is the code that Arduino send TTL signal to airpuff machine. The airpuff will follow the time and duration of the TTL signal
//For each output, the duration of will increase from con1 to con4 and then go back to con1
//Total 8 output with interval (from the end of one output to the start of the next one) of "conWait"

void loop() 
{
  nVoke_GPO = digitalRead(TTL1); // Read TTL1, if HIGH, start the stimulation
  if (nVoke_GPO == HIGH)
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
}



// void loop()
// {
//   nVoke_GPO = digitalRead (TTL1);
//   if  (nVoke_GPO == HIGH)
//   {
//     //1st airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // Serial.print("Variable 1:");
//     // Serial.print(static_var_on);
//     // Serial.println("airpuff on");
//     // digitalWrite(TTL1, HIGH);
//     delay(con1);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // Serial.print("Variable 1:");
//     // Serial.print(static_var_off);
//     // Serial.println("airpuff off");
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //2nd airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // Serial.println("airpuff on");
//     // digitalWrite(TTL1, HIGH);
//     delay(con2);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // Serial.println("airpuff off");
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //3rd airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // digitalWrite(TTL1, HIGH);
//     delay(con3);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //4th airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // digitalWrite(TTL1, HIGH);
//     delay(con4);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //5th airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // digitalWrite(TTL1, HIGH);
//     delay(con4);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //6th airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // digitalWrite(TTL1, HIGH);
//     delay(con3);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //7th airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // digitalWrite(TTL1, HIGH);
//     delay(con2);

//     //Interval time to allow neurons to recover
//     digitalWrite(TTL2, LOW);
//     // digitalWrite(TTL1, LOW);
//     delay(conWait);

//     //8th airpuff stimulation
//     digitalWrite(TTL2, HIGH);
//     // digitalWrite(TTL1, HIGH);
//     delay(con1);

//     //End the stimulation TTL signal
//     digitalWrite(TTL2, LOW);
//   }
// }