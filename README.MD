# Arduino Alzheimers Phone

A project for a friend. This modifies a Cordtelco 2500 model telephone so that it 
effectively only works as a speed dial however has a 911 override. Its designed with 
alzheimers in mind so that if the person continues to try and dial the phone only 
the first number matters fixing what is seen as a defect in current phones you can buy for alzheimers.

I have included the PCB in kicad I used to make this phone, Also the arduino code. The board uses a
at328p microprocessor with an 8mhz crystal as the voltage on a phone line can vary wildly and 5v cannot
be assured. It programs as if its an arduino pro mini