#include "Headers/includes.h"

//1
void encdecExample() {
  cout << "Are you Encrypting or Decrypting?" << endl << "e/d" << endl;
    char user_choice; cin >> user_choice; cout << "Please input the text" << endl << "Input here:" << endl; string user_inputted_string;
    cin.ignore();
    getline(cin >> noskipws, user_inputted_string);
    
    if (user_choice == 'e') {
      KEY::key = encdec::generate_key();
      output_str = encdec::encrypt(user_inputted_string);
    }
    else if (user_choice == 'd') {
      cout << "Please enter the numeric key" << endl;
      string _key;
      cin >> _key;
      encdec::assign_key(_key);
      output_str = encdec::decrypt(user_inputted_string);
      cin.ignore();
    }
    else {
      cout << "ERR please try again" << endl;
      return;
    };
    
    cout << "Key: " << KEY::key << endl << "Result: " << endl << output_str << endl;
    cout << "Press any key to continue " << endl;
    cin.ignore();
};

//2
void aesExample() {

};

//3
void duoExample() {

};

//4
void textFromFileExample() {

};

//5
void wholeFileEncryptionExample() {

};

string GREETING = "Thank you for using my Encryption/Decryption header.\nPlease visit my Github at ArthurF23\nVersion " + VERSION::ver + "\n\n##########################\n" + "\nWhich example would you like to use?\n";

int main() {
  cout << GREETING;
  cout << "encdec = 1" << endl;
  cout << "AES = 2" << endl;
  cout << "DUO = 3" << endl;
  cout << "AES text from file = 4" << endl;
  cout << "AES whole file encryption = 5" << endl;
  cout << "Please input the cooresponding number to your desired example" << endl;
  char input;
  cin >> input;
  switch (input) {
    case '1':
      encdecExample();
      break;
    case '2':
      AES::start_example();
      break;
    case '3':
      DUO::example();
      break;

    default:
      cout << "Err please input 1, 2, or 3" << endl;
      return main();
  };
  return main();
}