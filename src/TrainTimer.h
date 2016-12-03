#ifndef TRAINTIMER_H
#define TRAINTIMER_H

enum DistanceUnits {
    METRIC,
    IMPERIAL
};

enum RailroadScale {
    RAILROADSCALE_O,
    RAILROADSCALE_HO,
    RAILROADSCALE_N
};

enum MetricDistance {
    METRIC_CM,
    METRIC_M,
    METRIC_KM
};

enum ImperialDistance {
    IMPERIAL_IN,
    IMPERIAL_FT,
    IMPERIAL_MI
};



class TrainTimerEEPROM {
public:

    RailroadScale getScale()    { return scale; }
    DistanceUnits getUnits()    { return units; }
    double        getDistance() { return distance; }
    
private:
    long maxTime;        // max timing in milliseconds
    RailroadScale scale;

    DistanceUnits units;

    // if (units == Imperial) this is in inches
    // if (units == Metric) this is in centimeters
    double distance;

    void initDefaults();
};


#endif
