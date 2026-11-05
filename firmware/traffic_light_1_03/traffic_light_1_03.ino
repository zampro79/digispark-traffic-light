#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

const uint8_t VERDE = 0;
const uint8_t GIALLO = 1;
const uint8_t ROSSO = 2;
const uint8_t OFF = 3;

const uint8_t PIN_VERDE = 0;
const uint8_t PIN_DIAGNOSTICO = 1;
const uint8_t PIN_GIALLO = 2;
const uint8_t PIN_ROSSO = 4;

// Impostare a false sulle revisioni con LED onboard attivo LOW.
const bool LED_DIAGNOSTICO_ATTIVO_HIGH = true;

const uint8_t NUM_CICLI_SEMAFORO = 10;
const uint8_t NUM_LAMPEGGI_GIALLI = 10;

const uint16_t INTERVALLO_DIAGNOSTICA_MS = 200;
const uint8_t INTERVALLI_PATTERN = 15;  // 15 x 200 ms = 3 s
const uint8_t INTERVALLI_VERDE = 100;   // 20 s
const uint8_t INTERVALLI_GIALLO = 25;   // 5 s
const uint8_t INTERVALLI_ROSSO = 100;   // 20 s
const uint8_t INTERVALLI_LAMPEGGIO = 5; // 1 s
const uint8_t INTERVALLI_STANDBY =
  NUM_LAMPEGGI_GIALLI * 2 * INTERVALLI_LAMPEGGIO; // 20 s
const uint8_t RITARDO_TEST_MS = 25;

void impostaLedDiagnostico(bool acceso)
{
  const uint8_t livelloAttivo = LED_DIAGNOSTICO_ATTIVO_HIGH ? HIGH : LOW;
  const uint8_t livelloSpento = LED_DIAGNOSTICO_ATTIVO_HIGH ? LOW : HIGH;
  digitalWrite(PIN_DIAGNOSTICO, acceso ? livelloAttivo : livelloSpento);
}

void cambiaLuce(uint8_t luce)
{
  // Spegne prima tutte le luci per evitare sovrapposizioni.
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

void aggiornaLedDiagnostico(uint16_t intervallo, uint8_t impulsi)
{
  const uint8_t posizione = intervallo % INTERVALLI_PATTERN;
  const bool impulsoAttivo =
    (posizione % 2 == 0) && (posizione < impulsi * 2);
  impostaLedDiagnostico(impulsoAttivo);
}

void attendiConDiagnostica(uint8_t intervalli, uint8_t impulsi)
{
  for (uint8_t i = 0; i < intervalli; i++)
  {
    aggiornaLedDiagnostico(i, impulsi);
    delay(INTERVALLO_DIAGNOSTICA_MS);
  }

  impostaLedDiagnostico(false);
}

void spegniTutto()
{
  cambiaLuce(OFF);
  impostaLedDiagnostico(false);
}

void sospendiDefinitivamente()
{
  // Conserva i quattro pin LED come uscite nello stato spento.
  spegniTutto();

  // Da questo punto non vengono piu' usati delay(), millis() o periferiche.
  cli();

  wdt_reset();
  MCUSR &= ~_BV(WDRF);
  wdt_disable();

  ADCSRA &= ~_BV(ADEN);
  ACSR |= _BV(ACD);

  TIMSK = 0;
  GIMSK = 0;
  PCMSK = 0;
  USICR = 0;
  GIFR = _BV(INTF0) | _BV(PCIF);

  PLLCSR &= ~_BV(PLLE);
  power_all_disable();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Un eventuale risveglio non dovuto a reset rientra subito in Power-down.
  for (;;)
  {
    sleep_bod_disable();
    sleep_cpu();
  }
}

void initSequence()
{
  for (uint8_t i = 0; i < 10; i++)
  {
    impostaLedDiagnostico(true);
    cambiaLuce(VERDE);
    delay(RITARDO_TEST_MS);

    impostaLedDiagnostico(false);
    cambiaLuce(GIALLO);
    delay(RITARDO_TEST_MS);

    impostaLedDiagnostico(true);
    cambiaLuce(ROSSO);
    delay(RITARDO_TEST_MS);

    impostaLedDiagnostico(false);
    cambiaLuce(OFF);
    delay(RITARDO_TEST_MS);
  }

  spegniTutto();
  delay(1000UL);
}

void semaforo()
{
  cambiaLuce(VERDE);
  attendiConDiagnostica(INTERVALLI_VERDE, 1);

  cambiaLuce(GIALLO);
  attendiConDiagnostica(INTERVALLI_GIALLO, 2);

  cambiaLuce(ROSSO);
  attendiConDiagnostica(INTERVALLI_ROSSO, 3);
}

void semaforoStandby()
{
  for (uint8_t i = 0; i < INTERVALLI_STANDBY; i++)
  {
    if (i % INTERVALLI_LAMPEGGIO == 0)
    {
      const bool gialloAcceso =
        (i / INTERVALLI_LAMPEGGIO) % 2 == 0;
      cambiaLuce(gialloAcceso ? GIALLO : OFF);
    }

    aggiornaLedDiagnostico(i, 2);
    delay(INTERVALLO_DIAGNOSTICA_MS);
  }

  spegniTutto();
}

void setup()
{
  // Precarica i livelli spenti prima di abilitare le uscite.
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_GIALLO, LOW);
  digitalWrite(PIN_ROSSO, LOW);
  digitalWrite(
    PIN_DIAGNOSTICO,
    LED_DIAGNOSTICO_ATTIVO_HIGH ? LOW : HIGH
  );

  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_DIAGNOSTICO, OUTPUT);
  pinMode(PIN_GIALLO, OUTPUT);
  pinMode(PIN_ROSSO, OUTPUT);

  spegniTutto();
  initSequence();

  for (uint8_t i = 0; i < NUM_CICLI_SEMAFORO; i++)
  {
    semaforo();
  }

  semaforoStandby();
  sospendiDefinitivamente();
}

void loop()
{
  // La sequenza si ripete soltanto dopo un reset.
}
