#include <sstream>
#include <tnt/component.h>
#include <tnt/componentfactory.h>
#include <tnt/httprequest.h>
#include <tnt/httpreply.h>
#include <cxxtools/log.h>
#include <strassenfestresult.h>
#include <strassenfestmanager.h>
#include <SessionShareds.h>

log_define("suche.controller")

namespace
{
  class sucheController : public tnt::Component
  {
    public:
      unsigned operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam);
  };

  static tnt::ComponentFactoryImpl<sucheController> factory("controller/suche");

  unsigned sucheController::operator() (tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam)
  {
    TNT_APPLICATION_SHARED_VAR(std::vector<std::string>, bezirke, ());
    TNT_REQUEST_SHARED_VAR(StrassenfestResult, strassenfestResult, ());

    TNT_SESSION_SHARED_VAR(SessionShareds, sessionShareds, ());

//     TNT_SESSION_SHARED_VAR(std::string, q, ());
//     TNT_SESSION_SHARED_VAR(std::string, bezirk, ());
//     TNT_SESSION_SHARED_VAR(std::string, von_from, ());
//     TNT_SESSION_SHARED_VAR(std::string, von_to, ());
//     TNT_SESSION_SHARED_VAR(cxxtools::Date, from, ());
//     TNT_SESSION_SHARED_VAR(cxxtools::Date, to, ());
//     TNT_SESSION_SHARED_VAR(unsigned, pageNo, (1));
//     TNT_SESSION_SHARED_VAR(unsigned, itemsPerPage, (20));

    log_debug("sucheController; sessionShareds.q=" << qparam.getUrl());

    StrassenfestManager manager;

    if (bezirke.empty())
    {
      bezirke = manager.getBezirke();
    }

    if (qparam.arg<bool>("suchen"))
    {
      log_debug("suchen");

      sessionShareds.q        = qparam.arg<std::string>("q");
      sessionShareds.bezirk   = qparam.arg<std::string>("bezirk");

      sessionShareds.von_from = qparam.arg<std::string>("von_from");
      sessionShareds.von_to   = qparam.arg<std::string>("von_to");

      if (!sessionShareds.von_from.empty())
        sessionShareds.from = cxxtools::Date(sessionShareds.von_from, "%d.%m.%Y");

      if (!sessionShareds.von_to.empty())
        sessionShareds.to = cxxtools::Date(sessionShareds.von_to, "%d.%m.%Y");

      sessionShareds.pageNo = 1;
      strassenfestResult = manager.search(sessionShareds.q, sessionShareds.bezirk, sessionShareds.from, sessionShareds.to, cxxtools::Date(), sessionShareds.itemsPerPage, sessionShareds.pageNo);
    }
    else if (qparam.arg<bool>("previousPage"))
    {
      --sessionShareds.pageNo;
      strassenfestResult = manager.search(sessionShareds.q, sessionShareds.bezirk, sessionShareds.from, sessionShareds.to, cxxtools::Date(), sessionShareds.itemsPerPage, sessionShareds.pageNo);
    }
    else if (qparam.arg<bool>("nextPage"))
    {
      ++sessionShareds.pageNo;
      strassenfestResult = manager.search(sessionShareds.q, sessionShareds.bezirk, sessionShareds.from, sessionShareds.to, cxxtools::Date(), sessionShareds.itemsPerPage, sessionShareds.pageNo);
    }

    // Return DECLINED to tell tntnet to continue processing with next mapping.
    // The next mapping will be the corresponding view.

    return DECLINED;
  }

}
