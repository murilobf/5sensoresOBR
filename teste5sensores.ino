#include <AFMotor.h>
/// Porta Sensor de Linha Preta Esquerdo mais distante do centro
#define PSLP_ESQ_EX 7
// Porta Sensor de Linha Preta Esquerda mais perto do centro 
#define PSLP_ESQ_IN 8
// Porta Sensor de Linha Preta do centro
#define PSLP_MEIO 9
// Porta Sensor de Linha Preta Direito mais perto do centro
#define PSLP_DIR_IN 10
// Porta Sensor de Linha Preta Direito mais distante do centro
#define PSLP_DIR_EX 11


// Porta do motor esquerdo
#define PORTA_MOT_E 1
// Porta do motor direito
#define PORTA_MOT_D 2

// globais para armazenar a leitura dos sensores de linha
int sen_esq_ext = 0;
int sen_esq_int = 0;
int sen_meio = 0;
int sen_dir_int = 0;
int sen_dir_ext = 0;

typedef struct Vetor2i{
    int x;
    int y;
} Vetor2i;

Vetor2i rotacao;

#define MOV_LEVE 1
#define MOV_BRUCO 2
#define MOV_DELAY 3.0

AF_DCMotor Motor_esq(PORTA_MOT_E);
AF_DCMotor Motor_dir(PORTA_MOT_D);

uint8_t velocidade = 200;

#define INC_MOTOR_REAL 0.02 // 2%
double porcentagem_motor_Esq_real = 0.9;
double porcentagem_motor_Dir_real = 0.9;

void setup()
{
    // inicializando os sensores de linha preta
    pinMode(PSLP_ESQ_EX, INPUT);
    pinMode(PSLP_ESQ_IN, INPUT);
    pinMode(PSLP_MEIO, INPUT);
    pinMode(PSLP_DIR_IN, INPUT);
    pinMode(PSLP_DIR_EX, INPUT);

    Serial.begin(9600);
}

// loop de estados simples para gerar rotina
void loop()
{
    le_linhas();
    
    Serial.println(sen_esq_ext);
    Serial.println(sen_esq_int);
    Serial.println(sen_meio);
    Serial.println(sen_dir_int);
    Serial.println(sen_dir_ext);

    define_curva();
    anda();
    delay(1000);
}

// funcao que inicializa e atualiza todas as informacoes nescessarias para fazer a escolha de rotacao
void le_linhas()
{
     sen_esq_ext = digitalRead(PSLP_ESQ_EX);
     sen_esq_int = digitalRead(PSLP_ESQ_IN);
     sen_meio = digitalRead(PSLP_MIN);
     sen_dir_int = digitalRead(PSLP_DIR_IN);
     sen_dir_ext = digitalRead(PSLP_DIR_EX);
}

// função que avalia as informações lidas e escolhe que caminho seguir
void define_curva()
{
    // possibilidades de leitura
    // definir o comportamento pra mais de uma leitura igual e nao só na primeira

    //arvore de escolhas
    if (sen_Esq_Meio)
    {
        if (sen_Dir_Meio)
        {
            if (sen_Esq_Lado)
            {
                if (sen_Dir_Lado)
                    para(); // P P P P
                else // !sen_Dir_Lado
                    curva_quadrada_esq(); // P P P B
            }
            else // !sen_Esq_Lado
            {
                if (sen_Dir_Lado)
                    curva_quadrada_dir(); // B P P P
                else // !sen_Dir_Lado
                    reto(); // B P P B
            }
        }
        else // !sen_Dir_Meio
        {
            if ( !(sen_Esq_Lado || sen_Dir_Lado) )
                curva_pouquinho_dir(); // B P B B
        }
    }
    else // !sen_Esq_Meio
    {
        if(sen_Dir_Meio)
            if(!(sen_Esq_Lado || sen_Dir_Lado))
                curva_pouquinho_esq(); // B B P B
        else // !sen_Dir_Meio
        {
            if( !(sen_Esq_Lado || sen_Dir_Lado) )
                anda_sem_rumo(); // B B B B
            else if (sen_Esq_Lado)
                curva_quadrada_esq(); // P B B B
            else if (sen_Dir_Lado)
                curva_quadrada_dir(); // B B B P
        }
    }
}

// funcoes de definicao de rotacao // TEM MUITO O QUE MELHORAR, È SÒ UMA BASE

void para()
{
    rotacao.x = 0;
    rotacao.y = 0;
    set_motores(0, RELEASE, RELEASE);
}
void reto()
{
    rotacao.x = 0;
    rotacao.y = 0;
}
void curva_quadrada_esq()
{
    rotacao.x = 0;
    rotacao.y = 90;
}
void curva_quadrada_dir()
{
    rotacao.x = 0;
    rotacao.y = -90;
}
void curva_pouquinho_esq()
{
    rotacao.x = 0;
    rotacao.y = 30;
}
void curva_pouquinho_dir()
{
    rotacao.x = 0;
    rotacao.y = -30;
}
void anda_sem_rumo()
{
    reto();
}

// dada a escolha de caminho essa função segue o caminho
void anda()
{
   /* if (rotacao.y > 0)
        curva(ESQUERDA, rotacao.y);
    else if (rotacao.y < 0)
        curva(DIREITA, abs(rotacao.y));*/
}


void set_motores(uint8_t velocidade, int direcao_esq, int direcao_dir)
{
  Motor_esq.setSpeed((uint8_t) (velocidade * porcentagem_motor_Esq_real));
  Motor_dir.setSpeed((uint8_t) (velocidade * porcentagem_motor_Dir_real));
  Motor_esq.run(direcao_esq);
  Motor_dir.run(direcao_dir);
}

void curva(int direcao, double angulo)
{  
  /*if (direcao == ESQUERDA)
    set_motores(velocidade, FORWARD, BACKWARD);
  else if (direcao == DIREITA)
    set_motores(velocidade, BACKWARD, FORWARD);

  delay(MOV_DELAY * angulo);*/
}