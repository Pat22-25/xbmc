/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

class TiXmlElement;
namespace XFILE
{
class CCurlFile;
}

class CScraperUrl
{
public:
  enum class UrlType
  {
    General = 1,
    Season = 2
  };

  struct SUrlEntry
  {
    SUrlEntry() : m_type(UrlType::General), m_post(false), m_isgz(false), m_season(-1) {}

    std::string m_spoof;
    std::string m_url;
    std::string m_cache;
    std::string m_aspect;
    UrlType m_type;
    bool m_post;
    bool m_isgz;
    int m_season;
  };

  CScraperUrl();
  explicit CScraperUrl(std::string strUrl);
  explicit CScraperUrl(const TiXmlElement* element);
  ~CScraperUrl();

  const std::string& GetTitle() const { return m_title; }
  void SetTitle(std::string title) { m_title = std::move(title); }

  double GetRelevance() const { return m_relevance; }
  void SetRelevance(double relevance) { m_relevance = relevance; }

  bool Parse();
  bool ParseString(std::string strUrl); // copies by intention
  bool ParseElement(const TiXmlElement* element);
  bool ParseEpisodeGuide(std::string strUrls); // copies by intention
  void AddElement(std::string url,
                  std::string aspect = "",
                  std::string preview = "",
                  std::string referrer = "",
                  std::string cache = "",
                  bool post = false,
                  bool isgz = false,
                  int season = -1);

  const SUrlEntry GetFirstThumb(const std::string& type = "") const;
  const SUrlEntry GetSeasonThumb(int season, const std::string& type = "") const;
  unsigned int GetMaxSeasonThumb() const;

  /*! \brief fetch the full URL (including referrer) of a thumb
   \param URL entry to use to create the full URL
   \return the full URL, including referrer
   */
  static std::string GetThumbURL(const CScraperUrl::SUrlEntry& entry);

  /*! \brief fetch the full URLs (including referrer) of thumbs
   \param thumbs [out] vector of thumb URLs to fill
   \param type the type of thumb URLs to fetch, if empty (the default) picks any
   \param season number of season that we want thumbs for, -1 indicates no season (the default)
   \param unique avoid adding duplicate URLs when adding to a thumbs vector with existing items
   */
  void GetThumbURLs(std::vector<std::string>& thumbs,
                    const std::string& type = "",
                    int season = -1,
                    bool unique = false) const;
  void Clear();
  static bool Get(const SUrlEntry& scrURL,
                  std::string& strHTML,
                  XFILE::CCurlFile& http,
                  const std::string& cacheContext);

  std::string m_xml;
  std::string m_id;
  std::vector<SUrlEntry> m_url;

private:
  std::string m_title;
  double m_relevance;
};
