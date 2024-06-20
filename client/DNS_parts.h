#include <iostream>
#include <string>

namespace dns
{

    class DNS_header
    { 
        unsigned short id;        // numar identificator unic, asociere raspuns cerere
        unsigned char rd: 1;      // recursion desired -> interogare recursiva
        unsigned char tc: 1;      // truncated message -> measaj trunchiat, datele nu incap intr-un sigur msj DNS
        unsigned char aa :1;      // authoritive answer -> DNS-ul care furnizeaza raspunsul este o autoritate pentru domeniul cerut în interogare
        unsigned char opcode :4;  // operation code -> tipul de operatie solicitata: 0 - cerere standard
                                    //                                                 1 - cerere inversa ( adresa ip -> nume domeniu)
                                    //                                                 2 - cerere status server
                                    //                                                 3 - 15 - utilizari viitoare
        unsigned char qr :1;      // query/response flag -> tipul de mesaj DNS: 0 - cerere
                                    //                                            1 - raspuns
        unsigned char rcode :4;   // response code -> prezent doar in cadrul raspunsurilor: 0 - succes
                                    //                                                        1 - 5 - diferite erori
                                    //                                                        6 - 15 - utilizari viitoare       
        unsigned char cd :1;      // checking disabled -> doar in cereri dns: 0 - verificarea DNSSEC 
                                    //                                          1 - ignora verificarile DNSSEC
        unsigned char ad :1;      // authenticated data -> doar in raspunsuri DNS: 0 - informatiile nu sunt autentificare in DNSSEC
                                    //                                               1 - informatiile sunt autentificare in DNSSEC
        unsigned char z :1;       // reserved -> utilizat pentru utilizari viitoare: trebuie sa fie seatat pe 0
        unsigned char ra :1;      // recursion available -> doar in raspunsuri DNS:  0 - serverul DNS nu accepta cereri recursive
                                    //                                                 1 - serverul DNS accepta cereri recursive
        unsigned short q_count;   // number of question entries -> numar de intrebari incluse in pachetul DNS
        unsigned short ans_count; // number of answer entries -> numarul de raspunsuri incluse in pachetul DNS
        unsigned short auth_count;// number of authority entries -> numarul de raspunsuri de autoritate
        unsigned short add_count; // number of resource entries -> informatii suplimentare (ex:  adrese IP suplimentare asociate cu un nume de domeniu )
    public:
        DNS_header();
        void set_all(unsigned short id, bool rd, bool tc, bool aa, unsigned char opcode, bool qr, unsigned char rcode,
                bool cd, bool ad, bool z, bool ra, unsigned short q_count, unsigned short ans_count,
                unsigned short auth_count, unsigned short add_count);

    };

    class question
    {
        unsigned short q_type;      // tipul de informatie pe care clientul DNS o solicita: 1 - A Ipv4
                                    //                                                     28 - AAAA Ipv6 
                                    //                                                     5 - CNMAE nume canonic (alias pentru alt nume de domeniu)
                                    //                                                     2 - NS nume server -> server DNS autoritare pentru un nume de domeniu
                                    //                                                     6 - SOA - informarii despre zona DNS                 
        unsigned short q_class;     // tipul de retea pentru care face cererea: 1 - internet class
                                    //                                          3 - Chaosnet  etc
    public:
        question():q_type(0),q_class(0){};
        void set_all(unsigned short q_type, unsigned short q_class);
    };

    #pragma pack(push,1);   //evitare padding

    class r_data
    {
        unsigned short type;
        unsigned short _class;     
        unsigned short data_len;
    public:
        r_data():type(0),_class(0),data_len(0){};
    };

    #pragma pack(pop);

    class resource_record
    {
        std::string name;
        std::string rdata;
        r_data *resource;
    public:
        resource_record():resource(nullptr){};
    };

    class query
    {
        std::string name;
        question *qst;
    public:
        query():qst(nullptr){};
    };

};
