#define NC3 7645.25
#define NF3 5727.37
#define NC4 3822.62
#define NB3 4050.22
#define NA3 4545.45
#define NG3 5102.04
#define NF4 2863.68
#define ND3 6811.98
#define buzzer 21
#define LED 22
#define button 32

hw_timer_t *My_timer = NULL;
int spd = 0;
bool t = true;
int Starwars_note[] = {
NC3, NC3, NC3, NF3, NC4, NB3, NA3, NG3, NF4, NC4, 
NB3, NA3, NG3, NF4, NC4, NB3, NA3, NB3, NG3, NC3, 
NC3, NC3, NF3, NC4, NB3, NA3, NG3, NF4, NC4, NB3, 
NA3, NG3, NF4, NC4, NB3, NA3, NB3, NG3, NC3, NC3, 
ND3, NC4, NB3, NA3, NG3, NF3, NF3, NG3, NA3, NG3
};
int Starwars_duration[] = {
8,8,16,2,2,8,16,8,2,4,8,16,
8,2,4,8,16,8,2,8,8,16,2,2,8,
16,8,2,4,8,16,8,2,4,8,16,8,2,
4,16,2,8,8,8,8,8,16,8,16,4
};

// This is the Timer ISR. It is automatically called 
// on a timer alarm event.
void IRAM_ATTR onTimer(){
  digitalWrite(buzzer, !digitalRead(buzzer));
  
}
void IRAM_ATTR Speed()
{
  digitalWrite(LED, !digitalRead(LED));
  if(t){
  spd++;
  if(spd>5){spd=0;}
  t=false;    
  }
}

void setup() {
  pinMode(button,INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LED,OUTPUT);
  attachInterrupt(button, Speed, RISING);
  My_timer = timerBegin(0, 80, true);  // use tiemr 0 and set prescale to 80 so 1 tick is 1 uSec
  timerAttachInterrupt(My_timer, &onTimer, true);
}

void loop() {
for (int thisNote = 0; thisNote < (sizeof(Starwars_note)/sizeof(int)); thisNote++) {
    int noteDuration = 1000 / Starwars_duration[thisNote];//convert duration to time delay
    timerAlarmWrite(My_timer,Starwars_note[thisNote], true);
    timerAlarmEnable(My_timer);
    delay(noteDuration+(spd*200));
    timerAlarmDisable(My_timer);
    int pauseBetweenNotes = noteDuration * (1.15); //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    t=true;
    }
  }