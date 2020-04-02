/*

*/

char incomingByte = 0;
String buffer = "";
bool b_data_available = false;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  pinMode(2, INPUT_PULLUP);
}

void loop() {
 
    
  if (Serial.available() > 0) 
  {
    while(Serial.available()>0)
    {
      // read the incoming byte:
      incomingByte = Serial.read();
      
      // say what you got:
      buffer.concat(incomingByte);
    }
    
    Serial.println(buffer);
    
    if(buffer=="~Type")
    {
      Serial.println("Arduino 2");
    }
    else if(buffer=="~Status")
    {
      
      int pot1 = analogRead(A0);
      int pot2 = analogRead(A1);
      int button = digitalRead(2);
      
      String outputLine = "";
      
      if(button==HIGH)
        outputLine.concat(".B0");
      else
        outputLine.concat(".B1");
      
      outputLine.concat(".T");
      outputLine.concat(pot1);
      outputLine.concat(".S");
      outputLine.concat(pot2);
      
      Serial.println(outputLine);
    }
    else
    {
      Serial.println("Unrecognized command");
    }
    
    buffer = "";
  }
}
