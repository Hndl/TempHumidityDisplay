

#include <LiquidCrystal.h>
#include <dht_nonblocking.h>

/* Uncomment according to your sensortype. */
#define DHT_SENSOR_TYPE DHT_TYPE_11


static const int DHT_SENSOR_PIN = 7;
static const unsigned long DHT_SAMPLE_RATE = 3000UL;
static const int LCD_RS = 12; 
static const int LCD_EN = 11;
static const int LCD_D4 = 5; 
static const int LCD_D5 = 4; 
static const int LCD_D6 = 3; 
static const int LCD_D7 = 2;
static const int LCD_MAXROW = 2;
static const int LCD_MAXCOL = 20;


static const int SETUP_DELAY = 1000;

static const int  SERIAL_BRATE = 9600;
static const bool SERIAL_DEBUG = true;


static const char DEGREE_C = 176;
static const char *REPORT_STRINGS[] = {"Temp ", "Humidity ", "%"};
static const char *BOOTING_MSG[] = {"Booting..","Owner Detect..",">>Izzy<<","OS:Ard","Intelli Ver:1.0"};

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

/*
 * Initialize the serial port.
 */
void setup( ){
  
  Serial.begin( SERIAL_BRATE );
  lcd.begin(LCD_MAXCOL, LCD_MAXROW);
  int n = sizeof(BOOTING_MSG) / sizeof(char);

  for ( int i = 0 ;  i < n ; i++ ) {
    lcd.clear();
    lcd.print(BOOTING_MSG[i]);
    delay(SETUP_DELAY);
  }
}



/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > DHT_SAMPLE_RATE )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}



void report_environment( float temperature, float humidity ){

  if (SERIAL_DEBUG){
    Serial.print( REPORT_STRINGS[0] );
    Serial.print( temperature, 1 );
    Serial.print( REPORT_STRINGS[1] );
    Serial.print( humidity, 1 );
    Serial.println( REPORT_STRINGS[2] );
  }
  
  lcd.clear();
  lcd.print(REPORT_STRINGS[0]); 
  lcd.print(temperature,1);
  lcd.print(DEGREE_C);
  lcd.setCursor(0, 1);
  lcd.print(REPORT_STRINGS[1]); 
  lcd.print(humidity,1);
  lcd.print(REPORT_STRINGS[2]);
}

/*
 * Main program loop.
 */
void loop( )
{
  float temperature;
  float humidity;
  static unsigned long offlineTime = 0UL;
  bool hasReading = measure_environment( &temperature, &humidity );
  if( hasReading ){
    report_environment( temperature, humidity ) ;    
  } 
}
