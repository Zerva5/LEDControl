int r,g,b;


const byte numChars = 33;
char receivedChars[numChars];

bool newCommand = false;

//<1>

struct colorid{
  //char
  int id;
  
  int r;
  int g;
  int b;
}colorID_d = {0, 0, 0, 0};

typedef struct colorid colorID;

struct item{
  colorID c1;
  colorID c2;
  colorID c3;
  //num
  int id;

  int t;
}item_d = {colorID_d, colorID_d, colorID_d, 0, 0};

typedef struct item Item;


Item testItem = item_d;

void setup() {
    Serial.begin(9600);
    Serial.println("<Ready For Command>");

    
}

void loop() {
    getCommand();
    if(newCommand == true){
      parseCommand(receivedChars, &testItem);
    }


    
}

//<11rFF>
// Change item 1, color 1, value r, to hex value FF

//<1tFFF>
//change item 1, time value, to hex value FFF

//Can use max FF for color (255)
//max FFF for time (4095)

void parseCommand(char commandRaw[numChars], item * changeItem){


  int id = commandRaw[0] - '0';
  
  char rawValue[3];
  
  int value = 0;

// If the second char is not T that means we are talking about a color
  if(commandRaw[1] != 't'){
    rawValue[0] = commandRaw[3];
    rawValue[1] = commandRaw[4];    
    rawValue[2] = '\0';
  }else{
    rawValue[0] = commandRaw[2];
    rawValue[1] = commandRaw[3];
    rawValue[2] = commandRaw[4];
    rawValue[3] = '\0';
  }

  value = (int)strtol(rawValue, NULL, 16);

//Switch statements to change color values, easier if they were in array.
  if(commandRaw[1] != 't'){
    switch(commandRaw[1]){
      
      case '1':
        switch(commandRaw[2]){
          case 'r':
            changeItem->c1.r = value;
            break;
          case 'g':
            changeItem->c1.g = value;
          case 'b':
            changeItem->c1.b = value;
        }
        
      case '2':
        switch(commandRaw[2]){
          case 'r':
            changeItem->c2.r = value;
            break;
          case 'g':
            changeItem->c2.g = value;
          case 'b':
            changeItem->c2.b = value;
        }
      case '3':
        switch(commandRaw[3]){
          case 'r':
            changeItem->c3.r = value;
            break;
          case 'g':
            changeItem->c3.g = value;
          case 'b':
            changeItem->c3.b = value;
        }
        
    }
  }else{
    changeItem->t = value;
  }

  Serial.println(changeItem->t);
  Serial.println(testItem.t);
  
  newCommand = false;
}

void getCommand() {
    static boolean recvInProgress = false;
    static byte recvIndex = 0;
    char startMark = '<';
    char endMark = '>';
    char recv;
 
    while (Serial.available() > 0 && newCommand == false) {
        recv = Serial.read();

        if (recvInProgress == true) {
            if (recv != endMark) {
              
                receivedChars[recvIndex] = recv;
                recvIndex++;

                //May have to bump up numChars
                if (recvIndex >= numChars) {
                    recvIndex = numChars - 1;
                }
                
            }
            else {
                receivedChars[recvIndex] = '\0'; // terminate the string
                
                recvInProgress = false;
                recvIndex = 0;
                newCommand = true;
            }
        }

        else if (recv == startMark) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newCommand == true) {
        Serial.println(receivedChars);
        newCommand = false;
    }
}
