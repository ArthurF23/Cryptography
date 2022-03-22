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
  string actual_string;
  AES::aes_init(AES::OPTIONS::doGenerateKey); //Call before use

  //Output key
  cout << "Key: ";
  for(int i=0; i<sizeof(AES::KEY::key)/sizeof(AES::KEY::key[0]); ++i) {
    cout << AES::KEY::key[i] << " ";
  };
  cout << endl;
  
  cout << "Input what you would like encrypted:" << endl;
  cin.ignore();
  getline(cin >> noskipws, actual_string);
  

  cout << endl << "Plaintext to be encrypted:"<< endl << actual_string << endl;

  //Encryption, output ciphertext  
  actual_string = AES::encrypt(actual_string);  
  cout << "Encrypted ciphertext:" << endl << actual_string << endl;
  //Decrypt, output plaintext 
  actual_string = AES::decrypt(actual_string);
  cout << "Decrypted plaintext:" << endl << actual_string << endl;
  cout << "Press any key to continue" << endl;
  cin.ignore();
  return;
};

//3
void duoExample() {
  string input;
  cout << "Please input a string" << endl;
  cin.ignore();
  cin >> input;

  DUO::init("82468224");

  input = DUO::encrypt(input/*, encryption::encdec::FLAGS::no_bloat, encryption::encdec::FLAGS::no_rand_pattern*/);

  cout << "Encrypted Result: " << endl << input << endl << "Decrypted Result: " << endl;

  input = DUO::decrypt(input);

  cout << input << endl;
};

//4
void textFromFileExample() {
  string input;
  cout << "Please input the path to the file: " << endl;
  cin.ignore();
  cin >> input;
  if (!AES::encryptFF(input)) {return;}; //returns false if path is bad
  ifstream readfile(input);
  string line;
  cout << "Encrypted: " << endl;
  while ( getline (readfile,line) ) {
      cout << line << endl;
  };
  readfile.close();
  if (!AES::decryptFF(input)) {return;};
  readfile.open(input);
  cout << "Decrypted: " << endl;
  while ( getline (readfile,line) ) {
      cout << line << endl;
  };
  readfile.close();
};


//5
inline void wholeFileEncryptionExample() {
  AES::aes_init(AES::OPTIONS::doGenerateKey);
  
  cout << "Generated key: ";
  for(int i=0; i<sizeof(AES::KEY::key)/sizeof(AES::KEY::key[0]); ++i) {
    cout << AES::KEY::key[i] << " ";
  };
  cout << endl << endl << endl;
  
  string input;
  printf("Please input the path to the file (please see testFiles folder): \n");
  cin.ignore();
  cin >> input;
  
  
  if (!AES::encryptFile(input)) {return;}; //returns false if path is bad

  printf("Please input new path to .aesenc file\n");
  cin.ignore();
  string newInput;
  cin >> newInput;
  printf("encrypted\ndecrypting...\n");
  thread dec(AES::decryptFile, ref(newInput), "", (AES::FILE_FLAGS)(AES::FILE_FLAGS::deleteAesencFile | AES::FILE_FLAGS::deleteKeyFile));
  dec.join();
};

//6
void vigenereExample() {
  string inp;
  cout << "Are you" << endl << "encrypting = e" << endl << "decrypting = d" << endl; cin.ignore(); cin >> inp;

  string text;
  cout << "Input text here:" << endl; cin.ignore(); cin >> text;


  if (inp[0] == 'e') {
    text = "Encrypted: \n" + vigenere::encrypt(text);
  }
  else if (inp[0] == 'd') {
    string _key;
    cout << "Please input the key (no spaces please)" << endl; cin.ignore(); cin >> _key;
    text = "Decrypted: \n" + vigenere::decrypt(text, _key);
  }
  else {
    cout << "Err | Incorrect input" << endl;
    return vigenereExample();
  };
  cout << "Key:" << endl << VIGENERE_KEY::key << endl << text << endl; 
};

string text[8] = {"Thank you for using my Encryption/Decryption header.\nPlease visit my Github at ArthurF23\nVersion " + VERSION::ver + "\n\n##########################\n" + "\nWhich example would you like to use?", "encdec = 1", "AES = 2", "DUO = 3", "AES text from file = 4", "AES whole file encryption = 5", "Vigenere cypher = 6", "Please input the cooresponding number to your desired example"};

int main() {
  AES::aes_init(AES::OPTIONS::doGenerateKey);
  cout << AES::encryptFile("testFiles/bitmap/image.bmp") << endl;
  cout << AES::decryptFile("testFiles/bitmap/image.aesenc", "", (AES::FILE_FLAGS)(AES::FILE_FLAGS::deleteAesencFile | AES::FILE_FLAGS::deleteKeyFile)) << endl;
  return 0;
  cout << text[0] << endl;
  cout << text[1] << endl;
  cout << text[2] << endl;
  cout << text[3] << endl;
  cout << text[4] << endl;
  cout << text[5] << endl;
  cout << text[6] << endl;
  cout << text[7] << endl;
  
  char input;
  cin >> input;
  
  switch (input) {
    case '1':
      encdecExample();
      break;
    case '2':
      aesExample();
      break;
    case '3':
      duoExample();
      break;
    case '4':
      textFromFileExample();
      break;
    case '5':
      wholeFileEncryptionExample();
      break;
    case '6':
      vigenereExample();
      break;
      
    default:
      cout << "Err please input 1, 2, 3, 4, 5, or 6" << endl;
      return main();
  };
  return main();
}