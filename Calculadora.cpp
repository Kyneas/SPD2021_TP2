//https://www.tinkercad.com/things/fB2LWL5X1h4-spd2021tp2calculadora

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>
#define NO_BOTON 0
#define M1 1
#define M2 2

// prototipos funciones
int SumarDosNumeros (double x, double y);
int RestarDosNumeros (double x, double y);
int MultiplicarDosNumeros (double x, double y);
int DividirUnNumeroPorOtro (double dividendo, double divisor, double* resultado);
int RealizarOperacionMatematica (char operador);
void LimpiarPantalla (int columna, int fila, int cantidadEspacios);
void ImprimirTextoEnPantalla (int columna, int fila, String dato);
void ImprimirNumeroEnPantalla (int columna, int fila, long dato);
void ImprimirDoubleEnPantalla (int columna, int fila, double dato);

LiquidCrystal lcd(0, 1, 2, 3, 4, 13);

char operando1[5];
char operando2[5];
char operador;
double numero1;
double numero2;
double resultado;
double numeroEnM1;
int i = 0;
int flagOperando1 = 1;
int flagOperando2 = 0;
int flagOperador = 0;
int flagResultado = 0;
int flagNuevaOperacion = 0;
int flagHayAlgoEnMemoria = 0;
int flagResultadoEnPantalla = 0;
int flagResultadoErroneo = 0;
int hayNumero = 0;
int contadorSignoNegativo = 0;

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'_', '0', '=', '/'}};

byte rowPins[ROWS] = {12, 11, 10, 9}; 
byte colPins[COLS] = {8, 7, 6, 5};    

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  pinMode(A0, INPUT);
}
int estadoBoton1 = NO_BOTON;
int estadoBoton2;

void loop()
{
  //////////////////////// BOTONES M1 Y M2 /////////////////////////////////////////

  estadoBoton2 = LeerBoton();

  if (estadoBoton2 != NO_BOTON && estadoBoton2 != estadoBoton1) 
  {
    ///////////////////// M1 /////////////////////
    if (estadoBoton2 == M1 && flagResultadoErroneo == 0) 
    {
        if (resultado > 9999 || resultado < -9999)
        {
            LimpiarPantalla(0,1,15);        
            ImprimirTextoEnPantalla(0,1,"Mem.ovf.");
            resultado = 0;
        }
         else
        {
            ImprimirTextoEnPantalla(15,1,"M");

        if (flagOperando1)
        {
          numeroEnM1 = numero1;
          flagHayAlgoEnMemoria = 1;
        }
        else if (flagOperando2)
        {
          numeroEnM1 = numero2;
          flagHayAlgoEnMemoria = 1;
        }
        else if (flagResultadoEnPantalla)
        {
          numeroEnM1 = resultado;
          flagHayAlgoEnMemoria = 1;
        }
      }
    }
    //////////////////// M2 /////////////////////////
    
    else if (estadoBoton2 == M2 && flagHayAlgoEnMemoria) 
    {
      if (flagOperando1 || flagResultadoEnPantalla)
      {
          LimpiarPantalla(0,0,16);
          LimpiarPantalla(0,1,15);
        
        if (numeroEnM1 >= 0)
        {
            ImprimirNumeroEnPantalla(1,0,(long)numeroEnM1);
        }
        else
        {     
            ImprimirNumeroEnPantalla(0,0,(long)numeroEnM1);
        }
        numero1 = numeroEnM1;
        flagOperando1 = 0;
        flagOperador = 1;
        flagResultadoEnPantalla = 0;
        flagNuevaOperacion = 0;
      }
      else if (flagOperando2)
      {
        LimpiarPantalla(8,0,5);
        ImprimirNumeroEnPantalla(9,0,(long)numeroEnM1);
        numero2 = numeroEnM1;
        flagOperando2 = 0;
        flagResultado = 1;
      }
    }
  }
  estadoBoton1 = estadoBoton2;

  char key = keypad.getKey();

  //////////////////////Limpiar para una nueva operacion/////////////////////////////
  if (key != NO_KEY)
  {
    if (flagNuevaOperacion && isdigit(key))
    {
      LimpiarPantallaSinBorrarElIndicadorDeMemoria();
      
      flagNuevaOperacion = 0;
      contadorSignoNegativo = 0;
      flagResultadoEnPantalla = 0;
      flagOperando1 = 1;
    }
    /////////////////////////////////////////OPERANDO UNO////////////////////////////////////////
    if (flagOperando1 && i < 5 && (isdigit(key) || key == '_'))
    {
      flagResultadoErroneo = 0;
      
      if (key == '_' && hayNumero)
      {
        contadorSignoNegativo++;
      }
      else if (isdigit(key) && i < 4)
      {
        ImprimirTextoEnPantalla(1+i,0,(String)key);
        operando1[i] = key;
        i++;
        hayNumero = 1;
        flagOperador = 1;
      }

      if (contadorSignoNegativo % 2 == 1 && hayNumero)
      {
        ImprimirTextoEnPantalla(0,0,"-");

        numero1 = atof(operando1) * -1; /////Convierto string a double neg
      }
      else if (contadorSignoNegativo % 2 == 0 && hayNumero)
      {
        ImprimirTextoEnPantalla(0,0," ");
        numero1 = atof(operando1); /////Convierto string a double
      }
    }

    ////////////////////////////////////////  OPERADORES + - * /  ///////////////////////////////////////
    if (!isdigit(key) && key != '=' && key != '_' && flagOperador)
    {
      operador = key;
      ImprimirTextoEnPantalla(7,0,(String)operador);
      flagOperando1 = 0;
      flagOperando2 = 1;
      i = 0;
      contadorSignoNegativo = 0;
      hayNumero = 0;
      flagOperador = 0;
    }
    /////////////////////////////////////  OPERANDO DOS  ///////////////////////////////////
    if (flagOperando2 && i < 5 && (isdigit(key) || key == '_'))
    {
      if (key == '_' && hayNumero)
      {
        contadorSignoNegativo++;
      }
      else if (isdigit(key) && i < 4)
      {
        ImprimirTextoEnPantalla(10+i,0,(String)key);
        operando2[i] = key;
        i++;
        flagResultado = 1;
        hayNumero = 1;
      }

      if (contadorSignoNegativo % 2 == 1 && hayNumero)
      {
        ImprimirTextoEnPantalla(9,0,"-");
        numero2 = atof(operando2) * -1;
      }
      else if (contadorSignoNegativo % 2 == 0 && hayNumero)
      {
        ImprimirTextoEnPantalla(9,0," ");
        numero2 = atof(operando2);
      }
    }
 /////////////////////////////   RESULTADO  //////////////////////////////
    if (key == '=' && flagResultado)
    {
        if(RealizarOperacionMatematica(operador)==-1)
        {
            ImprimirTextoEnPantalla(0,1,"Math error");
            flagResultadoErroneo = 1;
        }
        else
        {
            LimpiarPantalla(0,1,15);
            ImprimirTextoEnPantalla(0,1,"=");
            ImprimirDoubleEnPantalla (2,1, resultado);
        }

      LimpiarNumero(operando1);
      LimpiarNumero(operando2);

      flagOperando2 = 0;
      flagResultado = 0;
      i = 0;
      flagNuevaOperacion = 1;
      flagResultadoEnPantalla = 1;
    }
  }
  
  delay(50);
}

    //////////////  FUNCIONES  //////////////////////////////

int LeerBoton()
{
    int retorno = NO_BOTON;
    int valorA0 = analogRead(A0);

    if (valorA0 > 195 && valorA0 < 215)
    {
        retorno = M1;
    }

    else if (valorA0 > 502 && valorA0 < 522)
    {
        retorno = M2;
    }

    return retorno;
}

void LimpiarNumero(char numero[])
{
    for (int i = 0; i < 5; i++)
    {
        numero[i] = '\0';
    }
}

void LimpiarPantallaSinBorrarElIndicadorDeMemoria ()
{
    LimpiarPantalla(0,0,16);
    LimpiarPantalla(0,1,15);
}

void LimpiarPantalla (int columna, int fila, int cantidadEspacios)
{
    String espacios;

    for(int i = 0; i < cantidadEspacios; i++)
    {
        espacios += " ";
    } 

    lcd.setCursor(columna, fila);
    lcd.print(espacios); 
}

void ImprimirTextoEnPantalla (int columna, int fila, String dato)
{
    lcd.setCursor(columna,fila);
    lcd.print(dato);
}

void ImprimirNumeroEnPantalla (int columna, int fila, long dato)
{
    lcd.setCursor(columna,fila);
    lcd.print(dato);
}

void ImprimirDoubleEnPantalla (int columna, int fila, double dato)
{
    lcd.setCursor(columna,fila);
    lcd.print(dato);
}

int SumarDosNumeros (double x, double y)
{
  int resultado;
  
  resultado = x + y;
  
  return resultado;
}

int RestarDosNumeros (double x, double y)
{
    int resultado;

    resultado = x - y;

    return resultado;
}

int MultiplicarDosNumeros (double x, double y)
{
    int resultado;

    resultado = x * y;

    return resultado;
}

int DividirUnNumeroPorOtro (double dividendo, double divisor, double* resultado)
{
    int estado;
    estado = -1;

    if (divisor != 0)
    {
        *resultado = dividendo / divisor;
        estado = 0;
    }
    
    return estado;
}

int RealizarOperacionMatematica (char operador)
{
    int estado;
    estado = 0;

    switch (operador)
        {
            case '+':
                resultado = SumarDosNumeros(numero1, numero2);
            break;

            case '-':
                resultado = RestarDosNumeros(numero1, numero2);
            break;

            case '/':
                if (DividirUnNumeroPorOtro(numero1, numero2, &resultado) == -1)
                {
                   estado = -1;
                }

            break;

            case '*':
                resultado = MultiplicarDosNumeros(numero1, numero2);
            break;
        }
    return estado;
}
