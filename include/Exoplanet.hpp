#pragma once
#include <string>

struct Exoplanet {
    int rowid, kepid;
    std::string kepoi_name, kepler_name, koi_disposition, koi_pdisposition;
    double koi_score;
    bool koi_fpflag_nt, koi_fpflag_ss, koi_fpflag_co, koi_fpflag_ec;
    double koi_period, koi_prad, koi_teq, koi_insol, koi_impact;
    double koi_time0bk, koi_duration, koi_depth;
    double koi_model_snr, koi_steff, koi_slogg, koi_srad;
    double ra, dec, koi_kepmag;
};