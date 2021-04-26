#ifndef DRONE_GPS_STRUCT_H
#define DRONE_GPS_STRUCT_H

struct GPSCoordinates {
    float longitude = 0, latitude = 0, altitude = 0;

    GPSCoordinates() {

    }

    GPSCoordinates(float altitude, float longitude, float latitude) {
        this->altitude = altitude;
        this->longitude = longitude;
        this->latitude = latitude;
    }

    GPSCoordinates(const GPSCoordinates& g) {
        this->altitude = g.altitude;
        this->longitude = g.longitude;
        this->latitude = g.latitude;
    }
};

#endif