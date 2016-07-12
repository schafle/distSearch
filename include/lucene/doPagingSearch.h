#define NOMINMAX

#include "targetver.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "LuceneHeaders.h"
#include "FilterIndexReader.h"
#include "MiscUtils.h"

using namespace Lucene;

/// This demonstrates a typical paging search scenario, where the search engine presents pages of size n
/// to the user. The user can then go to the next page if interested in the next hits.
///
/// When the query is executed for the first time, then only enough results are collected to fill 5 result
/// pages. If the user wants to page beyond this limit, then the query is executed another time and all
/// hits are collected.
int doPagingSearch(const SearcherPtr& searcher, const QueryPtr& query, int32_t hitsPerPage, bool raw, bool interactive);
