#include "Headers/includes.h"

// 1
void aesExample() {
	string actual_string;
	AES::aes_init(AES::OPTIONS::doGenerateKey); // Call before use

	// Output key
	cout << "Key: ";
	for (int i = 0; i < sizeof(AES::KEY::key) / sizeof(AES::KEY::key[0]); ++i) {
		cout << AES::KEY::key[i] << " ";
	};
	cout << endl;

	cout << "Input what you would like encrypted:" << endl;
	cin.ignore();
	getline(cin >> noskipws, actual_string);

	cout << endl
		 << "Plaintext to be encrypted:" << endl
		 << actual_string << endl;

	// Encryption, output ciphertext
	actual_string = AES::encrypt(actual_string);
	cout << "Encrypted ciphertext:" << endl << actual_string << endl;
	// Decrypt, output plaintext
	actual_string = AES::decrypt(actual_string);
	cout << "Decrypted plaintext:" << endl << actual_string << endl;
	cout << "Press any key to continue" << endl;
	cin.ignore();
	return;
};

// 2
void textFromFileExample() {
	string input;
	cout << "Please input the path to the file: " << endl;
	cin.ignore();
	cin >> input;
	if (!AES::encryptFF(input)) {
		return;
	}; // returns false if path is bad
	ifstream readfile(input);
	string line;
	cout << "Encrypted: " << endl;
	while (getline(readfile, line)) {
		cout << line << endl;
	};
	readfile.close();
	if (!AES::decryptFF(input)) {
		return;
	};
	readfile.open(input);
	cout << "Decrypted: " << endl;
	while (getline(readfile, line)) {
		cout << line << endl;
	};
	readfile.close();
};

// 3
inline void wholeFileEncryptionExample() {
	AES::aes_init(AES::OPTIONS::doGenerateKey);

	cout << "Generated key: ";
	for (int i = 0; i < sizeof(AES::KEY::key) / sizeof(AES::KEY::key[0]); ++i) {
		cout << AES::KEY::key[i] << " ";
	};
	cout << endl << endl << endl;

  string pass;
  printf("Please input a password\n");
	cin.ignore();
	cin >> pass;
  
  
	string input;
	printf(
		"Please input the path to the file (please see testFiles folder): \n");
	cin.ignore();
	cin >> input;

	if (!AES::encryptFile(input, pass)) {
		return;
	}; // returns false if path is bad

	printf("Please input new path to .aesenc file\n");
	cin.ignore();
	string newInput;
	cin >> newInput;
  
  printf("Please input your password\n");
	cin.ignore();
	cin >> pass;
  
	printf("Encrypted\nDecrypting...\n");
	thread dec(
		AES::decryptFile,
		ref(newInput),
    ref(pass),
		"",
		(AES::FILE_FLAGS)(
			AES::FILE_FLAGS::deleteAesencFile |
			AES::FILE_FLAGS::deleteKeyFile));
	dec.join();
};

// 5
void aes_debug() {  
  cout << "AES SECTION" << endl;
	cout << "Testing file functions" << endl << endl;
	cout << "1mb" << endl;

  cout << "Reading" << endl;
	ifstream file("testFiles/1mb/1mb.txt");
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	char *buffer = new char[length];
	file.read(buffer, length);
	string text = buffer;
	delete[] buffer;
	file.close();
  cout << "Testing" << endl;

  cout << "Init" << endl;
	AES::aes_init(AES::OPTIONS::doGenerateKey);
  cout << "Encrypting" << endl;
	AES::encryptFile("testFiles/1mb/1mb.txt", "test");
  cout << "Decrypting" << endl;
	AES::decryptFile(
		"testFiles/1mb/1mb.aesenc",
    "test",
		"",
		(AES::FILE_FLAGS)(
			AES::FILE_FLAGS::deleteAesencFile |
			AES::FILE_FLAGS::deleteKeyFile));

  cout << "Comparing" << endl;
	file.open("testFiles/1mb/1mb.txt");
	file.seekg(0, file.end);
	length = file.tellg();
	file.seekg(0, file.beg);
	buffer = new char[length];
	file.read(buffer, length);
	string text2 = buffer;
	delete[] buffer;
	file.close();

	if (text == text2) {
		cout << "Test 1 Success" << endl;
	} else {
    for (long int i = 0; i < text.length(); i++) {
      cout << text[i] << " | " << text2[i] << " | " << (text[i] == text2[i]) << endl;
      if (text[i] != text2[i]) {break;};
    };
		cout << "Test 1 Fail" << endl;
	};

	cout << "Testing Standard Encrypt & Decrypt" << endl;
	text = "this is a test";
	text2 = AES::encrypt(text);
	text2 = AES::decrypt(text2);

	if (text == text2) {
		cout << "Test 2 Success" << endl;
	} else {
		cout << "Test 2 Fail" << endl;
	};

  cout << "Testing bitmap, check image to verify test" << endl;
  AES::encryptFile("testFiles/bitmap/img.bmp", "test");
  AES::decryptFile("testFiles/bitmap/img.aesenc", "test", "", (AES::FILE_FLAGS)(AES::FILE_FLAGS::deleteAesencFile | AES::FILE_FLAGS::deleteKeyFile));
  cout << "finished" << endl;
};

string text[6] = {
	"AES = 1",
	"AES text from file = 2",
	"AES whole file encryption = 3",
  "List all files = 4",
	"AES Debug = 5",
	"Please input the cooresponding number to your desired example"};

bool firstRun = false;
int main() {  
  if (firstRun == false) {
    cout << "Thank you for using my Encryption/Decryption header" << endl 
      << "Please visit my Github at ArthurF23" << endl
      << VERSION_INFO::GITLINK << endl << "Version " << VERSION::ver << endl;
    cout << "Revised " << VERSION_INFO::REVISION_DATE << endl;
    firstRun = true;
  }; 
  
  cout << "\n##########################\n"
    << "\nWhich example would you like to use?" << endl;

	cout << text[0] << endl;
	cout << text[1] << endl;
	cout << text[2] << endl;
	cout << text[3] << endl;
	cout << text[4] << endl;
	cout << text[5] << endl;

	char input;
	cin >> input;
	string files, folders;
	switch (input) {
	case '1':
		aesExample();
		break;
	case '2':
		textFromFileExample();
		break;
	case '3':
		wholeFileEncryptionExample();
		break;
	case '4':
		FileOP::searchDir((char *)"testFiles/", files, folders, true);
		cout << " ------ " << endl << files << endl << " ------ " << endl;
		break;
	case '5':
		aes_debug();
		break;

	default:
		cout << "Err please input 1, 2, 3, 4, 5, 6, or 7" << endl;
		return main();
	};
	return main();
};