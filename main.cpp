#include "Headers/includes.h"

int main() {
  cout << "Thank you for using my Encryption/Decryption header. \n Please visit my Github at ArthurF23" << endl;
  cout << "Which example would you like to use?" << endl << "1: simple example" << endl << "2: AES example" << endl << "1 or 2" << endl;
  char input;
  cin >> input;
  switch (input) {
    case '1':
      encdec::start_example();
      break;
    case '2':
      AES::start_example();
      break;

    default:
      cout << "Err please input 1 or 2" << endl;
      return main();
  };
  return main();
}