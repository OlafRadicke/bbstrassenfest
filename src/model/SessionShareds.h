/*
 * Copyright (C) 2013 Tommi Maekitalo
 *
 */

#ifndef SESSIONSHAREDS_H
#define SESSIONSHAREDS_H

#include <strassenfest.h>


class SessionShareds
{

    public:
        SessionShareds(): from(), to(), pageNo(1), itemsPerPage(20) {};

        std::string q;
        std::string bezirk;
        std::string von_from;
        std::string von_to;
        cxxtools::Date from;
        cxxtools::Date to;
        unsigned pageNo;
        unsigned itemsPerPage;

  private:

};



#endif // STRASSENFESTRESULT_H

