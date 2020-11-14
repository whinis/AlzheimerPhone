
#include <Keypad.h>



#define ROW_NUM  4        //four rows
#define COLUMN_NUM  3       //three columns
#define NUMBER_SIZE 15

//Number dialed whenever corresponding key pad pressed

//To change which numbers are called just input them here
String dialer_mapping[12] = {
  {""}, // 0 
  {""}, // 1
  {""}, // 2
  {""}, // 3
  {""}, // 4
  {""}, // 5
  {""}, // 6
  {""}, // 7
  {""}, // 8
  {""}, // 9
  {""}, // *
  {""}  // #
};


char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};



//Each number is Row A0 Row A1 Column A0 Column A1
int dialer[12][4] = {
  {1,1,1,0}, // 0 
  {0,0,0,0}, // 1
  {0,0,1,0}, // 2
  {0,0,0,1}, // 3
  {1,0,0,0}, // 4
  {1,0,1,0}, // 5
  {1,0,0,1}, // 6
  {0,1,0,0}, // 7
  {0,1,1,0}, // 8
  {0,1,0,1}, // 9
  {1,1,0,0}, // *
  {1,1,0,1}  // #
};


#define DIALER_EN A0

byte pin_rows[ROW_NUM] = {19, 7, 9, 10}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {12, 11, 8}; //connect to the column pinouts of the keypad

byte mux_control_pins[4] = {A1, A2, A3, A4}; //connect to the row pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void setup(){
  Serial.begin(9600);
  for(int j = 0;j<4;j++){ //setup the pins for the muxing chips
    pinMode(mux_control_pins[j],OUTPUT);
  }
  pinMode(DIALER_EN,OUTPUT);
  Serial.println("Setup Complete");
}
String extract_number(int position){ //extract the number from the dialing map
  return dialer_mapping[position];
}

String emergency="";
int timer = 0;
bool dialed = 0;
bool key_pressed = 0;

void dial(String number){
  for(int i=0;i<number.length();i++){
    int k = number.charAt(i)-48; //get the correct index
    if(k < 0) //skip if too high or low
      continue;
    if(k > 11)
      continue;
    Serial.println(k);
    for(int j = 0;j<4;j++){
      digitalWrite(mux_control_pins[j],dialer[k][j]); //setup mux to dial this number
    }
    digitalWrite(DIALER_EN,1); //dial
    delay(50);                //pause to simulate button press
    digitalWrite(DIALER_EN,0); //disable muxes
    delay(150);                //pause to allow time for next button press
  }
}

void loop(){
  char tempNumber[15];
  String number_to_dial ="";
  if(timer > 0){ //timer for emergency number, only used if 9 is the first key pressed
    timer --;
    if(timer ==0 and !emergency.equals("911")){ //if timer reaches 0 and we never reached 911 then just dial 9
      Serial.println("No Emergency Dialing 9");
      emergency="";
      number_to_dial = extract_number(9);
    }
      
  }
  char key = keypad.getKey();
  if (key){
    Serial.println(key);
    if(key == 42) //If * set to 48+10 so it ends up being 10 in the end
      key = 48+10;
    else if(key == 35) //If # set to 48+11 so it ends up being 11 in the end
      key = 48+11; 
    key = key-48; //remove the 48 from standard ascii table
    if(key == 9 && !key_pressed){ //if number is 9 we might be trying to call 911
      Serial.print("Possible Emergency ");
      Serial.println(key);
      timer = 50;
      emergency+="9";
    }else if(key == 1 && emergency.length() > 0){ //if there is any number in emergency and 1 key was pressed continue dialing
      Serial.print("Possible Emergency ");
      Serial.println(key);
      emergency+="1";
      if(emergency.length() < 3){
        timer = 50;
      }else{
        timer = 0;
        number_to_dial = emergency;
        Serial.println("EMERGENCY DIALING 911");
        emergency="";
      }
    }else{
      Serial.print("Straight Dial ");
      Serial.println(key);
      delay(10);
      Serial.println(extract_number(key));
      number_to_dial = extract_number(key);
    }
    key_pressed = 1;
  }
  if(number_to_dial.length() > 0 && !dialed){ //if there a number to dial then dial it
    Serial.println(number_to_dial);
    dial(number_to_dial);
    number_to_dial = "";
    dialed = 1;
  }
  delay(50);
}
