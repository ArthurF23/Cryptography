### H3 Thank you for using my header.
The program has the key which is located through `encryption::KEY::key`, you will need to set this before using the `encryption::encdec::decrypt()` function, since that function grabs from the namespace to use it and the only thing you pass to that function is the string. You must set the key before you call that function. It's not a problem for the `encrypt()` function since it generates a key each time its called. The generated key is the same varible `encryption::KEY::key`. Anytime you need to grab the key or set it, it is there, it will be nowhere else.

It is possible to change how much bloat is in the encryption by going to encryption.h and finding the bloat struct and changing the non default values.

The algorithm is very basic and how it works is it takes the key and finds a number that will evenly go into it called scramble which is the first number before any other numbers, so when decrypting it will remove the bloat then take the first number. It takes each char value in the string and adds it to the key/scramble `(string[i] + (key/scramble))`. It then adds the bloat to the output in random places and adds predefined patterns of `> . _` characters to make it seem like theres a pattern and adds them in randomly as well.

### Example

Input:
> 
```
Text Example
```

Generated Key:
>
``` 
72154752
```

Result:
>
```
?BhAwAPw=|a[VH_mB}BRvok_>..rd~bv:W^wl\J~@]LVnkk?Nfw;OWA?Bv.>_>VTNClN@CV{vuLgkvBaZ|MizpOyS2RE>>._fN_mb____rVk}PQQJqd_Yb.>.>qIrJYXIxgILjOCbhG}ZhoZvsUC>RE?BGPaP@X]m@}sJV=]b>.>>Af<zqmL36OB=mtcgK0774<Fybhya^QexJU{lt60>>>gC`uzls:||X>..>XWlST>Y\JlxhzR><twE\mX<cLH>.>_iKL_UaPtcopZmen\k\JJMcqKI~HsxW;~z360774{W[G~Bdu7M@>..>[UmB{^gmJRGe<T7_;hPAga_..>.Rh?{YFI>>?Ne>...ULHhM{K|C>>>>\Zy`J36077491_.>>CWtC|mh~<36077492@R>.__>_..uN{rashxuFsxaN..>.pGl^ebZFz~_.>>I]qEqA]yxKOmxMLAoNn:@3R@xGY]B6>.>_07740vrCb_OkW8>.>_>.>_S{{FDky....Mr]3Lt[z\mwDCY6077445__>>jSnQbS;3>.__>_..6077496_Be>>__g|@h`u>..Krd>...@Gv36_i;z^T077473>.>_sTps:<[a360L>_____>>a@z=TBQv77485>._>pL____bQ^i3607>.>_7488....NmvnhR36077484>...bb;[T}ZAu=lBk}bi<36077477.>_>is>?MQFJ
```

Result without bloat:
>
```
2360774603607747736077491360774923607740836077445360774963607747336077485360774883607748436077477
```


Breaking that result without bloat down. The first digit **2** is the divisor that the key was divided by. Next we have the rest of te number where we can break it down into groups, 8 digits each.

Key Divisor (Scramble): 2

1st Letter: 36077460 "T"

2nd Letter: 36077477 "e"

3rd Letter: 36077491 "x"

4th Letter: 36077492 "t"

5th Letter: 36077408 "SPACE"

6th Letter: 36077445 "E"

7th Letter: 36077496 "x"

8th Letter: 36077473 "a"

9th Letter: 36077485 "m"

10th Letter: 36077488 "p"

11th Letter: 36077484 "l"

12th Letter: 36077477 "e"

When Decrypting, it takes the key and divides it by the scramble which is 2, then subrtacts that from the chunk/letter to get the ASCII char number.

72154752 / 2 = 36077376

I'll use the 1st letter for this example.

36077460 - 36077376 = 84

The ASCII code for 84 is "T".
