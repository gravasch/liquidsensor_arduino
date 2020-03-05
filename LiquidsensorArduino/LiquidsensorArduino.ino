/*
 * Liquidsensor Arduino 
 * Αυτό το πρόγραμμα τρέχει σε Arduino Uno.
 * Παρατηρεί την στάθμη των υδάτων μέσω των αισθητήρων υγρών και εμφανίζει τα αντίστοιχα μηνύματα σε μία οθόνη LCD
 * Όταν η στάθμη των υδάτων βρίσκεται στο ανώτερο επίπεδο το ηχείο buzzer προειδοποιεί και το πρόσθετο module SIM900 στέλνει μήνυμα για να προειδοποιήσει τον χρήστη. 
 * Η οθόνη LCD εμφανίζει τα αντίστοιχα μηνύματα. 
 */
 

// προσθέτουμε τις απαραίτητες βιβλιοθήκες 
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>




void sendSMS();         // δηλώνουμε την συνάρτηση αποστολής μηνυμάτων 

void setup() {               //Η συνάρτηση setup() εκτελείται μία φορά όταν το Arduino ανοίξει 
  
  // αρχικοποιούμε τις πόρτες του Arduino 
  
  SoftwareSerial SIM900(7, 8); 
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
  int peizoPin=A1;
  
  //αρχικοποίηση μεταβλητών
  
  boolean layer_Low = 0;     // Μεταβλητή πρώτου αισθητήρα υδάτων 
  boolean layer_Normal = 0;  //Μεταβλητή δεύτερου αισθητήρα υδάτων 
  boolean layer_High = 0;   //Μεταβλητή τρίτου  αισθητήρα υδάτων 
  String layerText="";      //Μεταβλητή κειμένου 
  int counterTime=0;        //Μεταβλητή - μετρητής προσπαθιών...  όταν 0 επιτρέπει την κάρτα SIM να στείλει μήνυμα
         
  
  lcd.begin(16, 2);               // ορίζουμε τις γραμμές και της στίλες που έχει η LCD οθόνη 
  lcd.print(" Liquid Sensor  ");  // τυπώνουμε μήνυμα στην οθόνη 
  tone(peizoPin,3000,500);        //ορίζουμε συχνότητα και διάρκεια στο ηχείο  buzzer .
                                  //ακούγεται ο χαρακτηριστικός ήχος που δηλώνει το άνοιγμα του συστήματος 
  delay(1000);                    //δίνουμε χρόνο για να ακουστεί ο ήχος 

  SIM900.begin(19200);            //ορίζουμε ρυθμό  επικοινωνίας μεταξύ του Arduino και to SIM 900  
  delay(20000);                   //δίνουμε χρόνο για να διαβαστεί η sim κάρτα
  

}


void loop() {              //η συνάρτηση loop() εκτελείται συνεχόμενα Όσο το Arduino έχει ρεύμα

  //διαβάζουμε τις τιμές των αισθητήρων υδάτων
  
  layer_Low = digitalRead(13);
  layer_Normal = digitalRead(10);
  layer_High = digitalRead(9);

  
  //ελέγχουμε αν η στάθμη των υδάτων είναι πολύ χαμηλή
  if (layer_Low== 0 && layer_Normal == 0 && layer_High == 0){
    
    //εμφανίζουμε το αντίστοιχο μήνυμα
    lcd.setCursor(0, 0);
    lcd.print(" Liquid Sensor  ");      
    lcd.setCursor(0, 1);
    lcd.print("Liquid layer Low");
    }
    
  //ελέγχουμε αν η στάθμη των υδάτων είναι σε κανονικό επίπεδο   
  if (layer_Low== 1 && layer_Normal == 0 && layer_High == 0){
  
    //εμφανίζουμε το αντίστοιχο μήνυμα      
    lcd.setCursor(0, 0);
    lcd.print(" Liquid Sensor  ");
    lcd.setCursor(0, 1);
    lcd.print("Liquid layer Normal");
    }

  // έχουμε στάθμη των υδάτων είναι σε υψηλό επίπεδο 
  if (layer_Low== 1 && layer_Normal == 1 && layer_High == 0){
    
    //εμφανίζουμε το αντίστοιχο μήνυμα  
    lcd.setCursor(0, 0);
    lcd.print(" Liquid Sensor  ");
    lcd.setCursor(0, 1);
    lcd.print("Liquid layer High");
    }

  //ελέγχουμε αν η στάθμη των υδάτων είναι πάνω από το όριο
  if (layer_Low== 1 && layer_Normal == 1 && layer_High == 1){
    //εμφανίζουμε το αντίστοιχο μήνυμα   
    lcd.setCursor(0, 0);
    lcd.print("     Danger!    ");        
    lcd.setCursor(0, 1);
    lcd.print("Very High layer ");

    //έλεγχο του μετρητή μας Αν επιτρέπεται να στείλουμε μήνυμα
    if (counterTime == 0){
      sendSMS();                      //αποστολή μηνύματος

      //εμφανίζουμε το αντίστοιχο μήνυμα
      lcd.setCursor(0, 0);
      lcd.print(" Sending message  ");
      delay(10000);                    //δίνουμε χρόνο στην κάρτα SIM να στείλει το μήνυμα
      counterTime = 360000;           //δίνουμε κατάλληλη τιμή στο μετρητή για να μην επιτρέπεται να ξανά στείλει κατευθείαν μήνυμα
      } else {                              //αν δεν επιτρέπεται να στείλουμε μήνυμα
        counterTime = counterTime - 1 ;     //αλλάζουμε τιμή στο μετρητή
        }
    //ακούγεται ο χαρακτηριστικός ήχος εκτάκτου ανάγκης
    tone(peizoPin,3000,500);
    delay(1000);
    tone(peizoPin,3000,500);
    delay(1000);        
    tone(peizoPin,3000,500);
    delay(1000);
    }
}



//ανάπτυξη συνάρτησης αποστολής μηνύματος. H συνάρτηση δίνεται μαζί με το module sim900
void sendSMS() {
  
  SIM900.print("AT+CMGF=1\r");       // ορισμός του ΣΙΜ900 σε λειτουργία sms
  delay(100);

  // αντικαταστήστε τα Χ με τα ψηφία του κινητού τηλεφώνου
  //SIM900.println("AT + CMGS = \"+ΧΧΧΧΧΧΧΧΧΧΧΧ\"");
  // χρησιμοποιούμε το διεθνές σύστημα κωδικοποίησης για κινητά τηλέφωνα
  SIM900.println("AT + CMGS = \"+306949165881\""); 
  delay(100);
  
  // αντικαθιστούμε το κείμενο με το κείμενο που θέλουμε να στείλουμε μέσω sms
  SIM900.println("ATTENTION \nLiquid level VERY HIGH!!  \nMessage sent from LiquidSensorArduino. "); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();

  delay(5000);   //δίνουμε χρόνο στο μοντέλο να στείλει το sms

}
