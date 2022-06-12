#ifndef D3DES_H
#define D3DES_H

#define EN0	0	/* MODE == encrypt */
#define DE1	1	/* MODE == decrypt */
#define BLOCK_BYTES 8

class d3des
{
public:
	d3des();
	~d3des();
	d3des(int keySize, unsigned char* keyBytes);
	int Encrypt(char * from, char * into, int len);
	int Decrypt(char * from, char * into, int len);
	int Encrypt_Field(char * from, char * into, int len);
	int Decrypt_Field(char * from, char * into, int len);
private:
	unsigned char key[24];

	#define desDkey(a,b)	des3key((a),(b))
	#define makeDkey(a,b)	make3key((a),(b))
    #define useDkey(a)	use3key((a))
    #define cpDkey(a)	cp3key((a))

	void deskey(unsigned char *, short);
	void des2key(unsigned char *, short);
	void des3key(unsigned char *, short);
	void usekey(unsigned long *);
	void use3key(unsigned long *);
	void cpkey(unsigned long *);
	void cp3key(unsigned long *);
	void des(unsigned char *, unsigned char *);

	void Ddes(unsigned char *, unsigned char *);
	void D2des(unsigned char *, unsigned char *);
	void D3des(unsigned char *, unsigned char *);
	void makekey(char *, unsigned char *);
	void make2key(char *, unsigned char *);
	void make3key(char *, unsigned char *);

	void scrunch(unsigned char *, unsigned long *);
	void unscrun(unsigned long *, unsigned char *);
	void desfunc(unsigned long *, unsigned long *);
	void cookey(unsigned long *);

	int DesDecrypt(unsigned char *szKey, unsigned char *lpbySrc, unsigned char * lpbyDest, int iLength);
	int DesEncrypt(unsigned char *szKey, unsigned  char *lpbySrc, unsigned char * lpbyDest, int iLength); 
};

#endif // D3DES_H
