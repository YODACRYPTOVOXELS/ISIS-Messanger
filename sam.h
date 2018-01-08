#ifndef SAM_H
#define SAM_H

typedef enum{
    stream, datagram, raw
}SAMstyle;

class sam
{

private:
    void connect(const char * host, int port);


protected:
    int m_sock, m_port;
    bool error;
    char * m_host, * pubkey, * privkey;

public: // sesions
    void DeleteSession(const char * nickname);
    void CreateSession(const char * nickname="Isis messa(e)nger", SAMstyle s = stream );

public: // keys
    void loadKeys(const char * pubkey, const char * privkey);
    void generateDestination(void);
    char * getPubkey(void);
    char * getPrivkey(void);

public: // errors/is sam
    bool isSam(const char * host, int port);
    bool isSam(int sock, bool closeConnect=true);
    bool getError(void);

public: // construct/destruct
   explicit sam(const char * host, int port);
   ~sam();
};

#endif // SAM_H