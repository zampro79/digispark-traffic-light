#define VERDE  0
#define GIALLO 1
#define ROSSO  2
#define OFF    3

int ledBoard            = 1;
int mainCounter         = 0;
bool mainCounterEnable  = true;
int delayInitBlink      = 25;

int semaforo1[]         = { 0, 1, 4 };
int ritardoRosso        = 20000;
int ritardoVerde        = 20000;
int ritardoStandby      = 1000;
int ritardoGiallo       = 5000;
int ritardo;
int semaforoRead;
int lightCycle          = 10;
int standbyCycle        = 20;

int PinBatt             = A2;
float R1                = 2000; //modificare questi valori con i valori realmente utilizzati.
float R2                = 10000;
int ValPinBatt;
float Vpin_perc;
float Vbatt;
float calc_res;

float batteryLowThr     = 4.5;
float batteryHighThr    = 5.5;
int batteryStatus       = 0;

void setup()
{
  pinMode (PinBatt, INPUT);

  for (int i = 0; i < 3; i++)
  {
    pinMode(semaforo1[i], OUTPUT);
  }
}

void loop()
{
  if (mainCounter == 0)
  {
    initSequence();
    //checkBatteria();
  }

  if (0 <= mainCounter && mainCounter <= lightCycle)  {
    semaforo();
  }

  if (lightCycle + 1  <= mainCounter && mainCounter <= standbyCycle) {
    semaforoStandby();
  }

  if (mainCounter > 10000)
  {
    mainCounterEnable = false;
    semaforoOff();
  }

  if (mainCounterEnable == true) {
    mainCounter++;
  }
}

void semaforoStandby()
{

  cambiaLuce(semaforo1, GIALLO, true, false);
  delay(ritardoStandby);
  cambiaLuce(semaforo1, OFF, true, false);
  delay(ritardoStandby);

}

void semaforoOff()
{
  cambiaLuce(semaforo1, OFF, true, false);
}

void initSequence()
{
  for (int i = 0; i < 10; i++)
  {
    cambiaLuce(semaforo1, VERDE, true, false);
    delay (delayInitBlink);
    cambiaLuce(semaforo1, GIALLO, true, false);
    delay (delayInitBlink);
    cambiaLuce(semaforo1, ROSSO, true, false);
    delay (delayInitBlink);
    cambiaLuce(semaforo1, OFF, true, false);
    delay (delayInitBlink);
  }
  delay (1000);

}
void cambiaLuce(int semaforo[], int luce, bool cambiaLuceStampEnable, bool cambiaLuceDelayEnable)
{

  for (int i = 0; i < 3; i++)
  {

    if (i == luce)
    {
      digitalWrite(semaforo[i], HIGH);
    }
    else
    {
      digitalWrite(semaforo[i], LOW);
    }
  }
  switch (luce)
  {
    case 0:
      if (cambiaLuceStampEnable == true) {
      }
      if (cambiaLuceDelayEnable == true) {
        ritardo = ritardoVerde;
      } else {
        ritardo = 0;
      }
      digitalWrite(ledBoard, LOW);
      break;
    case 1:
      if (cambiaLuceStampEnable == true) {
      }
      if (cambiaLuceDelayEnable == true) {
        ritardo = ritardoGiallo;
      } else {
        ritardo = 0;
      }
      digitalWrite(ledBoard, HIGH);
      break;
    case 2:
      if (cambiaLuceStampEnable == true) {

      }
      if (cambiaLuceDelayEnable == true) {
        ritardo = ritardoRosso;
      } else {
        ritardo = 0;
      }
      digitalWrite(ledBoard, LOW);
      break;
    case 3:
      if (cambiaLuceStampEnable == true) {
      }
      if (cambiaLuceDelayEnable == true) {
        ritardo = ritardoRosso;
      } else {
        ritardo = 0;
      }
      digitalWrite(ledBoard, LOW);
    default:
      break;
  }

  delay(ritardo);

}

void checkBatteria()
{
  //Vout = Vin * R2 / (R1 + R2)
  calc_res = (R1 + R2) / R2;
  ValPinBatt  = analogRead(PinBatt);
  Vpin_perc   = map (ValPinBatt, 0, 1023, 0, 500);
  //legge 1023 quando ho 5 volt sul pin, quindi scalo 0-1023 in 0-500.
  Vbatt       = Vpin_perc * calc_res / 100 ;
  //ora divido tutto per 100 perche' sopra ho scalato da 0 a 500 anziche' da 0 a 5V

  if (Vbatt >= batteryHighThr)                          {
    batteryStatus = 0;
  }
  if (batteryLowThr <= Vbatt && Vbatt <= batteryHighThr) {
    batteryStatus = 1;
  }
  if (Vbatt <= batteryLowThr)                           {
    batteryStatus = 2;
  }

  switch (batteryStatus)
  {
    case 2:
      cambiaLuce(semaforo1, ROSSO, false, false );
      break;
    case 1:
      cambiaLuce(semaforo1, GIALLO, false, false );
      break;
    case 0:
      cambiaLuce(semaforo1, VERDE, false, false );
      break;
    default:
      break;
  }
}

void semaforo()
{

  cambiaLuce(semaforo1, VERDE, true, true);
  cambiaLuce(semaforo1, GIALLO, true, true);
  cambiaLuce(semaforo1, ROSSO, true, true);
}
