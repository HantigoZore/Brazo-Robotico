#include <Arduino.h>

// ======================== CONFIGURACIÓN ========================
struct Motor {
    int encA, encB;       // Pines del encoder
    int in1, in2;         // Pines de dirección
    int pwm;              // Pin de PWM
    volatile long ticks;  // Contador de ticks del encoder
    long lastTicks;       // Para calcular velocidad
    float velocity;       // Velocidad calculada
};

// ======================== CONSTANTES ========================
const float PPR_AB = 64.0;     // Pulsos por vuelta motor A/B
const float GEAR_AB = 270.0;   // Relación A/B
const float CPR_AB = PPR_AB * GEAR_AB; // Pulsos por vuelta eje

const float PPR_DE = 11.0;     // Pulsos por vuelta motor D/E
const float GEAR_DE = 200.0;   // Relación D/E
const float CPR_DE = PPR_DE * GEAR_DE; // Pulsos por vuelta eje

// ======================== MOTORES ========================
Motor motorA = {36, 39, 32, 33, 23, 0, 0, 0};
Motor motorB = {34, 35, 32, 33, 5, 0, 0, 0};
Motor motorC = {26, 25, 21, 22, 15, 0, 0, 0};
Motor motorD = {14, 27, 19, 18, 16, 0, 0, 0};
Motor motorE = {13, 4, 19, 18, 17, 0, 0, 0};

// ======================== ISR ENCODERS ========================
void IRAM_ATTR encoderISR_A() { motorA.ticks++; }
void IRAM_ATTR encoderISR_B() { motorB.ticks++; }
void IRAM_ATTR encoderISR_C() { motorC.ticks++; }
void IRAM_ATTR encoderISR_D() { motorD.ticks++; }
void IRAM_ATTR encoderISR_E() { motorE.ticks++; }

// ======================== CONFIGURAR MOTOR ========================
void setupMotor(Motor &m, void (*isr)()) {
    pinMode(m.encA, INPUT_PULLUP);
    pinMode(m.encB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m.encA), isr, RISING);

    pinMode(m.in1, OUTPUT);
    pinMode(m.in2, OUTPUT);
    pinMode(m.pwm, OUTPUT);
}

// ======================== CONTROL DIRECCIÓN ========================
void setDirectionAB(int dir) {
    if (dir == 1) { digitalWrite(32, HIGH); digitalWrite(33, LOW); }
    else if (dir == -1) { digitalWrite(32, LOW); digitalWrite(33, HIGH); }
    else { digitalWrite(32, LOW); digitalWrite(33, LOW); }
}

void setDirectionC(int dir) {
    if (dir == 1) { digitalWrite(21, HIGH); digitalWrite(22, LOW); }
    else if (dir == -1) { digitalWrite(21, LOW); digitalWrite(22, HIGH); }
    else { digitalWrite(21, LOW); digitalWrite(22, LOW); }
}

void setDirectionDE(int dir) {
    if (dir == 1) { digitalWrite(19, HIGH); digitalWrite(18, LOW); }
    else if (dir == -1) { digitalWrite(19, LOW); digitalWrite(18, HIGH); }
    else { digitalWrite(19, LOW); digitalWrite(18, LOW); }
}

// ======================== PWM ========================
void setupPWM(int channel, int pin) {
    ledcSetup(channel, 20000, 8); // 20 kHz, 8 bits
    ledcAttachPin(pin, channel);
}
void setSpeed(int channel, int value) {
    ledcWrite(channel, constrain(value, 0, 255));
}

// ======================== SETUP ========================
void setup() {
    Serial.begin(115200);

    // PWM canales distintos
    setupPWM(0, motorA.pwm);
    setupPWM(1, motorB.pwm);
    setupPWM(2, motorC.pwm);
    setupPWM(3, motorD.pwm);
    setupPWM(4, motorE.pwm);

    // Configurar encoders
    setupMotor(motorA, encoderISR_A);
    setupMotor(motorB, encoderISR_B);
    setupMotor(motorC, encoderISR_C);
    setupMotor(motorD, encoderISR_D);
    setupMotor(motorE, encoderISR_E);
}

// ======================== LOOP ========================
unsigned long lastPrint = 0;

void loop() {
    unsigned long now = millis();

    if (now - lastPrint >= 500) {
        float dt = (now - lastPrint) / 1000.0; // en segundos

        // ---- Motor A ----
        long deltaA = motorA.ticks - motorA.lastTicks;
        motorA.lastTicks = motorA.ticks;
        float revA = deltaA / CPR_AB;          // vueltas en dt
        float rpmA = (revA / dt) * 60.0;       // RPM
        float posA = (motorA.ticks % (long)CPR_AB) * (360.0 / CPR_AB);

        // ---- Motor B ----
        long deltaB = motorB.ticks - motorB.lastTicks;
        motorB.lastTicks = motorB.ticks;
        float revB = deltaB / CPR_AB;
        float rpmB = (revB / dt) * 60.0;
        float posB = (motorB.ticks % (long)CPR_AB) * (360.0 / CPR_AB);

        // ---- Motor C ---- (ticks, porque no conocemos CPR)
        long deltaC = motorC.ticks - motorC.lastTicks;
        motorC.lastTicks = motorC.ticks;
        float velC = deltaC / dt;  // ticks/s

        // ---- Motor D ----
        long deltaD = motorD.ticks - motorD.lastTicks;
        motorD.lastTicks = motorD.ticks;
        float revD = deltaD / CPR_DE;
        float rpmD = (revD / dt) * 60.0;
        float posD = (motorD.ticks % (long)CPR_DE) * (360.0 / CPR_DE);

        // ---- Motor E ----
        long deltaE = motorE.ticks - motorE.lastTicks;
        motorE.lastTicks = motorE.ticks;
        float revE = deltaE / CPR_DE;
        float rpmE = (revE / dt) * 60.0;
        float posE = (motorE.ticks % (long)CPR_DE) * (360.0 / CPR_DE);

        // ---- Serial ----
        Serial.printf("A Pos:%.2f° Vel:%.2f RPM | ", posA, rpmA);
        Serial.printf("B Pos:%.2f° Vel:%.2f RPM | ", posB, rpmB);
        Serial.printf("C Ticks:%ld Vel:%.2f ticks/s | ", motorC.ticks, velC);
        Serial.printf("D Pos:%.2f° Vel:%.2f RPM | ", posD, rpmD);
        Serial.printf("E Pos:%.2f° Vel:%.2f RPM\n", posE, rpmE);

        lastPrint = now;
    }

    // ===== Ejemplo de uso =====
    setDirectionAB(1);
    setSpeed(0, 150);
    setSpeed(1, 200);

    setDirectionC(-1);
    setSpeed(2, 180);

    setDirectionDE(1);
    setSpeed(3, 220);
    setSpeed(4, 100);
}
