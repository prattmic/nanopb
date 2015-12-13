#include <stdint.h>
#include <stdio.h>

#include "tests/cpp/person.pb.hpp"

void person(void) {
    Person p;
    p.set_id(20);

    uint8_t buf[256] = { 0 };
    if (!p.Encode(buf, 256)) {
        printf("Person encode failed");
        return;
    }

    auto p2 = Person::Decode(buf, 256);
    printf("id = %d\n", p2.get_id());
}

void phone_number(void) {
    Person_PhoneNumber p;
    p.set_type(nanopb::Person_PhoneType_WORK);

    uint8_t buf[256] = { 0 };
    if (!p.Encode(buf, 256)) {
        printf("Peron_PhoneNumber encode failed");
        return;
    }

    auto p2 = Person_PhoneNumber::Decode(buf, 256);
    printf("want = %d type = %d\n", nanopb::Person_PhoneType_WORK, p2.get_type());
}

void people(void) {
    Person p;
    p.set_id(50);

    People pp;
    pp.set_leader(p);
}

int main(void) {
    person();
    phone_number();
    people();
    return 0;
}
