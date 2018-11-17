#ifndef PODSERVER_DATABASE_H
#define PODSERVER_DATABASE_H

#include <string>

#include <wlib/json>
#include <SQLiteCpp/SQLiteCpp.h>
#include <CanPacket.h>

#define xstr(s) ___str(s)
#define ___str(s) #s

#if !defined(SENSOR_DB_FILE)
#error "Sensor Database file not defined"
#elif !defined(CONTROLS_DB_FILE)
#error "Controls Database file not defined"
#endif

class DatabaseHandler {
    SQLite::Database *m_pSensorDB;
    SQLite::Database *m_pControlsDB;
    std::string tableName;
    static DatabaseHandler *m_pInstance;

    DatabaseHandler(const std::string &sensorDBFilename, const std::string &controlsDBFilename);

    DatabaseHandler();

    ~DatabaseHandler();

public:
    /// Provides DatabaseHandler instance (singleton)
    static DatabaseHandler* geInstance();

    /// Stores CAN packet received into appropriate database
    /// @param json json_element after the CAN packet was parsed
    void storePacket(wlp::json_element &json);
};

#endif //PODSERVER_DATABASE_H
