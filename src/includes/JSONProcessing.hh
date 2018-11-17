#ifndef JSON_PROCESSING_H
#define JSON_PROCESSING_H

#include <iostream>
#include <sstream>
#include <CanPacket.h>
#include <cmath>

#include <wlib/json>
#include <wlib/memory>

// CAN Packet to JSON_Element
wlp::json_element parsePacket(wlp::Packet &packet) {
    const uint32_t id = packet.id();
    /*Different Types
    0 - Controls (Int)
    1 - Controls (Float)
    2 - Sensors (Int)
    3 - Sensors (Float)
    */
    const uint32_t type = packet.type();
    const uint32_t data = packet.data();
    const float dataf = packet.dataf();

    char buffer[1024];
    wlp::json_object payload = {
        "id", id,
        "type", type,
        "data", data,
        "dataf", dataf
    };
    wlp::json_element element = wlp::move(payload);
    wlp::json::stringify(buffer, element);

#ifdef DEBUG
    std::cout << "Incoming Packet: " << std::endl << buffer << std::endl << std::endl;
#endif

    return element;
}

// JSON_Element to CAN Packet
wlp::Packet createPacket(wlp::json_element &json) {
    const uint32_t id = json["id"].as<uint32_t>();
    const uint32_t type = json["type"].as<uint32_t>();
    const uint32_t data = json["data"].as<uint32_t>();
    const float dataf = json["dataf"].as<float>();

    wlp::Packet packet(id);
    packet.type() = type;
    if (type % 2)
        packet.dataf() = dataf;
    else
        packet.data() = data;
    return packet;
}

#endif