#include <stdio.h>
#include <string.h>
#include <math.h>

//First we have to generate keys
// We take a input from user as key which is of 32 bits
// We now have to generate 5 ki where 1 <= i <=5
//we generate j here such that j = ( 4 * i ) - 3
// then we have to take 16 consecutive bits such that the starting position
// of k is the jth index
int keyE(unsigned int key, int r)
{
   int j = ( 4 * r ) - 3; 
   //printf("\n%d",j);
   if(j==1)
    {
      // here we have different condition for j as the the key index starts from 1 whereas
      // C's index starts from zero
      // also if we shift 32 bits to right the value of key will overflow  
      int k = key  >> 16;
      //we need first 16 bits therefore we right shift key by 16 bits
      // so the first 16 bits will be zero and remaining 16 bits will be our shifted bits
      //which is the value we wanted
      //printf("\n%d",k);
      return ((k));
    }
   else
   {
     //here we left shift the key by j indices and or it with right shift of key
     // with 32 -j indices so this way the first j indices and last 32 -j indices are eliminated
     //the remanining bits are right shifted so that we get our resultant 16 bits   
     int k =((key << j | (key >> (32 - j))) >> 16);
     //printf("\n%d",k);
     return (k);
   }

}
   
int substituion(int plaintext)
{
    // we have our plaintext xored with round keys for we now 
    // substitute it by this function
    int k=0;
    int res;
    int s[] = {0xE, 0x4, 0xD, 0x1,
               0x2, 0xF, 0xB, 0x8,
               0x3, 0xA, 0x6, 0xC, 
               0x5, 0x9, 0x0, 0x7};
   for(int i=0; i < 4;i++)
   {
     //the int k is mapped with s box values
     k = s[(plaintext >>(i*4))&0xf];
     //printf("\n%d",k);
     res += k << (i*4);
    // printf("\n%d", res);
   }
   return res;
}
int permutation(int plaintext)
{
    //the value of plaintext here is obtained by substitution of values of plaintext   
    int pkey=0;
    int P[] = {0,1,5,9,13,
              2,6,10,14,
              3,7,11,15,
              4,8,12,16};
    //here extra 0 is added to adjust the index           
    for(int i=0; i < 17;i++)
    {
      pkey = pkey | ((plaintext >> (P[i])) & 0x1) << (i);
      //printf(("%d", plaintext >> (P[i]));
      //printf("%d", pkey);
      
    }  
    return pkey;
  
}
int encrypt(int plaintext, int key)
{
  //we generate round keys xor it with plaintext and then the
  //resultant is substituted  and permutated for 3 times
  for(int i =1; i <= 3;i++)
  {
    plaintext = plaintext ^ keyE(key, i);
    plaintext = substituion(plaintext);
    plaintext = permutation(plaintext);
  }
  //for the 4th time we only xor and substitute 
  plaintext = plaintext ^ keyE(key, 4);
  plaintext = substituion(plaintext);
  //for the 5th time we only xor
  plaintext = plaintext ^ keyE(key, 5);
  return plaintext;
}
int decrypt(int plaintext, int key)
{  
    //for decryption we exactly do opposite operations as we did in
    //encryption
    plaintext = plaintext ^ keyE(key, 5);
    plaintext = substituion(plaintext);
    plaintext = plaintext ^ keyE(key, 4);
  
  for(int i =3; i >=1;i--)
  {
    plaintext = permutation(plaintext);
    plaintext = substituion(plaintext);
    plaintext = plaintext ^ keyE(key, i);  
   
  }

}

int main()
{
    int plaintext,k;
    char hex[50];
    char new[250];
    int key[32];
    int p = 15;
    printf("Enter 16 bits plain text: ");
    scanf("%x", &plaintext);
    printf("Enter the key of 32 bits: ");
    scanf("%x", &k);    
    printf("\nThe plaintext is = %x", plaintext);  
    printf("\nEncrypted text = %x", encrypt(plaintext,k));    
    printf("\nDecrypted text = %x", decrypt(plaintext,k));  
    return 0;
}
