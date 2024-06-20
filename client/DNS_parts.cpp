#include "DNS_parts.h"

dns::DNS_header::DNS_header()
{
    id=0;
    rd=0;
    tc=0;
    aa=0;
    opcode=0;
    qr=0;
    rcode=0;
    cd=0;
    ad=0;
    z=0;
    ra=0;
    q_count=0;
    ans_count=0;
    auth_count=0;
    add_count=0;
}

void dns::DNS_header::set_all(unsigned short id, bool rd, bool tc, bool aa, unsigned char opcode, bool qr, unsigned char rcode, bool cd, bool ad, bool z, bool ra, unsigned short q_count, unsigned short ans_count, unsigned short auth_count, unsigned short add_count)
{
    this->id = id;
    this->rd = rd;
    this->tc = tc;
    this->aa = aa;
    this->opcode = opcode;
    this->qr = qr;
    this->rcode = rcode;
    this->cd = cd;
    this->ad = ad;
    this->z = z;
    this->ra = ra;
    this->q_count = q_count;
    this->ans_count = ans_count;
    this->auth_count = auth_count;
    this->add_count = add_count;
}

void dns::question::set_all(unsigned short q_type, unsigned short q_class)
{
    this->q_type=q_type;
    this->q_class=q_class;
}
