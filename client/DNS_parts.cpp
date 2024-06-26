#include "DNS_parts.h"
#include <cstring>

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

void dns::DNS_header::set_all( bool rd, bool tc, bool aa, unsigned char opcode, bool qr, unsigned char rcode, bool cd, bool ad, bool z, bool ra, unsigned short q_count, unsigned short ans_count, unsigned short auth_count, unsigned short add_count)
{
    auto procces_id=getpid();
    this->id = htons(static_cast<unsigned short>(procces_id));
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
    this->q_count = htons(q_count);
    this->ans_count = ans_count;
    this->auth_count = auth_count;
    this->add_count = add_count;
}

void dns::question::set_all(unsigned short q_type, unsigned short q_class)
{
    this->q_type=q_type;
    this->q_class=htons(q_class);
}

void dns::question::get_data(char *ptr, int &size)
{
    std::memcpy(ptr,&q_type,sizeof(q_type));
    size+=sizeof(q_type);
    ptr+=sizeof(q_type);
    std::memcpy(ptr,&q_class,sizeof(q_class));
    size+=sizeof(q_class);
    ptr+=sizeof(q_class);

}

void dns::question::populate_question(char *&ptr)
{
    memcpy(&q_type,ptr,sizeof(q_type));
    ptr+=sizeof(q_type);
    memcpy(&q_class,ptr,sizeof(q_class));
    ptr+=sizeof(q_class);
}

void dns::print_record_type()
{
    std::cout<<"A\tNS\tCNAME\tSOA\tPTR\tMX\n";
}

dns::dns_record_type dns::get_record_type(std::string t_type)
{
   if(t_type=="A")
   {   
        return dns_record_type::A;
   }else if(t_type=="NS")
   {
        return dns_record_type::NS;
   }else if(t_type=="CNAME")
   {
        return dns_record_type::CNAME;
   }else if(t_type=="SOA")
   {
        return dns_record_type::SOA;
   }else if(t_type=="PTR")
   {
        return dns_record_type::PTR;
   }else if(t_type=="MX")
   {
        return dns_record_type::MX;
   }else 
   {
        return dns_record_type::NONE;
   }
}

dns::dns_record_type dns::check_type(std::string type)
{
    dns::dns_record_type r_type=dns::get_record_type(type);

    while(r_type==dns::NONE)
    {
        std::cout<<"Invalid record type.Re-enter record type:\n";
        std::string type;
        std::cin>>type;

        r_type=dns::get_record_type(type);
        
    }

    return r_type;
}


// www.youtube.com -> 3www7youtube3com
void dns::query::change_to_dns_name_format()
{
    std::string name;
    domain_name+=".";

    int start=0;
    int pos;
    while((pos=domain_name.find('.',start))!=std::string::npos)
    {
        unsigned char length=pos-start;
        name+=length;
        name+=domain_name.substr(start,length);
        start=pos+1;
    }

    domain_name=name;
}

dns::query::query(std::string name, unsigned short q_type, unsigned short q_class)
{
    this->set_domain_name(name);
    this->set_question(q_type,q_class);
}

void dns::query::set_domain_name(std::string name)
{
    domain_name=name;
    change_to_dns_name_format();
}

void dns::query::populate_query(char *&ptr, int qname_size)
{
    domain_name.assign(ptr,qname_size+1);
    ptr+=qname_size+1;

    qst=new dns::question;
    qst->populate_question(ptr);
}

void dns::query::set_question(unsigned short q_type, unsigned short q_class)
{
    if(qst==nullptr)
    {
        qst=new question;
    }

    qst->set_all(q_type,q_class);
}

void dns::query::get_data(char* ptr,int &size)
{
    std::memcpy(ptr,domain_name.c_str(),domain_name.size()+1);
    size+=domain_name.size()+1;
    ptr+=domain_name.size()+1;
    qst->get_data(ptr,size);
}

dns::query::~query()
{
    if(qst!=nullptr)
    {
        delete qst;
        qst=nullptr;
    }
}

//                             primul octet                 al 2 lea octet
//                     ------------------------------         ------------
//pointer compresie : |     11       |      000010   |      |   offset     | 
//                     ------------------------------        -------------   
//                    ptr. compresie     prt. sup. offset

std::string dns::resource_record::read_domain_name(char*&ptr)
{
    std::string name_aux;
    int offset;
    bool jumped=0;

    char* original_ptr=ptr;

    while(*ptr!=0)
    {
        if(*ptr>=0xC0)   //pointer de compresie
        {
            offset=((*ptr & 63)<<8)|*(ptr+1);
            ptr=original_ptr+offset;
            jumped=1;
            
        }
        else
        {
            int len=*ptr;
            ptr++;

            for(int i=0;i<len;i++)
            {
                name_aux+=*ptr;
                ptr++;
            }

            name_aux+='.';
        }


        if(!jumped)
        {
            ptr++;
        }
        else
        {
            jumped=false;
        }
    }

    name_aux.pop_back();    

    return name_aux;
}

void dns::resource_record::populate_record(char *&ptr)
{
    if(resource==nullptr)
    {
        resource=new dns::r_data;
    }

    resource->populate_rdata(ptr);
   
    if(resource->get_type()==1)
    {
        name.assign(ptr,resource->get_data_len());
        ptr=ptr+=resource->get_data_len();
    }
    else
    {
        rdata=read_domain_name(ptr);
    }

}

void dns::resource_record::populate_record2(char *&ptr)
{
    resource=(r_data*)(ptr);
    ptr+=sizeof(r_data);
}



dns::resource_record::~resource_record()
{
    if(resource!=nullptr)
    {
        delete resource;
        resource=nullptr;
    }

  
}

void dns::r_data::populate_rdata(char *&ptr)
{
    unsigned short value;

    memcpy(&value, ptr, sizeof(unsigned short));
    type = ntohs(value);
    ptr += sizeof(unsigned short);

    memcpy(&value, ptr, sizeof(unsigned short));
    _class = ntohs(value);
    ptr += sizeof(unsigned short);

    int value2;

    memcpy(&value2, ptr, sizeof(int));
    ttl = ntohs(value2);
    ptr += sizeof(int);

    memcpy(&value, ptr, sizeof(unsigned short));
   data_len = ntohs(value);
    ptr += sizeof(unsigned short);
}
