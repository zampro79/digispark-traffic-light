const uint8_t VERDE = 0;
const uint8_t GIALLO = 1;
const uint8_t ROSSO = 2;
const uint8_t OFF = 3;

const uint8_t PIN_VERDE = 0;
const uint8_t PIN_GIALLO = 1;
const uint8_t PIN_ROSSO = 4;

const uint8_t NUM_CICLI_SEMAFORO = 10;
const uint8_t NUM_LAMPEGGI_GIALLI = 10;

const unsigned long RITARDO_VERDE = 20000UL;
const unsigned long RITARDO_GIALLO = 5000UL;
const unsigned long RITARDO_ROSSO = 20000UL;
const unsigned long RITARDO_STANDBY = 1000UL;
const unsigned long RITARDO_TEST = 25UL;

void cambiaLuce(uint8_t luce)
{
  // Prima spegne tutte le uscite, evitando sovrapposizioni tra le luci.
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_GIALLO, LOW);
  digitalWrite(PIN_ROSSO, LOW);

  switch (luce)
  {
    case VERDE:
      digitalWrite(PIN_VERDE, HIGH);
      break;
    case GIALLO:
      digitalWrite(PIN_GIALLO, HIGH);
      break;
    case ROSSO:
      digitalWrite(PIN_ROSSO, HIGH);
      break;
    case OFF:
    default:
      break;
  }
}

void semaforoOff()
{
  cambiaLuce(OFF);
}

void initSequence()
{
  for (uint8_t i = 0; i < 10; i++)
  {
    cambiaLuce(VERDE);
    delay(RITARDO_TEST);
    cambiaLuce(GIALLO);
    delay(RITARDO_TEST);
    cambiaLuce(ROSSO);
    delay(RITARDO_TEST);
    cambiaLuce(OFF);
    delay(RITARDO_TEST);
  }

  delay(1000UL);
}

void semaforo()
{
  cambiaLuce(VERDE);
  delay(RITARDO_VERDE);
  cambiaLuce(GIALLO);
  delay(RITARDO_GIALLO);
  cambiaLuce(ROSSO);
  delay(RITARDO_ROSSO);
}

void semaforoStandby()
{
  cambiaLuce(GIALLO);
  delay(RITARDO_STANDBY);
  cambiaLuce(OFF);
  delay(RITARDO_STANDBY);
}

void setup()
{
  // Imposta prima i latch a LOW, quindi abilita i pin come uscite.
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_GIALLO, LOW);
  digitalWrite(PIN_ROSSO, LOW);

  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_GIALLO, OUTPUT);
  pinMode(PIN_ROSSO, OUTPUT);

  semaforoOff();
  initSequence();

  for (uint8_t i = 0; i < NUM_CICLI_SEMAFORO; i++)
  {
    semaforo();
  }

  for (uint8_t i = 0; i < NUM_LAMPEGGI_GIALLI; i++)
  {
    semaforoStandby();
  }

  semaforoOff();
}

void loop()
{
  // La sequenza si ripete soltanto dopo un reset.
}
