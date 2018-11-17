#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>

#include <wlib/json>

#include "DatabaseHandler.hh"

DatabaseHandler* DatabaseHandler::m_pInstance = nullptr;

DatabaseHandler::DatabaseHandler(const std::string &sensorDBFilename, const std::string &controlsDBFilename) {
    try {
        this->m_pSensorDB = new SQLite::Database(sensorDBFilename, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        this->m_pControlsDB = new SQLite::Database(controlsDBFilename, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        // Create Tables for this run
        this->tableName = std::to_string(std::time(0));
        std::string statement = "\
            CREATE TABLE [" + this->tableName + "] (\
                id INT(32) PRIMARY KEY UNIQUE, \
                type INT(32), \
                data NUMERIC\
            );";
        SQLite::Statement createSensorTable(*(this->m_pSensorDB), statement);
        createSensorTable.exec();
        SQLite::Statement createControlsTable(*(this->m_pControlsDB), statement);
        createControlsTable.exec();
    
    } catch (SQLite::Exception &e) {
        std::cout << "SQLite exception: " << e.getErrorStr() << std::endl;
    }
}

DatabaseHandler::DatabaseHandler(): DatabaseHandler(xstr(SENSOR_DB_FILE), xstr(CONTROLS_DB_FILE)) {}

DatabaseHandler::~DatabaseHandler() {
    delete m_pSensorDB;
    delete m_pControlsDB;
}

bool checkZero(double a) {
    return ((int64_t)(round(a * 1000000))) == 0;
}
void DatabaseHandler::storePacket(wlp::json_element &json) {
    const uint32_t id = json["id"].as<uint32_t>();
    const uint32_t type = json["type"].as<uint32_t>(); // Where 0 - Controls, 1 - Sensors
    const uint32_t data = json["data"].as<uint32_t>();
    const float dataf = json["dataf"].as<float>();

    try {
        std::string insertStatement = "\
            INSERT INTO [" + this->tableName + "] (id, type, data)\
            VALUES(?, ?, ?);";
        SQLite::Statement insert((type == 2 || type == 3) ? *(this->m_pSensorDB) : *(this->m_pControlsDB), insertStatement);
        insert.bind(1, id);
        insert.bind(2, type);
        insert.bind(3, type % 2 ? dataf : data);
        insert.exec();
    }
    catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

DatabaseHandler *DatabaseHandler::geInstance() {
    if (m_pInstance == nullptr)
        m_pInstance = new DatabaseHandler();
    return m_pInstance;
}