#include <iostream>
#include <sstream>
#include <CanPacket.h>
#include <cmath>

#include <SQLiteCpp/SQLiteCpp.h>
#include <wlib/json>
#include <wlib/memory>

#include "database/DatabaseHandler.hh"
#include "includes/JSONProcessing.hh"

static constexpr unsigned s_poolSize = 16400 * 4;
static char s_memory[s_poolSize];

int main() {
    if (!wlp::mem::init(s_memory, s_poolSize)) {
        std::cout << "Failed to init memory" << std::endl;
    }

    DatabaseHandler *databaseHandler = DatabaseHandler::geInstance();
    
    /*Different Types
    0 - Controls (Int)
    1 - Controls (Float)
    2 - Sensors (Int)
    3 - Sensors (Float)
    */

    /*
        Packet(uint32_t id, uint64_t packet);
        Packet(uint32_t id, uint32_t data, uint32_t type);
        Packet(uint32_t id, float data, uint32_t type);
    */
    int i = 0;
    
    // Test Packet to JSON (1 and 4 are the same, 2, 3 and 5 are the same)
    wlp::Packet p1(i++, (uint32_t) 1070843560, 0);
    wlp::json_element j_p1 = parsePacket(p1);
    databaseHandler->storePacket(j_p1);

    wlp::Packet p2(i++, (uint32_t) 1070843560, 1);
    wlp::json_element j_p2 = parsePacket(p2);
    databaseHandler->storePacket(j_p2);

    wlp::Packet p3(i++, (uint64_t) 5365810856);
    wlp::json_element j_p3 = parsePacket(p3);
    databaseHandler->storePacket(j_p3);

    wlp::Packet p4(i++, 1.6545f, 0);
    wlp::json_element j_p4 = parsePacket(p4);
    databaseHandler->storePacket(j_p4);

    wlp::Packet p5(i++, 1.6545f, 1);
    wlp::json_element j_p5 = parsePacket(p5);
    databaseHandler->storePacket(j_p5);


    // Test JSON to Packet
    wlp::Packet p6 = createPacket(j_p1);
    p6.id() = i++;
    p6.type() = 2;
    wlp::json_element j_p6 = parsePacket(p6);
    databaseHandler->storePacket(j_p6);

    wlp::Packet p7 = createPacket(j_p1);
    p7.id() = i++;
    p7.type() = 3;
    wlp::json_element j_p7 = parsePacket(p7);
    databaseHandler->storePacket(j_p7);
}