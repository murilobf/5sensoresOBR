#include <AFMotor.h>

/// Porta Sensor de Linha Preta Esquerdo mais distante do centro
#define PSLP_ESQ_EX 42
// Porta Sensor de Linha Preta Esquerda mais perto do centro 
#define PSLP_ESQ_IN 44
// Porta Sensor de Linha Preta do centro
#define PSLP_MEIO 46
// Porta Sensor de Linha Preta Direito mais perto do centro
#define PSLP_DIR_IN 48
// Porta Sensor de Linha Preta Direito mais distante do centro
#define PSLP_DIR_EX 50


// Portas do motor esquerdo
AF_DCMotor motorE(1);
AF_DCMotor motorD(2);

uint8_t velocidade_rap = 200;
uint8_t velocidade_med = 180;
uint8_t velocidade_len = 150;

// globais para armazenar a leitura dos sensores de linha
// OBS: para o sensor de 5 linhas preto é 0 e branco é 1
int sen_esq_ext = 0;
int sen_esq_int = 0;
int sen_meio = 0;
int sen_dir_int = 0;
int sen_dir_ext = 0;


void setup()
{
    // inicializando os sensores de linha preta

    Serial.begin(9600);

    pinMode(PSLP_ESQ_EX, INPUT);
    pinMode(PSLP_ESQ_IN, INPUT);
    pinMode(PSLP_MEIO, INPUT);
    pinMode(PSLP_DIR_IN, INPUT);
    pinMode(PSLP_DIR_EX, INPUT);

    motorE.setSpeed(velocidade_med);
    motorD.setSpeed(velocidade_med);
}

// loop de estados simples para gerar rotina
void loop()
{
    le_linhas();
    define_curva();
    delay(1000);
}

// funcao que inicializa e atualiza todas as informacoes nescessarias para fazer a escolha de rotacao
void le_linhas()
{
     sen_esq_ext = digitalRead(PSLP_ESQ_EX);
     sen_esq_int = digitalRead(PSLP_ESQ_IN);
     sen_meio = digitalRead(PSLP_MEIO);
     sen_dir_int = digitalRead(PSLP_DIR_IN);
     sen_dir_ext = digitalRead(PSLP_DIR_EX);

     /*Serial.print(sen_esq_ext);
     Serial.print(sen_esq_int);
     Serial.print(sen_meio);
     Serial.print(sen_dir_int);
     Serial.print(sen_dir_ext);
     Serial.println();*/
}

// função que avalia as informações lidas e escolhe que caminho seguir
// OBS: para o sensor de 5 linhas preto é 0 e branco é 1
void define_curva()
{
    // Casos curva brusca a esquerda
    // O valor do meio não importa pois, tanto para 0 quanto pra 1, se os dois sensores da esquerda estão ativados ele precisa virar pra esquerda
    if(sen_esq_ext == 0 &&
       sen_esq_int == 0 && 
       sen_dir_int == 1 &&
       sen_dir_ext == 1){

          forte_esquerda();
          Serial.println("fore\n");
       }

    // Caso curva mais suave a esquerda
    // Idem para meio
    else if((sen_esq_ext == 1 &&
       sen_esq_int == 0) || 
       (sen_esq_ext == 0 &&
       sen_esq_int == 1)&&
       sen_dir_int == 1 &&
       sen_dir_ext == 1){

        fraco_esquerda();
        Serial.println("frae\n");
       }

    // Vai reto, como os valores dos sensores da esquerda já foram verificados, eles não são necessários, mantenho apenas por questão de legibilidade
    else if(sen_esq_ext == 1 &&
       sen_esq_int == 1 && 
       sen_meio == 0 &&
       sen_dir_int == 1 &&
       sen_dir_ext == 1){

        anda_frente();
        Serial.println("frent\n");
       }

    // Curva suave a direita
    // Idem para o meio
    else if(sen_esq_ext == 1 &&
       sen_esq_int == 1 && 
       (sen_dir_int == 0 &&
       sen_dir_ext == 1) ||
       (sen_dir_int == 1 &&
       sen_dir_ext == 0)){

        fraco_direita();
        Serial.println("fradir\n");
       }

    // Curva brusca a direita
    // Idem para o meio
    else if(sen_esq_ext == 1 &&
       sen_esq_int == 1 && 
       sen_dir_int == 0 &&
       sen_dir_ext == 0){

        forte_direita();
        Serial.println("fordir\n");
       }
}

void forte_esquerda(){
    motorE.setSpeed(0);
    motorD.setSpeed(velocidade_med);

    motorE.run(FORWARD);
    motorD.run(FORWARD);
}

void fraco_esquerda(){

    motorE.setSpeed(velocidade_len);
    motorD.setSpeed(velocidade_med);

    motorE.run(FORWARD);
    motorD.run(FORWARD);
}

void anda_frente(){
    motorE.setSpeed(velocidade_med);
    motorD.setSpeed(velocidade_med);

    motorE.run(FORWARD);
    motorD.run(FORWARD);
}

void fraco_direita(){

    motorE.setSpeed(velocidade_med);
    motorD.setSpeed(velocidade_len);

    motorE.run(FORWARD);
    motorD.run(FORWARD);
}

void forte_direita(){

    motorE.setSpeed(velocidade_med);
    motorD.setSpeed(0);

    motorE.run(FORWARD);
    motorD.run(FORWARD);
}