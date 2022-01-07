Thank you for using my header.

The program has the key which is located through encryption::KEY::key, you will need to set this before using the encryption::encdec::decrypt() function, since that function grabs from the namespace to use it and the only thing you pass to that function is the string. You must set the key before you call that function. It's not a problem for the encrypt() function since it generates a key each time its called. The generated key is the same varible encryption::KEY::key. Anytime you need to grab the key or set it, it is there, it will be nowhere else.

It is possible to change how much bloat is in the encryption by going to encryption.h and finding the bloat struct and changing the non default values.